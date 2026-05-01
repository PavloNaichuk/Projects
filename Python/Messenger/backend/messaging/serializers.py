from rest_framework import serializers

from accounts.models import User
from .models import Conversation, ConversationParticipant, Message


class UserShortSerializer(serializers.ModelSerializer):
    class Meta:
        model = User
        fields = ("id", "username", "email")


class MessageSerializer(serializers.ModelSerializer):
    sender = UserShortSerializer(read_only=True)
    conversation = serializers.PrimaryKeyRelatedField(read_only=True)

    class Meta:
        model = Message
        fields = ("id", "conversation", "sender", "text", "created_at", "is_read")
        read_only_fields = ("id", "conversation", "sender", "created_at", "is_read")

class ConversationParticipantSerializer(serializers.ModelSerializer):
    user = UserShortSerializer(read_only=True)

    class Meta:
        model = ConversationParticipant
        fields = ("id", "user", "joined_at")


class ConversationSerializer(serializers.ModelSerializer):
    participants = ConversationParticipantSerializer(many=True, read_only=True)

    class Meta:
        model = Conversation
        fields = ("id", "participants", "created_at", "updated_at")