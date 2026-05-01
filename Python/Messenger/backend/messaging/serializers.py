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
    text = serializers.CharField(
        trim_whitespace=True,
        error_messages={
            "blank": "Message text cannot be empty.",
            "required": "Message text is required.",
        },
    )

    class Meta:
        model = Message
        fields = ("id", "conversation", "sender", "text", "created_at", "is_read")
        read_only_fields = ("id", "conversation", "sender", "created_at", "is_read")

    def validate_text(self, value):
        value = value.strip()

        if not value:
            raise serializers.ValidationError("Message text cannot be empty.")

        return value
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
        

class ConversationCreateSerializer(serializers.Serializer):
    user_id = serializers.IntegerField()

    def validate_user_id(self, value):
        request = self.context.get("request")

        if request and value == request.user.id:
            raise serializers.ValidationError(
                "You cannot create a conversation with yourself."
            )

        if not User.objects.filter(id=value).exists():
            raise serializers.ValidationError("User not found.")

        return value