from rest_framework import serializers

from .models import BlockedUser, ContactNickname, User


def get_user_display_name(user, request):
    if not request or not request.user or not request.user.is_authenticated:
        return user.username

    if request.user.id == user.id:
        return user.username

    nickname = (
        ContactNickname.objects.filter(
            owner=request.user,
            target_user=user,
        )
        .values_list("nickname", flat=True)
        .first()
    )

    return nickname or user.username


def get_is_blocked_by_me(user, request):
    if not request or not request.user or not request.user.is_authenticated:
        return False

    if request.user.id == user.id:
        return False

    return BlockedUser.objects.filter(
        blocker=request.user,
        blocked=user,
    ).exists()


def get_has_blocked_me(user, request):
    if not request or not request.user or not request.user.is_authenticated:
        return False

    if request.user.id == user.id:
        return False

    return BlockedUser.objects.filter(
        blocker=user,
        blocked=request.user,
    ).exists()


class UserRepresentationMixin(serializers.Serializer):
    avatar_url = serializers.SerializerMethodField()
    display_name = serializers.SerializerMethodField()
    is_blocked_by_me = serializers.SerializerMethodField()
    has_blocked_me = serializers.SerializerMethodField()

    def get_display_name(self, obj):
        request = self.context.get("request")
        return get_user_display_name(obj, request)

    def get_avatar_url(self, obj):
        if not obj.avatar:
            return None

        request = self.context.get("request")

        if request:
            return request.build_absolute_uri(obj.avatar.url)

        return obj.avatar.url

    def get_is_blocked_by_me(self, obj):
        request = self.context.get("request")
        return get_is_blocked_by_me(obj, request)

    def get_has_blocked_me(self, obj):
        request = self.context.get("request")
        return get_has_blocked_me(obj, request)


class UserSearchSerializer(UserRepresentationMixin, serializers.ModelSerializer):
    class Meta:
        model = User
        fields = (
            "id",
            "username",
            "display_name",
            "email",
            "avatar_url",
            "last_seen_at",
            "email_verified_at",
            "is_email_verified",
            "is_blocked_by_me",
            "has_blocked_me",
        )


class UserSerializer(UserRepresentationMixin, serializers.ModelSerializer):
    class Meta:
        model = User
        fields = (
            "id",
            "username",
            "display_name",
            "email",
            "avatar_url",
            "last_seen_at",
            "email_verified_at",
            "is_email_verified",
            "is_blocked_by_me",
            "has_blocked_me",
        )


class UserProfileSerializer(UserRepresentationMixin, serializers.ModelSerializer):
    class Meta:
        model = User
        fields = (
            "id",
            "username",
            "display_name",
            "email",
            "avatar_url",
            "last_seen_at",
            "email_verified_at",
            "is_email_verified",
            "is_blocked_by_me",
            "has_blocked_me",
        )
        read_only_fields = (
            "id",
            "display_name",
            "avatar_url",
            "last_seen_at",
            "email_verified_at",
            "is_email_verified",
            "is_blocked_by_me",
            "has_blocked_me",
        )

    def validate_username(self, value):
        value = value.strip()

        if not value:
            raise serializers.ValidationError("Username is required.")

        if User.objects.exclude(id=self.instance.id).filter(username=value).exists():
            raise serializers.ValidationError("User with this username already exists.")

        return value

    def validate_email(self, value):
        value = value.lower().strip()

        if not value:
            raise serializers.ValidationError("Email is required.")

        if User.objects.exclude(id=self.instance.id).filter(email=value).exists():
            raise serializers.ValidationError("User with this email already exists.")

        return value


class UserAvatarSerializer(UserRepresentationMixin, serializers.ModelSerializer):
    class Meta:
        model = User
        fields = (
            "id",
            "username",
            "display_name",
            "email",
            "avatar",
            "avatar_url",
            "last_seen_at",
            "email_verified_at",
            "is_email_verified",
            "is_blocked_by_me",
            "has_blocked_me",
        )
        read_only_fields = (
            "id",
            "username",
            "display_name",
            "email",
            "avatar_url",
            "last_seen_at",
            "email_verified_at",
            "is_email_verified",
            "is_blocked_by_me",
            "has_blocked_me",
        )

    def validate_avatar(self, value):
        allowed_content_types = ("image/jpeg", "image/png", "image/webp")

        if value.content_type not in allowed_content_types:
            raise serializers.ValidationError(
                "Avatar must be a JPG, PNG, or WEBP image."
            )

        max_size = 2 * 1024 * 1024

        if value.size > max_size:
            raise serializers.ValidationError(
                "Avatar file size must be 2 MB or less."
            )

        return value


