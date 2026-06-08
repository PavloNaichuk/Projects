from django.contrib.auth import get_user_model
from django.core.files.uploadedfile import SimpleUploadedFile
from django.test import TestCase, override_settings
from django.urls import reverse
from rest_framework import serializers, status
from rest_framework.test import APIClient, APIRequestFactory
from rest_framework_simplejwt.tokens import RefreshToken

from .models import BlockedUser, ContactNickname
from .serializers import (
    UserAvatarSerializer,
    UserProfileSerializer,
    UserRegistrationSerializer,
    UserSearchSerializer,
    UserSerializer,
)

TEST_CHANNEL_LAYERS = {
    "default": {
        "BACKEND": "channels.layers.InMemoryChannelLayer",
    },
}
@override_settings(CHANNEL_LAYERS=TEST_CHANNEL_LAYERS)
class AccountsAPITests(TestCase):
    def setUp(self):
        User = get_user_model()

        self.user = User.objects.create_user(
            username="pavlo",
            email="pavlo@test.ua",
            password="testpassword123",
        )

        self.client = APIClient()

    def test_user_can_register_and_receive_tokens(self):
        url = reverse("user-register")

        response = self.client.post(
            url,
            {
                "username": "newuser",
                "email": "newuser@test.ua",
                "password": "testpassword123",
                "password_confirm": "testpassword123",
            },
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_201_CREATED)
        self.assertEqual(response.data["user"]["username"], "newuser")
        self.assertEqual(response.data["user"]["email"], "newuser@test.ua")
        self.assertIn("access", response.data)
        self.assertIn("refresh", response.data)

    def test_register_requires_matching_passwords(self):
        url = reverse("user-register")

        response = self.client.post(
            url,
            {
                "username": "newuser",
                "email": "newuser@test.ua",
                "password": "testpassword123",
                "password_confirm": "differentpassword123",
            },
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            response.data["password_confirm"][0],
            "Passwords do not match.",
        )

    def test_register_rejects_duplicate_username(self):
        url = reverse("user-register")

        response = self.client.post(
            url,
            {
                "username": "pavlo",
                "email": "newemail@test.ua",
                "password": "testpassword123",
                "password_confirm": "testpassword123",
            },
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            response.data["username"][0],
            "User with this username already exists.",
        )

    def test_register_rejects_duplicate_email(self):
        url = reverse("user-register")

        response = self.client.post(
            url,
            {
                "username": "newuser",
                "email": "pavlo@test.ua",
                "password": "testpassword123",
                "password_confirm": "testpassword123",
            },
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            response.data["email"][0],
            "User with this email already exists.",
        )

    def test_authenticated_user_can_get_me(self):
        self.client.force_authenticate(user=self.user)

        url = reverse("current-user")
        response = self.client.get(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["id"], self.user.id)
        self.assertEqual(response.data["username"], "pavlo")
        self.assertEqual(response.data["email"], "pavlo@test.ua")

    def test_authenticated_user_can_search_users(self):
        User = get_user_model()

        other_user = User.objects.create_user(
            username="user1",
            email="user1@test.ua",
            password="testpassword123",
        )

        self.client.force_authenticate(user=self.user)

        url = reverse("user-search")
        response = self.client.get(url, {"q": "user"})

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(len(response.data), 1)
        self.assertEqual(response.data[0]["id"], other_user.id)
        self.assertEqual(response.data[0]["username"], "user1")

    def test_search_does_not_return_current_user(self):
        self.client.force_authenticate(user=self.user)

        url = reverse("user-search")
        response = self.client.get(url, {"q": "pavlo"})

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(len(response.data), 0)

    def test_authenticated_user_can_logout(self):
        self.client.force_authenticate(user=self.user)

        refresh = RefreshToken.for_user(self.user)

        url = reverse("logout")
        response = self.client.post(
            url,
            {"refresh": str(refresh)},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["detail"], "Successfully logged out.")

    def test_logout_requires_refresh_token(self):
        self.client.force_authenticate(user=self.user)

        url = reverse("logout")
        response = self.client.post(url, {}, format="json")

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            response.data["refresh"][0],
            "Refresh token is required.",
        )

    def test_authenticated_user_can_block_user(self):
        User = get_user_model()

        other_user = User.objects.create_user(
            username="user1",
            email="user1@test.ua",
            password="testpassword123",
        )

        self.client.force_authenticate(user=self.user)

        url = reverse("user-block", kwargs={"user_id": other_user.id})
        response = self.client.post(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["detail"], "User blocked.")
        self.assertEqual(response.data["user"]["id"], other_user.id)
        self.assertTrue(response.data["user"]["is_blocked_by_me"])

        self.assertTrue(
            BlockedUser.objects.filter(
                blocker=self.user,
                blocked=other_user,
            ).exists()
        )

    def test_user_cannot_block_self(self):
        self.client.force_authenticate(user=self.user)

        url = reverse("user-block", kwargs={"user_id": self.user.id})
        response = self.client.post(url)

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(response.data["detail"], "You cannot block yourself.")

        self.assertFalse(
            BlockedUser.objects.filter(
                blocker=self.user,
                blocked=self.user,
            ).exists()
        )

    def test_block_missing_user_returns_404(self):
        self.client.force_authenticate(user=self.user)

        url = reverse("user-block", kwargs={"user_id": 999})
        response = self.client.post(url)

        self.assertEqual(response.status_code, status.HTTP_404_NOT_FOUND)
        self.assertEqual(response.data["detail"], "User not found.")

    def test_authenticated_user_can_unblock_user(self):
        User = get_user_model()

        other_user = User.objects.create_user(
            username="user1",
            email="user1@test.ua",
            password="testpassword123",
        )

        BlockedUser.objects.create(
            blocker=self.user,
            blocked=other_user,
        )

        self.client.force_authenticate(user=self.user)

        url = reverse("user-unblock", kwargs={"user_id": other_user.id})
        response = self.client.post(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["detail"], "User unblocked.")
        self.assertEqual(response.data["user"]["id"], other_user.id)
        self.assertFalse(response.data["user"]["is_blocked_by_me"])

        self.assertFalse(
            BlockedUser.objects.filter(
                blocker=self.user,
                blocked=other_user,
            ).exists()
        )

    def test_user_cannot_unblock_self(self):
        self.client.force_authenticate(user=self.user)

        url = reverse("user-unblock", kwargs={"user_id": self.user.id})
        response = self.client.post(url)

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(response.data["detail"], "You cannot unblock yourself.")

    def test_unblock_missing_user_returns_404(self):
        self.client.force_authenticate(user=self.user)

        url = reverse("user-unblock", kwargs={"user_id": 999})
        response = self.client.post(url)

        self.assertEqual(response.status_code, status.HTTP_404_NOT_FOUND)
        self.assertEqual(response.data["detail"], "User not found.")

    def test_authenticated_user_can_get_blocked_users_list(self):
        User = get_user_model()

        blocked_user = User.objects.create_user(
            username="blocked",
            email="blocked@test.ua",
            password="testpassword123",
        )
        another_blocked_user = User.objects.create_user(
            username="anotherblocked",
            email="anotherblocked@test.ua",
            password="testpassword123",
        )
        not_blocked_user = User.objects.create_user(
            username="notblocked",
            email="notblocked@test.ua",
            password="testpassword123",
        )

        BlockedUser.objects.create(
            blocker=self.user,
            blocked=blocked_user,
        )
        BlockedUser.objects.create(
            blocker=self.user,
            blocked=another_blocked_user,
        )
        BlockedUser.objects.create(
            blocker=not_blocked_user,
            blocked=self.user,
        )

        self.client.force_authenticate(user=self.user)

        url = reverse("blocked-users")
        response = self.client.get(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)

        blocked_user_ids = [user["id"] for user in response.data]

        self.assertIn(blocked_user.id, blocked_user_ids)
        self.assertIn(another_blocked_user.id, blocked_user_ids)
        self.assertNotIn(not_blocked_user.id, blocked_user_ids)
        self.assertNotIn(self.user.id, blocked_user_ids)

    def get_serializer_request(self, user):
        factory = APIRequestFactory()
        request = factory.get("/")
        request.user = user
        return request

    def test_user_serializer_uses_contact_nickname_for_other_user(self):
        User = get_user_model()

        other_user = User.objects.create_user(
            username="user1",
            email="user1@test.ua",
            password="testpassword123",
        )

        ContactNickname.objects.create(
            owner=self.user,
            target_user=other_user,
            nickname="Best Friend",
        )

        request = self.get_serializer_request(self.user)

        serializer = UserSerializer(
            other_user,
            context={"request": request},
        )

        self.assertEqual(serializer.data["display_name"], "Best Friend")

    def test_user_serializer_uses_username_for_current_user(self):
        request = self.get_serializer_request(self.user)

        serializer = UserSerializer(
            self.user,
            context={"request": request},
        )

        self.assertEqual(serializer.data["display_name"], "pavlo")

    def test_user_serializer_returns_block_flags(self):
        User = get_user_model()

        blocked_by_me = User.objects.create_user(
            username="blockedbyme",
            email="blockedbyme@test.ua",
            password="testpassword123",
        )
        blocked_me = User.objects.create_user(
            username="blockedme",
            email="blockedme@test.ua",
            password="testpassword123",
        )

        BlockedUser.objects.create(
            blocker=self.user,
            blocked=blocked_by_me,
        )
        BlockedUser.objects.create(
            blocker=blocked_me,
            blocked=self.user,
        )

        request = self.get_serializer_request(self.user)

        blocked_by_me_serializer = UserSerializer(
            blocked_by_me,
            context={"request": request},
        )
        blocked_me_serializer = UserSerializer(
            blocked_me,
            context={"request": request},
        )

        self.assertTrue(blocked_by_me_serializer.data["is_blocked_by_me"])
        self.assertFalse(blocked_by_me_serializer.data["has_blocked_me"])
        self.assertFalse(blocked_me_serializer.data["is_blocked_by_me"])
        self.assertTrue(blocked_me_serializer.data["has_blocked_me"])

    def test_user_search_serializer_returns_avatar_url(self):
        avatar = SimpleUploadedFile(
            "avatar.png",
            b"avatar content",
            content_type="image/png",
        )
        self.user.avatar.save("avatar.png", avatar, save=True)

        request = self.get_serializer_request(self.user)

        serializer = UserSearchSerializer(
            self.user,
            context={"request": request},
        )

        self.assertIn("/media/", serializer.data["avatar_url"])
        self.assertIn("avatar", serializer.data["avatar_url"])

    def test_profile_serializer_normalizes_valid_username_and_email(self):
        serializer = UserProfileSerializer(
            self.user,
            data={
                "username": "  pavlo_new  ",
                "email": "  PAVLO_NEW@TEST.UA  ",
            },
            partial=True,
        )

        self.assertTrue(serializer.is_valid(), serializer.errors)
        self.assertEqual(serializer.validated_data["username"], "pavlo_new")
        self.assertEqual(serializer.validated_data["email"], "pavlo_new@test.ua")

    def test_profile_serializer_rejects_blank_username_and_email(self):
        serializer = UserProfileSerializer(
            self.user,
            data={
                "username": "   ",
                "email": "   ",
            },
            partial=True,
        )

        self.assertFalse(serializer.is_valid())
        self.assertEqual(
            serializer.errors["username"][0],
            "This field may not be blank.",
        )
        self.assertEqual(
            serializer.errors["email"][0],
            "This field may not be blank.",
        )

    def test_profile_serializer_rejects_duplicate_username_and_email(self):
        User = get_user_model()

        User.objects.create_user(
            username="taken",
            email="taken@test.ua",
            password="testpassword123",
        )

        serializer = UserProfileSerializer(
            self.user,
            data={
                "username": "taken",
                "email": "taken@test.ua",
            },
            partial=True,
        )

        self.assertFalse(serializer.is_valid())
        self.assertEqual(
            serializer.errors["username"][0],
            "A user with that username already exists.",
        )
        self.assertEqual(
            serializer.errors["email"][0],
            "user with this email already exists.",
        )

    def test_avatar_serializer_accepts_valid_avatar(self):
        valid_png = (
            b"\x89PNG\r\n\x1a\n"
            b"\x00\x00\x00\rIHDR"
            b"\x00\x00\x00\x01\x00\x00\x00\x01"
            b"\x08\x06\x00\x00\x00"
            b"\x1f\x15\xc4\x89"
            b"\x00\x00\x00\nIDAT"
            b"x\x9cc\x00\x01\x00\x00\x05\x00\x01"
            b"\r\n-\xb4"
            b"\x00\x00\x00\x00IEND\xaeB`\x82"
        )

        avatar = SimpleUploadedFile(
            "avatar.png",
            valid_png,
            content_type="image/png",
        )

        serializer = UserAvatarSerializer(
            self.user,
            data={"avatar": avatar},
            partial=True,
        )

        self.assertTrue(serializer.is_valid(), serializer.errors)

    def test_avatar_serializer_rejects_unsupported_avatar_type(self):
        avatar = SimpleUploadedFile(
            "avatar.gif",
            b"avatar content",
            content_type="image/gif",
        )

        serializer = UserAvatarSerializer()

        with self.assertRaisesMessage(
            serializers.ValidationError,
            "Avatar must be a JPG, PNG, or WEBP image.",
        ):
            serializer.validate_avatar(avatar)

    def test_avatar_serializer_rejects_too_large_avatar(self):
        valid_png = (
            b"\x89PNG\r\n\x1a\n"
            b"\x00\x00\x00\rIHDR"
            b"\x00\x00\x00\x01\x00\x00\x00\x01"
            b"\x08\x06\x00\x00\x00"
            b"\x1f\x15\xc4\x89"
            b"\x00\x00\x00\nIDAT"
            b"x\x9cc\x00\x01\x00\x00\x05\x00\x01"
            b"\r\n-\xb4"
            b"\x00\x00\x00\x00IEND\xaeB`\x82"
        )

        avatar = SimpleUploadedFile(
            "avatar.png",
            valid_png + b"x" * (2 * 1024 * 1024 + 1),
            content_type="image/png",
        )

        serializer = UserAvatarSerializer(
            self.user,
            data={"avatar": avatar},
            partial=True,
        )

        self.assertFalse(serializer.is_valid())
        self.assertEqual(
            serializer.errors["avatar"][0],
            "Avatar file size must be 2 MB or less.",
        )

    def test_registration_serializer_creates_user_with_normalized_email(self):
        serializer = UserRegistrationSerializer(
            data={
                "username": "newuser",
                "email": "  NEWUSER@TEST.UA  ",
                "password": "testpassword123",
                "password_confirm": "testpassword123",
            }
        )

        self.assertTrue(serializer.is_valid(), serializer.errors)

        user = serializer.save()

        self.assertEqual(user.username, "newuser")
        self.assertEqual(user.email, "newuser@test.ua")
        self.assertTrue(user.check_password("testpassword123"))
