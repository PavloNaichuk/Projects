import os

from asgiref.sync import async_to_sync
from channels.layers import get_channel_layer
from django.contrib.auth import get_user_model
from django.core.files.base import ContentFile
from django.db.models import F, Max
from django.utils import timezone

from rest_framework import status
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from rest_framework.views import APIView

from .consumers import ACTIVE_USER_CONNECTIONS
from .models import Conversation, Message, MessageReaction
from .serializers import (
    ConversationCreateSerializer,
    ConversationSerializer,
    MessageSerializer,
)


MESSAGE_SELECT_RELATED = (
    "sender",
    "reply_to",
    "reply_to__sender",
    "forwarded_from",
    "forwarded_from__sender",
)


def notify_conversation_deleted(conversation_id, participant_ids):
    channel_layer = get_channel_layer()

    if not channel_layer:
        return

    for user_id in participant_ids:
        async_to_sync(channel_layer.group_send)(
            f"user_{user_id}_notifications",
            {
                "type": "conversation_deleted",
                "conversation_id": conversation_id,
            },
        )


def notify_message_created(conversation_id, participant_ids, message_data):
    channel_layer = get_channel_layer()

    if not channel_layer:
        return

    async_to_sync(channel_layer.group_send)(
        f"conversation_{conversation_id}",
        {
            "type": "chat_message",
            "message": message_data,
        },
    )

    for user_id in participant_ids:
        async_to_sync(channel_layer.group_send)(
            f"user_{user_id}_notifications",
            {
                "type": "sidebar_message",
                "message": message_data,
            },
        )


def notify_message_delivered(conversation_id, message_ids, user_id):
    channel_layer = get_channel_layer()

    if not channel_layer:
        return

    async_to_sync(channel_layer.group_send)(
        f"conversation_{conversation_id}",
        {
            "type": "delivery_event",
            "message_ids": message_ids,
            "user_id": user_id,
        },
    )


def notify_message_reaction_updated(conversation_id, message_data):
    channel_layer = get_channel_layer()

    if not channel_layer:
        return

    async_to_sync(channel_layer.group_send)(
        f"conversation_{conversation_id}",
        {
            "type": "message_updated",
            "message": message_data,
        },
    )


def save_attachment_metadata(message, uploaded_file):
    if not uploaded_file:
        return

    message.attachment_name = uploaded_file.name
    message.attachment_content_type = uploaded_file.content_type or ""
    message.attachment_size = uploaded_file.size
    message.save(
        update_fields=[
            "attachment_name",
            "attachment_content_type",
            "attachment_size",
            "updated_at",
        ]
    )


def copy_message_attachment(source_message, target_message):
    if not source_message.attachment:
        return

    source_message.attachment.open("rb")
    file_content = source_message.attachment.read()
    source_message.attachment.close()

    file_name = source_message.attachment_name or os.path.basename(
        source_message.attachment.name
    )

    target_message.attachment.save(
        file_name,
        ContentFile(file_content),
        save=False,
    )
    target_message.attachment_name = source_message.attachment_name
    target_message.attachment_content_type = source_message.attachment_content_type
    target_message.attachment_size = source_message.attachment_size
    target_message.save(
        update_fields=[
            "attachment",
            "attachment_name",
            "attachment_content_type",
            "attachment_size",
            "updated_at",
        ]
    )


def mark_message_as_delivered_for_online_participants(message, participant_ids):
    if message.is_delivered or message.is_deleted:
        return

    delivered_user_id = None

    for user_id in participant_ids:
        if user_id == message.sender_id:
            continue

        if ACTIVE_USER_CONNECTIONS.get(user_id, 0) > 0:
            delivered_user_id = user_id
            break

    if not delivered_user_id:
        return

    message.is_delivered = True
    message.delivered_at = timezone.now()
    message.save(update_fields=["is_delivered", "delivered_at", "updated_at"])

    notify_message_delivered(
        conversation_id=message.conversation_id,
        message_ids=[message.id],
        user_id=delivered_user_id,
    )


def serialize_message(message, request):
    message = (
        Message.objects.select_related(*MESSAGE_SELECT_RELATED)
        .prefetch_related("reactions__user")
        .get(id=message.id)
    )

    serializer = MessageSerializer(
        message,
        context={"request": request},
    )

    return serializer.data


