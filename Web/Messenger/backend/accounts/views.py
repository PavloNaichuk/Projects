from asgiref.sync import async_to_sync
from channels.layers import get_channel_layer
from django.contrib.auth.tokens import default_token_generator
from django.db import transaction
from django.db.models import Q
from django.utils import timezone
from django.utils.encoding import force_str
from django.utils.http import urlsafe_base64_decode
from rest_framework import status
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from rest_framework.views import APIView
from rest_framework_simplejwt.tokens import RefreshToken
from rest_framework_simplejwt.views import TokenObtainPairView, TokenRefreshView

from messaging.models import ConversationParticipant

from .models import BlockedUser, ContactNickname, EmailVerificationToken, User
from .serializers import (
    EmailVerificationConfirmSerializer,
    PasswordResetConfirmSerializer,
    PasswordResetRequestSerializer,
    UserAvatarSerializer,
    UserProfileSerializer,
    UserRegistrationSerializer,
    UserSearchSerializer,
    UserSerializer,
)
from .tasks import (
    send_email_verification_email_task,
    send_password_reset_email_task,
)


def get_profile_update_recipient_ids(user):
    conversation_ids = ConversationParticipant.objects.filter(
        user=user,
    ).values_list("conversation_id", flat=True)

    return list(
        ConversationParticipant.objects.filter(
            conversation_id__in=conversation_ids,
        )
        .exclude(user=user)
        .values_list("user_id", flat=True)
        .distinct()
    )


def get_user_display_name_for_owner(user, owner):
    if owner.id == user.id:
        return user.username

    nickname = (
        ContactNickname.objects.filter(
            owner=owner,
            target_user=user,
        )
        .values_list("nickname", flat=True)
        .first()
    )

    return nickname or user.username


def serialize_user_for_recipient(user, recipient, request):
    serializer = UserSerializer(
        user,
        context={
            "request": request,
            "viewer": recipient,
        },
    )

    data = dict(serializer.data)
    data["display_name"] = get_user_display_name_for_owner(user, recipient)
    data["is_blocked_by_me"] = BlockedUser.objects.filter(
        blocker=recipient,
        blocked=user,
    ).exists()
    data["has_blocked_me"] = BlockedUser.objects.filter(
        blocker=user,
        blocked=recipient,
    ).exists()

    return data


def notify_user_profile_updated(user, request):
    channel_layer = get_channel_layer()

    if not channel_layer:
        return

    recipient_ids = get_profile_update_recipient_ids(user)

    recipients = User.objects.filter(id__in=recipient_ids)

    for recipient in recipients:
        async_to_sync(channel_layer.group_send)(
            f"user_{recipient.id}_notifications",
            {
                "type": "user_profile_updated",
                "user": serialize_user_for_recipient(
                    user,
                    recipient,
                    request,
                ),
            },
        )


def notify_user_block_status_updated(blocker, blocked, request):
    channel_layer = get_channel_layer()

    if not channel_layer:
        return

    async_to_sync(channel_layer.group_send)(
        f"user_{blocked.id}_notifications",
        {
            "type": "user_profile_updated",
            "user": serialize_user_for_recipient(
                blocker,
                blocked,
                request,
            ),
        },
    )

    async_to_sync(channel_layer.group_send)(
        f"user_{blocker.id}_notifications",
        {
            "type": "user_profile_updated",
            "user": serialize_user_for_recipient(
                blocked,
                blocker,
                request,
            ),
        },
    )


def send_email_verification(user):
    verification_token = EmailVerificationToken.create_for_user(user)

    transaction.on_commit(
        lambda: send_email_verification_email_task.delay(verification_token.id)
    )

    return verification_token


def expire_unused_email_verification_tokens(user):
    EmailVerificationToken.objects.filter(
        user=user,
        used_at__isnull=True,
    ).update(used_at=timezone.now())


def send_password_reset_email(user):
    transaction.on_commit(lambda: send_password_reset_email_task.delay(user.id))


def get_user_from_password_reset_uid(uid):
    try:
        user_id = force_str(urlsafe_base64_decode(uid))
    except (TypeError, ValueError, OverflowError):
        return None

    return User.objects.filter(pk=user_id).first()


class LoginView(TokenObtainPairView):
    permission_classes = []
    throttle_scope = "auth"


class TokenRefreshAPIView(TokenRefreshView):
    permission_classes = []
    throttle_scope = "token_refresh"


class UserSearchView(APIView):
    permission_classes = [IsAuthenticated]
    throttle_scope = "search"

    def get(self, request):
        query = request.query_params.get("q", "").strip()

        users = User.objects.exclude(id=request.user.id)

        if query:
            users = users.filter(
                Q(username__icontains=query) | Q(email__icontains=query)
            )

        serializer = UserSearchSerializer(
            users[:20],
            many=True,
            context={"request": request},
        )
        return Response(serializer.data)


