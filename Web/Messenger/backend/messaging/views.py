from django.contrib.auth import get_user_model
from django.db.models import F, Max
from django.utils import timezone

from rest_framework import status
from rest_framework.permissions import IsAuthenticated
from rest_framework.views import APIView
from rest_framework.response import Response

from .models import Conversation, Message
from .serializers import (
    ConversationCreateSerializer,
    ConversationSerializer,
    MessageSerializer,
)


class ConversationListView(APIView):
    permission_classes = [IsAuthenticated]

    def get(self, request):
        conversations = (
            Conversation.objects.filter(participants__user=request.user)
            .prefetch_related("participants__user")
            .annotate(last_message_at=Max("messages__created_at"))
            .order_by(
                F("last_message_at").desc(nulls_last=True),
                "-updated_at",
            )
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
            context={"request": request}
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

    def get(self, request, conversation_id):
        conversation = (
            Conversation.objects.filter(
                id=conversation_id,
                participants__user=request.user,
            )
            .prefetch_related("participants__user")
            .first()
        )

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

class ConversationMessagesView(APIView):
    permission_classes = [IsAuthenticated]

    def get(self, request, conversation_id):
        base_messages = Message.objects.filter(
            conversation_id=conversation_id,
            conversation__participants__user=request.user,
        ).select_related("sender")

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

            serializer = MessageSerializer(page_messages, many=True)

            return Response(
                {
                    "results": serializer.data,
                    "has_more": False,
                    "next_before": None,
                }
            )

        if not limit_param and not before_param:
            serializer = MessageSerializer(base_messages, many=True)
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

        newest_first_messages = list(
            base_messages.order_by("-id")[: limit + 1]
        )
        has_more = len(newest_first_messages) > limit
        page_messages = newest_first_messages[:limit]
        page_messages.reverse()

        serializer = MessageSerializer(page_messages, many=True)

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
        conversation = Conversation.objects.filter(
            id=conversation_id,
            participants__user=request.user
        ).first()

        if not conversation:
            return Response(
                {"detail": "Conversation not found."},
                status=status.HTTP_404_NOT_FOUND
            )

        serializer = MessageSerializer(data=request.data)

        if serializer.is_valid():
            serializer.save(
                conversation=conversation,
                sender=request.user
            )
            return Response(serializer.data, status=status.HTTP_201_CREATED)

        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)
    
class ConversationMarkReadView(APIView):
    permission_classes = [IsAuthenticated]

    def post(self, request, conversation_id):
        conversation = Conversation.objects.filter(
            id=conversation_id,
            participants__user=request.user,
        ).first()

        if not conversation:
            return Response(
                {"detail": "Conversation not found."},
                status=status.HTTP_404_NOT_FOUND,
            )

        updated_count = Message.objects.filter(
            conversation=conversation,
            is_read=False,
            is_deleted=False,
        ).exclude(
            sender=request.user,
        ).update(is_read=True)

        return Response(
            {
                "detail": "Messages marked as read.",
                "updated_count": updated_count,
            }
        )

class MessageDetailView(APIView):
    permission_classes = [IsAuthenticated]

    def patch(self, request, message_id):
        message = Message.objects.filter(
            id=message_id,
            sender=request.user,
        ).select_related("sender").first()

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

        if "text" not in request.data:
            return Response(
                {"text": ["Message text is required."]},
                status=status.HTTP_400_BAD_REQUEST,
            )

        serializer = MessageSerializer(
            message,
            data=request.data,
            partial=True,
        )

        if serializer.is_valid():
            serializer.save(edited_at=timezone.now())
            return Response(serializer.data)

        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)

    def delete(self, request, message_id):
        message = Message.objects.filter(
            id=message_id,
            sender=request.user,
        ).first()

        if not message:
            return Response(
                {"detail": "Message not found."},
                status=status.HTTP_404_NOT_FOUND,
            )

        message.text = ""
        message.is_deleted = True
        message.deleted_at = timezone.now()
        message.save(update_fields=["text", "is_deleted", "deleted_at", "updated_at"])

        serializer = MessageSerializer(message)
        return Response(serializer.data, status=status.HTTP_200_OK)