class ConversationListView(APIView):
    permission_classes = [IsAuthenticated]

    def get(self, request):
        conversations = (
            Conversation.objects.filter(participants__user=request.user)
            .exclude(hidden_for=request.user)
            .prefetch_related("participants__user")
            .annotate(last_message_at=Max("messages__created_at"))
            .order_by(
                F("last_message_at").desc(nulls_last=True),
                "-updated_at",
            )
            .distinct()
        )

        serializer = ConversationSerializer(
            conversations,
            many=True,
            context={"request": request},
        )
        return Response(serializer.data)

    def post(self, request):
        serializer = ConversationCreateSerializer(
            data=request.data,
            context={"request": request},
        )

        if not serializer.is_valid():
            return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)

        user_id = serializer.validated_data["user_id"]

        User = get_user_model()
        target_user = User.objects.get(id=user_id)

        existing_conversation = (
            Conversation.objects.filter(participants__user=request.user)
            .filter(participants__user=target_user)
            .distinct()
            .first()
        )

        if existing_conversation:
            existing_conversation.hidden_for.remove(request.user)

            response_serializer = ConversationSerializer(
                existing_conversation,
                context={"request": request},
            )
            return Response(response_serializer.data, status=status.HTTP_200_OK)

        conversation = Conversation.objects.create()

        conversation.participants.create(user=request.user)
        conversation.participants.create(user=target_user)

        response_serializer = ConversationSerializer(
            conversation,
            context={"request": request},
        )
        return Response(response_serializer.data, status=status.HTTP_201_CREATED)


class ConversationDetailView(APIView):
    permission_classes = [IsAuthenticated]

    def get_conversation(self, request, conversation_id, include_hidden=False):
        queryset = Conversation.objects.filter(
            id=conversation_id,
            participants__user=request.user,
        ).prefetch_related("participants__user")

        if not include_hidden:
            queryset = queryset.exclude(hidden_for=request.user)

        return queryset.first()

    def get(self, request, conversation_id):
        conversation = self.get_conversation(request, conversation_id)

        if not conversation:
            return Response(
                {"detail": "Conversation not found."},
                status=status.HTTP_404_NOT_FOUND,
            )

        serializer = ConversationSerializer(
            conversation,
            context={"request": request},
        )
        return Response(serializer.data)

    def delete(self, request, conversation_id):
        delete_mode = request.query_params.get("mode", "for_me")

        if delete_mode not in ("for_me", "for_everyone"):
            return Response(
                {"mode": ["Mode must be 'for_me' or 'for_everyone'."]},
                status=status.HTTP_400_BAD_REQUEST,
            )

        conversation = self.get_conversation(
            request,
            conversation_id,
            include_hidden=True,
        )

        if not conversation:
            return Response(
                {"detail": "Conversation not found."},
                status=status.HTTP_404_NOT_FOUND,
            )

        if delete_mode == "for_me":
            conversation.hidden_for.add(request.user)

            return Response(
                {
                    "detail": "Conversation deleted for you.",
                    "conversation_id": conversation_id,
                    "mode": "for_me",
                },
                status=status.HTTP_200_OK,
            )

        participant_ids = list(
            conversation.participants.values_list("user_id", flat=True)
        )

        conversation.delete()
        notify_conversation_deleted(conversation_id, participant_ids)

        return Response(
            {
                "detail": "Conversation deleted for everyone.",
                "conversation_id": conversation_id,
                "mode": "for_everyone",
            },
            status=status.HTTP_200_OK,
        )


