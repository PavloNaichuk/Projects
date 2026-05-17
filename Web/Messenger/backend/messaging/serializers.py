from rest_framework import serializers

from accounts.models import User
from .models import Conversation, ConversationParticipant, Message, MessageReaction


class UserShortSerializer(serializers.ModelSerializer):
    avatar_url = serializers.SerializerMethodField()

    class Meta:
        model = User
        fields = ("id", "username", "email", "avatar_url", "last_seen_at")

    def get_avatar_url(self, obj):
        if not obj.avatar:
            return None

        request = self.context.get("request")

        if request:
            return request.build_absolute_uri(obj.avatar.url)

        return obj.avatar.url


class MessageReactionSerializer(serializers.ModelSerializer):
    user = UserShortSerializer(read_only=True)

    class Meta:
        model = MessageReaction
        fields = (
            "id",
            "user",
            "emoji",
            "created_at",
        )


class MessageReplySerializer(serializers.ModelSerializer):
    sender = UserShortSerializer(read_only=True)
    attachment_is_image = serializers.SerializerMethodField()

    class Meta:
        model = Message
        fields = (
            "id",
            "sender",
            "text",
            "attachment_name",
            "attachment_content_type",
            "attachment_is_image",
            "is_deleted",
            "created_at",
        )

    def get_attachment_is_image(self, obj):
        if not obj.attachment_content_type:
            return False

        return obj.attachment_content_type.startswith("image/")


class MessageForwardedSerializer(serializers.ModelSerializer):
    sender = UserShortSerializer(read_only=True)

    class Meta:
        model = Message
        fields = (
            "id",
            "sender",
            "is_deleted",
            "created_at",
        )


class MessageSerializer(serializers.ModelSerializer):
    sender = UserShortSerializer(read_only=True)
    conversation = serializers.PrimaryKeyRelatedField(read_only=True)

    reply_to = serializers.PrimaryKeyRelatedField(
        queryset=Message.objects.all(),
        required=False,
        allow_null=True,
        write_only=True,
    )
    reply_to_message = MessageReplySerializer(
        source="reply_to",
        read_only=True,
    )
    forwarded_from_message = MessageForwardedSerializer(
        source="forwarded_from",
        read_only=True,
    )

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

    reactions = serializers.SerializerMethodField()

    class Meta:
        model = Message
        fields = (
            "id",
            "conversation",
            "sender",
            "reply_to",
            "reply_to_message",
            "forwarded_from_message",
            "text",
            "attachment",
            "attachment_url",
            "attachment_name",
            "attachment_content_type",
            "attachment_size",
            "attachment_is_image",
            "reactions",
            "created_at",
            "updated_at",
            "edited_at",
            "is_deleted",
            "deleted_at",
            "is_delivered",
            "delivered_at",
            "is_read",
        )
        read_only_fields = (
            "id",
            "conversation",
            "sender",
            "reply_to_message",
            "forwarded_from_message",
            "attachment_url",
            "attachment_name",
            "attachment_content_type",
            "attachment_size",
            "attachment_is_image",
            "reactions",
            "created_at",
            "updated_at",
            "edited_at",
            "is_deleted",
            "deleted_at",
            "is_delivered",
            "delivered_at",
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

    def get_reactions(self, obj):
        request = self.context.get("request")
        current_user_id = None

        if request and request.user and request.user.is_authenticated:
            current_user_id = request.user.id

        reactions_by_emoji = {}

        for reaction in obj.reactions.select_related("user").all():
            if reaction.emoji not in reactions_by_emoji:
                reactions_by_emoji[reaction.emoji] = {
                    "emoji": reaction.emoji,
                    "count": 0,
                    "reacted_by_me": False,
                    "users": [],
                }

            reactions_by_emoji[reaction.emoji]["count"] += 1
            reactions_by_emoji[reaction.emoji]["users"].append(
                UserShortSerializer(
                    reaction.user,
                    context=self.context,
                ).data
            )

            if current_user_id and reaction.user_id == current_user_id:
                reactions_by_emoji[reaction.emoji]["reacted_by_me"] = True

        return list(reactions_by_emoji.values())

    def validate(self, attrs):
        text = attrs.get("text", "")
        attachment = attrs.get("attachment")

        if not attachment and hasattr(self, "initial_data"):
            attachment = self.initial_data.get("attachment")

        reply_to = attrs.get("reply_to")
        request = self.context.get("request")
        conversation = self.context.get("conversation")

        if "text" in attrs:
            attrs["text"] = text.strip()

        if reply_to:
            if conversation and reply_to.conversation_id != conversation.id:
                raise serializers.ValidationError(
                    {"reply_to": ["Reply message must be from the same conversation."]}
                )

            if request and not Message.objects.filter(
                id=reply_to.id,
                conversation__participants__user=request.user,
            ).exists():
                raise serializers.ValidationError(
                    {"reply_to": ["Reply message not found."]}
                )

        if self.instance:
            if "text" in attrs and not attrs["text"]:
                raise serializers.ValidationError(
                    {"text": ["Message text cannot be empty."]}
                )

            return attrs

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
    is_muted = serializers.SerializerMethodField()

    class Meta:
        model = Conversation
        fields = (
            "id",
            "participants",
            "last_message",
            "unread_count",
            "is_muted",
            "created_at",
            "updated_at",
        )

    def get_last_message(self, obj):
        message = (
            obj.messages
            .select_related(
                "sender",
                "reply_to",
                "reply_to__sender",
                "forwarded_from",
                "forwarded_from__sender",
            )
            .prefetch_related("reactions__user")
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
        
    def get_is_muted(self, obj):
        request = self.context.get("request")

        if not request or not request.user or not request.user.is_authenticated:
            return False

        return obj.muted_for.filter(id=request.user.id).exists()


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