class CurrentUserView(APIView):
    permission_classes = [IsAuthenticated]

    def get(self, request):
        serializer = UserSerializer(
            request.user,
            context={"request": request},
        )
        return Response(serializer.data)


class UserProfileView(APIView):
    permission_classes = [IsAuthenticated]
    throttle_scope = "actions"

    def patch(self, request):
        old_email = request.user.email

        serializer = UserProfileSerializer(
            request.user,
            data=request.data,
            partial=True,
            context={"request": request},
        )

        if serializer.is_valid():
            user = serializer.save()
            email_changed = (
                "email" in serializer.validated_data and user.email != old_email
            )

            if email_changed:
                expire_unused_email_verification_tokens(user)
                user.email_verified_at = None
                user.save(update_fields=["email_verified_at"])
                send_email_verification(user)

            notify_user_profile_updated(user, request)

            response_serializer = UserProfileSerializer(
                user,
                context={"request": request},
            )
            return Response(response_serializer.data)

        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)


class UserAvatarView(APIView):
    permission_classes = [IsAuthenticated]
    throttle_scope = "uploads"

    def patch(self, request):
        if "avatar" not in request.FILES:
            return Response(
                {"avatar": ["Avatar file is required."]},
                status=status.HTTP_400_BAD_REQUEST,
            )

        serializer = UserAvatarSerializer(
            request.user,
            data=request.data,
            partial=True,
            context={"request": request},
        )

        if serializer.is_valid():
            if request.user.avatar:
                request.user.avatar.delete(save=False)

            serializer.save()
            notify_user_profile_updated(request.user, request)
            return Response(serializer.data)

        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)

    def delete(self, request):
        if request.user.avatar:
            request.user.avatar.delete(save=False)
            request.user.avatar = None
            request.user.save(update_fields=["avatar"])

        notify_user_profile_updated(request.user, request)

        serializer = UserSerializer(
            request.user,
            context={"request": request},
        )
        return Response(serializer.data)


class ContactNicknameView(APIView):
    permission_classes = [IsAuthenticated]

    def patch(self, request, user_id):
        target_user = User.objects.filter(id=user_id).first()

        if not target_user:
            return Response(
                {"detail": "User not found."},
                status=status.HTTP_404_NOT_FOUND,
            )

        if target_user.id == request.user.id:
            return Response(
                {"detail": "You cannot set a nickname for yourself."},
                status=status.HTTP_400_BAD_REQUEST,
            )

        nickname = str(request.data.get("nickname", "")).strip()

        if len(nickname) > 50:
            return Response(
                {"nickname": ["Nickname must be 50 characters or less."]},
                status=status.HTTP_400_BAD_REQUEST,
            )

        if not nickname:
            ContactNickname.objects.filter(
                owner=request.user,
                target_user=target_user,
            ).delete()

            serializer = UserSerializer(
                target_user,
                context={"request": request},
            )

            return Response(
                {
                    "detail": "Contact nickname removed.",
                    "user": serializer.data,
                },
                status=status.HTTP_200_OK,
            )

        ContactNickname.objects.update_or_create(
            owner=request.user,
            target_user=target_user,
            defaults={"nickname": nickname},
        )

        serializer = UserSerializer(
            target_user,
            context={"request": request},
        )

        return Response(
            {
                "detail": "Contact nickname updated.",
                "user": serializer.data,
            },
            status=status.HTTP_200_OK,
        )


class UserBlockedListView(APIView):
    permission_classes = [IsAuthenticated]

    def get(self, request):
        blocked_users = User.objects.filter(
            blocked_by_users__blocker=request.user,
        ).order_by("username")

        serializer = UserSerializer(
            blocked_users,
            many=True,
            context={"request": request},
        )

        return Response(serializer.data)


class UserBlockView(APIView):
    permission_classes = [IsAuthenticated]

    def post(self, request, user_id):
        target_user = User.objects.filter(id=user_id).first()

        if not target_user:
            return Response(
                {"detail": "User not found."},
                status=status.HTTP_404_NOT_FOUND,
            )

        if target_user.id == request.user.id:
            return Response(
                {"detail": "You cannot block yourself."},
                status=status.HTTP_400_BAD_REQUEST,
            )

        BlockedUser.objects.get_or_create(
            blocker=request.user,
            blocked=target_user,
        )

        notify_user_block_status_updated(request.user, target_user, request)

        serializer = UserSerializer(
            target_user,
            context={"request": request},
        )

        return Response(
            {
                "detail": "User blocked.",
                "user": serializer.data,
            },
            status=status.HTTP_200_OK,
        )