class ConversationMuteView(APIView):
    permission_classes = [IsAuthenticated]

    def post(self, request, conversation_id):
        conversation = (
            Conversation.objects.filter(
                id=conversation_id,
                participants__user=request.user,
            )
            .exclude(
                hidden_for=request.user,
            )
            .prefetch_related("participants__user")
            .first()
        )

        if not conversation:
            return Response(
                {"detail": "Conversation not found."},
                status=status.HTTP_404_NOT_FOUND,
            )

        requested_is_muted = request.data.get("is_muted")

        if requested_is_muted is None:
            should_mute = not conversation.muted_for.filter(
                id=request.user.id
            ).exists()
        else:
            should_mute = requested_is_muted is True or str(
                requested_is_muted
            ).lower() == "true"

        if should_mute:
            conversation.muted_for.add(request.user)
            detail = "Conversation muted."
        else:
            conversation.muted_for.remove(request.user)
            detail = "Conversation unmuted."

        serializer = ConversationSerializer(
            conversation,
            context={"request": request},
        )

        return Response(
            {
                "detail": detail,
                "conversation": serializer.data,
            },
            status=status.HTTP_200_OK,
        )


class ConversationPinView(APIView):
    permission_classes = [IsAuthenticated]

    def post(self, request, conversation_id):
        conversation = (
            Conversation.objects.filter(
                id=conversation_id,
                participants__user=request.user,
            )
            .exclude(
                hidden_for=request.user,
            )
            .prefetch_related("participants__user")
            .first()
        )

        if not conversation:
            return Response(
                {"detail": "Conversation not found."},
                status=status.HTTP_404_NOT_FOUND,
            )

        requested_is_pinned = request.data.get("is_pinned")

        if requested_is_pinned is None:
            should_pin = not conversation.pinned_for.filter(
                id=request.user.id
            ).exists()
        else:
            should_pin = requested_is_pinned is True or str(
                requested_is_pinned
            ).lower() == "true"

        if should_pin:
            conversation.pinned_for.add(request.user)
            detail = "Conversation pinned."
        else:
            conversation.pinned_for.remove(request.user)
            detail = "Conversation unpinned."

        serializer = ConversationSerializer(
            conversation,
            context={"request": request},
        )

        return Response(
            {
                "detail": detail,
                "conversation": serializer.data,
            },
            status=status.HTTP_200_OK,
        )


class ConversationMessagesView(APIView):
    permission_classes = [IsAuthenticated]

    def get(self, request, conversation_id):
        base_messages = (
            Message.objects.filter(
                conversation_id=conversation_id,
                conversation__participants__user=request.user,
            )
            .exclude(
                conversation__hidden_for=request.user,
            )
            .select_related(*MESSAGE_SELECT_RELATED)
            .prefetch_related("reactions__user")
        )

        limit_param = request.query_params.get("limit")
        before_param = request.query_params.get("before")
        search_param = request.query_params.get("search", "").strip()

        if search_param:
            search_messages = base_messages.filter(
                text__icontains=search_param,
                is_deleted=False,
            ).order_by("-id")[:50]

            page_messages = list(search_messages)
            page_messages.reverse()

            serializer = MessageSerializer(
                page_messages,
                many=True,
                context={"request": request},
            )

            return Response(
                {
                    "results": serializer.data,
                    "has_more": False,
                    "next_before": None,
                }
            )

        if not limit_param and not before_param:
            serializer = MessageSerializer(
                base_messages,
                many=True,
                context={"request": request},
            )
            return Response(serializer.data)

        try:
            limit = int(limit_param or 20)
        except ValueError:
            return Response(
                {"limit": ["Limit must be a number."]},
                status=status.HTTP_400_BAD_REQUEST,
            )

        if limit < 1:
            return Response(
                {"limit": ["Limit must be greater than 0."]},
                status=status.HTTP_400_BAD_REQUEST,
            )

        limit = min(limit, 50)

        if before_param:
            try:
                before_id = int(before_param)
            except ValueError:
                return Response(
                    {"before": ["Before must be a message id."]},
                    status=status.HTTP_400_BAD_REQUEST,
                )

            base_messages = base_messages.filter(id__lt=before_id)

        newest_first_messages = list(base_messages.order_by("-id")[: limit + 1])
        has_more = len(newest_first_messages) > limit
        page_messages = newest_first_messages[:limit]
        page_messages.reverse()

        serializer = MessageSerializer(
            page_messages,
            many=True,
            context={"request": request},
        )

        next_before = None
        if has_more and page_messages:
            next_before = page_messages[0].id

        return Response(
            {
                "results": serializer.data,
                "has_more": has_more,
                "next_before": next_before,
            }
        )

    def post(self, request, conversation_id):
        conversation = (
            Conversation.objects.filter(
                id=conversation_id,
                participants__user=request.user,
            )
            .prefetch_related("participants")
            .first()
        )

        if not conversation:
            return Response(
                {"detail": "Conversation not found."},
                status=status.HTTP_404_NOT_FOUND,
            )

        conversation.hidden_for.clear()

        serializer = MessageSerializer(
            data=request.data,
            context={
                "request": request,
                "conversation": conversation,
            },
        )

        if serializer.is_valid():
            message = serializer.save(
                conversation=conversation,
                sender=request.user,
            )

            uploaded_file = request.FILES.get("attachment")
            save_attachment_metadata(message, uploaded_file)

            message_data = serialize_message(message, request)

            participant_ids = list(
                conversation.participants.values_list("user_id", flat=True)
            )

            notify_message_created(
                conversation_id=conversation.id,
                participant_ids=participant_ids,
                message_data=message_data,
            )

            mark_message_as_delivered_for_online_participants(
                message=message,
                participant_ids=participant_ids,
            )

            return Response(message_data, status=status.HTTP_201_CREATED)

        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)


