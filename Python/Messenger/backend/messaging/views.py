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