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
        messages = Message.objects.filter(
            conversation_id=conversation_id,
            conversation__participants__user=request.user
        ).select_related("sender")

        serializer = MessageSerializer(messages, many=True)
        return Response(serializer.data)

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

        message.delete()

        return Response(status=status.HTTP_204_NO_CONTENT)