class ConversationMarkReadView(APIView):
    permission_classes = [IsAuthenticated]

    def post(self, request, conversation_id):
        conversation = (
            Conversation.objects.filter(
                id=conversation_id,
                participants__user=request.user,
            )
            .exclude(
                hidden_for=request.user,
            )
            .first()
        )

        if not conversation:
            return Response(
                {"detail": "Conversation not found."},
                status=status.HTTP_404_NOT_FOUND,
            )

        messages = Message.objects.filter(
            conversation=conversation,
            is_read=False,
            is_deleted=False,
        ).exclude(
            sender=request.user,
        )

        now = timezone.now()

        messages.filter(is_delivered=False).update(
            is_delivered=True,
            delivered_at=now,
        )

        updated_count = messages.update(is_read=True)

        return Response(
            {
                "detail": "Messages marked as read.",
                "updated_count": updated_count,
            }
        )


class MessageDetailView(APIView):
    permission_classes = [IsAuthenticated]

    def patch(self, request, message_id):
        message = (
            Message.objects.filter(
                id=message_id,
                sender=request.user,
                conversation__participants__user=request.user,
            )
            .exclude(
                conversation__hidden_for=request.user,
            )
            .select_related("sender")
            .first()
        )

        if not message:
            return Response(
                {"detail": "Message not found."},
                status=status.HTTP_404_NOT_FOUND,
            )

        if message.is_deleted:
            return Response(
                {"detail": "Deleted message cannot be edited."},
                status=status.HTTP_400_BAD_REQUEST,
            )

        remove_attachment = request.data.get("remove_attachment")

        if remove_attachment is True or str(remove_attachment).lower() == "true":
            if not message.attachment:
                return Response(
                    {"detail": "Attachment not found."},
                    status=status.HTTP_400_BAD_REQUEST,
                )

            message.attachment.delete(save=False)

            message.attachment = None
            message.attachment_name = ""
            message.attachment_content_type = ""
            message.attachment_size = None

            update_fields = [
                "attachment",
                "attachment_name",
                "attachment_content_type",
                "attachment_size",
                "updated_at",
            ]

            if not message.text.strip():
                message.text = ""
                message.is_deleted = True
                message.deleted_at = timezone.now()
                update_fields.extend(["text", "is_deleted", "deleted_at"])

            message.save(update_fields=update_fields)

            message_data = serialize_message(message, request)

            return Response(message_data)

        if "text" not in request.data:
            return Response(
                {"text": ["Message text is required."]},
                status=status.HTTP_400_BAD_REQUEST,
            )

        serializer = MessageSerializer(
            message,
            data=request.data,
            partial=True,
            context={"request": request},
        )

        if serializer.is_valid():
            serializer.save(edited_at=timezone.now())

            message_data = serialize_message(message, request)

            return Response(message_data)

        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)

    def delete(self, request, message_id):
        message = (
            Message.objects.filter(
                id=message_id,
                sender=request.user,
                conversation__participants__user=request.user,
            )
            .exclude(
                conversation__hidden_for=request.user,
            )
            .first()
        )

        if not message:
            return Response(
                {"detail": "Message not found."},
                status=status.HTTP_404_NOT_FOUND,
            )

        if message.attachment:
            message.attachment.delete(save=False)

        message.text = ""
        message.attachment = None
        message.attachment_name = ""
        message.attachment_content_type = ""
        message.attachment_size = None
        message.is_deleted = True
        message.deleted_at = timezone.now()
        message.save(
            update_fields=[
                "text",
                "attachment",
                "attachment_name",
                "attachment_content_type",
                "attachment_size",
                "is_deleted",
                "deleted_at",
                "updated_at",
            ]
        )

        message_data = serialize_message(message, request)

        return Response(message_data, status=status.HTTP_200_OK)