class UserUnblockView(APIView):
    permission_classes = [IsAuthenticated]

    def post(self, request, user_id):
        target_user = User.objects.filter(id=user_id).first()

        if not target_user:
            return Response(
                {"detail": "User not found."},
                status=status.HTTP_404_NOT_FOUND,
            )

        if target_user.id == request.user.id:
            return Response(
                {"detail": "You cannot unblock yourself."},
                status=status.HTTP_400_BAD_REQUEST,
            )

        BlockedUser.objects.filter(
            blocker=request.user,
            blocked=target_user,
        ).delete()

        notify_user_block_status_updated(request.user, target_user, request)

        serializer = UserSerializer(
            target_user,
            context={"request": request},
        )

        return Response(
            {
                "detail": "User unblocked.",
                "user": serializer.data,
            },
            status=status.HTTP_200_OK,
        )


class UserRegistrationView(APIView):
    permission_classes = []
    throttle_scope = "auth"

    def post(self, request):
        serializer = UserRegistrationSerializer(
            data=request.data,
            context={"request": request},
        )

        if serializer.is_valid():
            user = serializer.save()
            send_email_verification(user)
            refresh = RefreshToken.for_user(user)

            return Response(
                {
                    "user": UserSerializer(
                        user,
                        context={
                            "request": request,
                            "include_private_fields": True,
                        },
                    ).data,
                    "refresh": str(refresh),
                    "access": str(refresh.access_token),
                },
                status=status.HTTP_201_CREATED,
            )

        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)


class EmailVerificationConfirmView(APIView):
    permission_classes = []
    throttle_scope = "auth"

    def post(self, request):
        serializer = EmailVerificationConfirmSerializer(data=request.data)

        if not serializer.is_valid():
            return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)

        verification_token = (
            EmailVerificationToken.objects.select_related("user")
            .filter(token=serializer.validated_data["token"])
            .first()
        )

        if not verification_token:
            return Response(
                {"token": ["Invalid verification token."]},
                status=status.HTTP_400_BAD_REQUEST,
            )

        if verification_token.is_used:
            return Response(
                {"token": ["Verification token has already been used."]},
                status=status.HTTP_400_BAD_REQUEST,
            )

        if verification_token.is_expired:
            return Response(
                {"token": ["Verification token has expired."]},
                status=status.HTTP_400_BAD_REQUEST,
            )

        user = verification_token.user
        user.mark_email_verified()
        verification_token.mark_used()

        serializer = UserSerializer(
            user,
            context={
                "request": request,
                "include_private_fields": True,
            },
        )

        return Response(
            {
                "detail": "Email verified successfully.",
                "user": serializer.data,
            },
            status=status.HTTP_200_OK,
        )


class EmailVerificationResendView(APIView):
    permission_classes = [IsAuthenticated]
    throttle_scope = "auth"

    def post(self, request):
        if request.user.is_email_verified:
            return Response(
                {"detail": "Email is already verified."},
                status=status.HTTP_400_BAD_REQUEST,
            )

        expire_unused_email_verification_tokens(request.user)
        send_email_verification(request.user)

        return Response(
            {"detail": "Verification email sent."},
            status=status.HTTP_200_OK,
        )


class PasswordResetRequestView(APIView):
    permission_classes = []
    throttle_scope = "auth"

    def post(self, request):
        serializer = PasswordResetRequestSerializer(data=request.data)

        if not serializer.is_valid():
            return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)

        user = User.objects.filter(
            email=serializer.validated_data["email"],
        ).first()

        if user:
            send_password_reset_email(user)

        return Response(
            {
                "detail": (
                    "If an account with this email exists, "
                    "a password reset email was sent."
                ),
            },
            status=status.HTTP_200_OK,
        )


class PasswordResetConfirmView(APIView):
    permission_classes = []
    throttle_scope = "auth"

    def post(self, request):
        serializer = PasswordResetConfirmSerializer(data=request.data)

        if not serializer.is_valid():
            return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)

        user = get_user_from_password_reset_uid(serializer.validated_data["uid"])

        if not user or not default_token_generator.check_token(
            user,
            serializer.validated_data["token"],
        ):
            return Response(
                {"token": ["Invalid or expired password reset token."]},
                status=status.HTTP_400_BAD_REQUEST,
            )

        user.set_password(serializer.validated_data["password"])
        user.save(update_fields=["password"])

        return Response(
            {"detail": "Password reset successfully."},
            status=status.HTTP_200_OK,
        )


class LogoutView(APIView):
    permission_classes = [IsAuthenticated]

    def post(self, request):
        refresh_token = request.data.get("refresh")

        if not refresh_token:
            return Response(
                {"refresh": ["Refresh token is required."]},
                status=status.HTTP_400_BAD_REQUEST,
            )

        try:
            token = RefreshToken(refresh_token)
            token.blacklist()
        except Exception:
            return Response(
                {"refresh": ["Invalid refresh token."]},
                status=status.HTTP_400_BAD_REQUEST,
            )

        return Response(
            {"detail": "Successfully logged out."},
            status=status.HTTP_200_OK,
        )
