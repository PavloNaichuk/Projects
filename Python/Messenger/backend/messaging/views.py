from rest_framework import status
from rest_framework.permissions import IsAuthenticated
from rest_framework.views import APIView
from rest_framework.response import Response

from .models import Conversation, Message
from .serializers import ConversationSerializer, MessageSerializer


class ConversationListView(APIView):
    permission_classes = [IsAuthenticated]

    def get(self, request):
        conversations = Conversation.objects.filter(
            participants__user=request.user
        ).prefetch_related("participants__user")

        serializer = ConversationSerializer(conversations, many=True)
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