class UserRegistrationSerializer(UserRepresentationMixin, serializers.ModelSerializer):
    username = serializers.CharField(
        required=True,
        error_messages={
            "required": "Username is required.",
            "blank": "Username is required.",
        },
    )
    email = serializers.EmailField(
        required=True,
        error_messages={
            "required": "Email is required.",
            "blank": "Email is required.",
            "invalid": "Enter a valid email address.",
        },
    )
    password = serializers.CharField(
        write_only=True,
        min_length=8,
        error_messages={
            "required": "Password is required.",
            "blank": "Password is required.",
            "min_length": "Password must be at least 8 characters long.",
        },
    )
    password_confirm = serializers.CharField(
        write_only=True,
        error_messages={
            "required": "Password confirmation is required.",
            "blank": "Password confirmation is required.",
        },
    )

    class Meta:
        model = User
        fields = (
            "id",
            "username",
            "display_name",
            "email",
            "avatar_url",
            "last_seen_at",
            "email_verified_at",
            "is_email_verified",
            "is_blocked_by_me",
            "has_blocked_me",
            "password",
            "password_confirm",
        )
        read_only_fields = (
            "id",
            "display_name",
            "avatar_url",
            "last_seen_at",
            "email_verified_at",
            "is_email_verified",
            "is_blocked_by_me",
            "has_blocked_me",
        )

    def validate_username(self, value):
        value = value.strip()

        if User.objects.filter(username=value).exists():
            raise serializers.ValidationError("User with this username already exists.")

        return value

    def validate_email(self, value):
        value = value.lower().strip()

        if User.objects.filter(email=value).exists():
            raise serializers.ValidationError("User with this email already exists.")

        return value

    def validate(self, attrs):
        if attrs["password"] != attrs["password_confirm"]:
            raise serializers.ValidationError(
                {"password_confirm": "Passwords do not match."}  # nosec B105
            )

        return attrs

    def create(self, validated_data):
        validated_data.pop("password_confirm")

        user = User.objects.create_user(
            username=validated_data["username"],
            email=validated_data["email"],
            password=validated_data["password"],
        )

        return user


class EmailVerificationConfirmSerializer(serializers.Serializer):
    token = serializers.CharField(
        required=True,
        error_messages={
            "required": "Verification token is required.",
            "blank": "Verification token is required.",
        },
    )

class PasswordResetRequestSerializer(serializers.Serializer):
    email = serializers.EmailField(
        required=True,
        error_messages={
            "required": "Email is required.",
            "blank": "Email is required.",
            "invalid": "Enter a valid email address.",
        },
    )

    def validate_email(self, value):
        return value.lower().strip()


class PasswordResetConfirmSerializer(serializers.Serializer):
    uid = serializers.CharField(
        required=True,
        error_messages={
            "required": "Password reset user id is required.",
            "blank": "Password reset user id is required.",
        },
    )
    token = serializers.CharField(
        required=True,
        error_messages={
            "required": "Password reset token is required.",
            "blank": "Password reset token is required.",
        },
    )
    password = serializers.CharField(
        write_only=True,
        min_length=8,
        error_messages={
            "required": "Password is required.",
            "blank": "Password is required.",
            "min_length": "Password must be at least 8 characters long.",
        },
    )
    password_confirm = serializers.CharField(
        write_only=True,
        error_messages={
            "required": "Password confirmation is required.",
            "blank": "Password confirmation is required.",
        },
    )

    def validate(self, attrs):
        if attrs["password"] != attrs["password_confirm"]:
            raise serializers.ValidationError(
                {"password_confirm": "Passwords do not match."}  # nosec B105
            )

        return attrs
