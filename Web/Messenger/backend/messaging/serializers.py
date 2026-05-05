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
        fields = (
            "id",
            "conversation",
            "sender",
            "text",
            "created_at",
            "updated_at",
            "edited_at",
            "is_deleted",
            "deleted_at",
            "is_read",
        )
        read_only_fields = (
            "id",
            "conversation",
            "sender",
            "created_at",
            "updated_at",
            "edited_at",
            "is_deleted",
            "deleted_at",
            "is_read",
        )

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
    last_message = serializers.SerializerMethodField()
    unread_count = serializers.SerializerMethodField()

    class Meta:
        model = Conversation
        fields = (
            "id",
            "participants",
            "last_message",
            "unread_count",
            "created_at",
            "updated_at",
        )

    def get_last_message(self, obj):
        message = obj.messages.select_related("sender").last()

        if not message:
            return None

        return MessageSerializer(message).data

    def get_unread_count(self, obj):
        request = self.context.get("request")

        if not request:
            return 0

        return obj.messages.filter(
            is_read=False,
            is_deleted=False,
        ).exclude(
            sender=request.user
        ).count()

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