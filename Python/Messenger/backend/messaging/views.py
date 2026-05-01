from django.contrib.auth import get_user_model

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
        conversations = Conversation.objects.filter(
            participants__user=request.user
        ).prefetch_related("participants__user")

        serializer = ConversationSerializer(conversations, many=True)
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
            response_serializer = ConversationSerializer(existing_conversation)
            return Response(response_serializer.data, status=status.HTTP_200_OK)

        conversation = Conversation.objects.create()

        conversation.participants.create(user=request.user)
        conversation.participants.create(user=target_user)

        response_serializer = ConversationSerializer(conversation)
        return Response(response_serializer.data, status=status.HTTP_201_CREATED)


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