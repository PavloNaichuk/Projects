from django.db.models import Q
from rest_framework import status
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from rest_framework.views import APIView
from rest_framework_simplejwt.tokens import RefreshToken

from .models import User
from .serializers import (
    UserAvatarSerializer,
    UserRegistrationSerializer,
    UserSearchSerializer,
    UserSerializer,
)

from asgiref.sync import async_to_sync
from channels.layers import get_channel_layer

from messaging.models import ConversationParticipant


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


def notify_user_profile_updated(user, request):
    channel_layer = get_channel_layer()

    if not channel_layer:
        return

    serializer = UserSerializer(
        user,
        context={"request": request},
    )

    recipient_ids = get_profile_update_recipient_ids(user)

    for user_id in recipient_ids:
        async_to_sync(channel_layer.group_send)(
            f"user_{user_id}_notifications",
            {
                "type": "user_profile_updated",
                "user": serializer.data,
            },
        )

class UserSearchView(APIView):
    permission_classes = [IsAuthenticated]

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


class UserAvatarView(APIView):
    permission_classes = [IsAuthenticated]

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


class UserRegistrationView(APIView):
    permission_classes = []

    def post(self, request):
        serializer = UserRegistrationSerializer(
            data=request.data,
            context={"request": request},
        )

        if serializer.is_valid():
            user = serializer.save()
            refresh = RefreshToken.for_user(user)

            return Response(
                {
                    "user": UserSerializer(
                        user,
                        context={"request": request},
                    ).data,
                    "refresh": str(refresh),
                    "access": str(refresh.access_token),
                },
                status=status.HTTP_201_CREATED,
            )

        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)


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