class MessageReactionToggleView(APIView):
    permission_classes = [IsAuthenticated]

    allowed_emojis = {"👍", "❤️", "😂", "😮"}

    def post(self, request, message_id):
        emoji = str(request.data.get("emoji", "")).strip()

        if emoji not in self.allowed_emojis:
            return Response(
                {"emoji": ["Unsupported reaction."]},
                status=status.HTTP_400_BAD_REQUEST,
            )

        message = (
            Message.objects.filter(
                id=message_id,
                conversation__participants__user=request.user,
            )
            .exclude(
                conversation__hidden_for=request.user,
            )
            .select_related("conversation")
            .first()
        )

        if not message:
            return Response(
                {"detail": "Message not found."},
                status=status.HTTP_404_NOT_FOUND,
            )

        if message.is_deleted:
            return Response(
                {"detail": "Cannot react to deleted message."},
                status=status.HTTP_400_BAD_REQUEST,
            )

        reaction = MessageReaction.objects.filter(
            message=message,
            user=request.user,
            emoji=emoji,
        ).first()

        if reaction:
            reaction.delete()
            action = "removed"
        else:
            MessageReaction.objects.create(
                message=message,
                user=request.user,
                emoji=emoji,
            )
            action = "added"

        message_data = serialize_message(message, request)

        notify_message_reaction_updated(
            conversation_id=message.conversation_id,
            message_data=message_data,
        )

        return Response(
            {
                "action": action,
                "message": message_data,
            },
            status=status.HTTP_200_OK,
        )


class MessageForwardView(APIView):
    permission_classes = [IsAuthenticated]

    def post(self, request, message_id):
        target_conversation_id = request.data.get("conversation_id")

        if not target_conversation_id:
            return Response(
                {"conversation_id": ["Conversation id is required."]},
                status=status.HTTP_400_BAD_REQUEST,
            )

        source_message = (
            Message.objects.filter(
                id=message_id,
                conversation__participants__user=request.user,
            )
            .exclude(
                conversation__hidden_for=request.user,
            )
            .select_related("sender")
            .first()
        )

        if not source_message:
            return Response(
                {"detail": "Message not found."},
                status=status.HTTP_404_NOT_FOUND,
            )

        if source_message.is_deleted:
            return Response(
                {"detail": "Cannot forward deleted message."},
                status=status.HTTP_400_BAD_REQUEST,
            )

        target_conversation = (
            Conversation.objects.filter(
                id=target_conversation_id,
                participants__user=request.user,
            )
            .prefetch_related("participants")
            .first()
        )

        if not target_conversation:
            return Response(
                {"detail": "Target conversation not found."},
                status=status.HTTP_404_NOT_FOUND,
            )

        target_conversation.hidden_for.clear()

        forwarded_message = Message.objects.create(
            conversation=target_conversation,
            sender=request.user,
            text=source_message.text,
            forwarded_from=source_message,
        )

        copy_message_attachment(source_message, forwarded_message)

        message_data = serialize_message(forwarded_message, request)

        participant_ids = list(
            target_conversation.participants.values_list("user_id", flat=True)
        )

        notify_message_created(
            conversation_id=target_conversation.id,
            participant_ids=participant_ids,
            message_data=message_data,
        )

        mark_message_as_delivered_for_online_participants(
            message=forwarded_message,
            participant_ids=participant_ids,
        )

        return Response(message_data, status=status.HTTP_201_CREATED)