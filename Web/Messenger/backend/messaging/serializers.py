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
        required=False,
        allow_blank=True,
    )

    attachment = serializers.FileField(
        required=False,
        write_only=True,
        allow_empty_file=False,
    )
    attachment_url = serializers.SerializerMethodField()
    attachment_is_image = serializers.SerializerMethodField()

    class Meta:
        model = Message
        fields = (
            "id",
            "conversation",
            "sender",
            "text",
            "attachment",
            "attachment_url",
            "attachment_name",
            "attachment_content_type",
            "attachment_size",
            "attachment_is_image",
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
            "attachment_url",
            "attachment_name",
            "attachment_content_type",
            "attachment_size",
            "attachment_is_image",
            "created_at",
            "updated_at",
            "edited_at",
            "is_deleted",
            "deleted_at",
            "is_read",
        )

    def get_attachment_url(self, obj):
        if not obj.attachment:
            return None

        request = self.context.get("request")

        if request:
            return request.build_absolute_uri(obj.attachment.url)

        return obj.attachment.url

    def get_attachment_is_image(self, obj):
        if not obj.attachment_content_type:
            return False

        return obj.attachment_content_type.startswith("image/")

    def validate(self, attrs):
        text = attrs.get("text", "")
        attachment = attrs.get("attachment")

        if "text" in attrs:
            attrs["text"] = text.strip()

        # Editing existing message
        if self.instance:
            if "text" in attrs and not attrs["text"]:
                raise serializers.ValidationError(
                    {"text": ["Message text cannot be empty."]}
                )

            return attrs

        # Creating new message
        if not attrs.get("text") and not attachment:
            raise serializers.ValidationError(
                {"text": ["Message text cannot be empty."]}
            )

        return attrs


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
        message = (
            obj.messages
            .select_related("sender")
            .order_by("-created_at", "-id")
            .first()
        )

        if not message:
            return None

        return MessageSerializer(
            message,
            context=self.context,
        ).data

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