from rest_framework import serializers

from .models import User


class UserSearchSerializer(serializers.ModelSerializer):
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


class UserSerializer(serializers.ModelSerializer):
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

class UserProfileSerializer(serializers.ModelSerializer):
    class Meta:
        model = User
        fields = ("id", "username", "email", "avatar_url", "last_seen_at")
        read_only_fields = ("id", "avatar_url", "last_seen_at")

    avatar_url = serializers.SerializerMethodField()

    def get_avatar_url(self, obj):
        if not obj.avatar:
            return None

        request = self.context.get("request")

        if request:
            return request.build_absolute_uri(obj.avatar.url)

        return obj.avatar.url

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
class UserAvatarSerializer(serializers.ModelSerializer):
    avatar_url = serializers.SerializerMethodField()

    class Meta:
        model = User
        fields = (
            "id",
            "username",
            "email",
            "avatar",
            "avatar_url",
            "last_seen_at",
        )
        read_only_fields = (
            "id",
            "username",
            "email",
            "avatar_url",
            "last_seen_at",
        )

    def get_avatar_url(self, obj):
        if not obj.avatar:
            return None

        request = self.context.get("request")

        if request:
            return request.build_absolute_uri(obj.avatar.url)

        return obj.avatar.url

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


class UserRegistrationSerializer(serializers.ModelSerializer):
    avatar_url = serializers.SerializerMethodField()

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
            "email",
            "avatar_url",
            "last_seen_at",
            "password",
            "password_confirm",
        )
        read_only_fields = ("id", "avatar_url", "last_seen_at")

    def get_avatar_url(self, obj):
        if not obj.avatar:
            return None

        request = self.context.get("request")

        if request:
            return request.build_absolute_uri(obj.avatar.url)

        return obj.avatar.url

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
                {"password_confirm": "Passwords do not match."}
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