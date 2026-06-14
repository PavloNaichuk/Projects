from django.contrib.auth import get_user_model
from django.contrib.auth.tokens import default_token_generator
from django.core import mail
from django.core.files.uploadedfile import SimpleUploadedFile
from django.test import TestCase, override_settings
from django.urls import reverse
from django.utils import timezone
from django.utils.encoding import force_bytes
from django.utils.http import urlsafe_base64_encode
from rest_framework import serializers, status
from rest_framework.test import APIClient, APIRequestFactory
from rest_framework_simplejwt.tokens import RefreshToken

from messaging.models import Conversation

from .models import BlockedUser, ContactNickname, EmailVerificationToken
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


@override_settings(
    CHANNEL_LAYERS=TEST_CHANNEL_LAYERS,
    CELERY_TASK_ALWAYS_EAGER=True,
    CELERY_TASK_EAGER_PROPAGATES=True,
)
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

    def get_valid_png_file(self, name="avatar.png"):
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

        return SimpleUploadedFile(
            name,
            valid_png,
            content_type="image/png",
        )

    def create_other_user(self, username="user1", email="user1@test.ua"):
        User = get_user_model()

        return User.objects.create_user(
            username=username,
            email=email,
            password="testpassword123",
        )

    def test_authenticated_user_can_update_profile(self):
        self.client.force_authenticate(user=self.user)

        url = reverse("user-profile")
        response = self.client.patch(
            url,
            {
                "username": "pavlo_new",
                "email": "pavlo_new@test.ua",
            },
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["username"], "pavlo_new")
        self.assertEqual(response.data["email"], "pavlo_new@test.ua")

        self.user.refresh_from_db()

        self.assertEqual(self.user.username, "pavlo_new")
        self.assertEqual(self.user.email, "pavlo_new@test.ua")

    @override_settings(
        EMAIL_BACKEND="django.core.mail.backends.locmem.EmailBackend",
        FRONTEND_URL="http://frontend.test",
    )
    def test_update_profile_resets_email_verification_when_email_changes(self):
        self.user.mark_email_verified()
        old_token = EmailVerificationToken.create_for_user(self.user)

        self.client.force_authenticate(user=self.user)

        url = reverse("user-profile")

        with self.captureOnCommitCallbacks(execute=True):
            response = self.client.patch(
                url,
                {
                    "username": "pavlo",
                    "email": "pavlo_new@test.ua",
                },
                format="json",
            )

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["email"], "pavlo_new@test.ua")
        self.assertFalse(response.data["is_email_verified"])
        self.assertIsNone(response.data["email_verified_at"])

        self.user.refresh_from_db()
        old_token.refresh_from_db()

        self.assertEqual(self.user.email, "pavlo_new@test.ua")
        self.assertFalse(self.user.is_email_verified)
        self.assertIsNone(self.user.email_verified_at)
        self.assertTrue(old_token.is_used)

        new_token = (
            EmailVerificationToken.objects.filter(
                user=self.user,
                used_at__isnull=True,
            )
            .exclude(id=old_token.id)
            .latest("created_at")
        )

        self.assertEqual(len(mail.outbox), 1)
        self.assertEqual(mail.outbox[0].to, ["pavlo_new@test.ua"])
        self.assertIn(new_token.token, mail.outbox[0].body)
        self.assertIn("http://frontend.test/verify-email", mail.outbox[0].body)

    @override_settings(EMAIL_BACKEND="django.core.mail.backends.locmem.EmailBackend")
    def test_update_profile_keeps_verification_when_email_is_unchanged(self):
        self.user.mark_email_verified()
        verified_at = self.user.email_verified_at

        self.client.force_authenticate(user=self.user)

        url = reverse("user-profile")
        response = self.client.patch(
            url,
            {
                "username": "pavlo_new",
                "email": "  PAVLO@TEST.UA  ",
            },
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["email"], "pavlo@test.ua")
        self.assertTrue(response.data["is_email_verified"])

        self.user.refresh_from_db()

        self.assertEqual(self.user.username, "pavlo_new")
        self.assertEqual(self.user.email, "pavlo@test.ua")
        self.assertTrue(self.user.is_email_verified)
        self.assertEqual(self.user.email_verified_at, verified_at)
        self.assertEqual(len(mail.outbox), 0)

    def test_update_profile_returns_validation_errors(self):
        other_user = self.create_other_user(
            username="taken",
            email="taken@test.ua",
        )

        self.client.force_authenticate(user=self.user)

        url = reverse("user-profile")
        response = self.client.patch(
            url,
            {
                "username": other_user.username,
                "email": other_user.email,
            },
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertIn("username", response.data)
        self.assertIn("email", response.data)

    def test_update_profile_notifies_conversation_participants(self):
        other_user = self.create_other_user()

        conversation = Conversation.objects.create()
        conversation.participants.create(user=self.user)
        conversation.participants.create(user=other_user)

        self.client.force_authenticate(user=self.user)

        url = reverse("user-profile")
        response = self.client.patch(
            url,
            {
                "username": "profile_update",
                "email": "profile_update@test.ua",
            },
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_200_OK)

    def test_avatar_upload_requires_file(self):
        self.client.force_authenticate(user=self.user)

        url = reverse("user-avatar")
        response = self.client.patch(url, {}, format="multipart")

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            response.data["avatar"][0],
            "Avatar file is required.",
        )

    def test_authenticated_user_can_update_avatar(self):
        self.client.force_authenticate(user=self.user)

        url = reverse("user-avatar")
        response = self.client.patch(
            url,
            {"avatar": self.get_valid_png_file()},
            format="multipart",
        )

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertIsNotNone(response.data["avatar_url"])

        self.user.refresh_from_db()

        self.assertTrue(self.user.avatar)

    def test_update_avatar_replaces_existing_avatar(self):
        self.user.avatar.save(
            "old_avatar.png",
            self.get_valid_png_file("old_avatar.png"),
            save=True,
        )

        self.client.force_authenticate(user=self.user)

        url = reverse("user-avatar")
        response = self.client.patch(
            url,
            {"avatar": self.get_valid_png_file("new_avatar.png")},
            format="multipart",
        )

        self.assertEqual(response.status_code, status.HTTP_200_OK)

        self.user.refresh_from_db()

        self.assertIn("new_avatar", self.user.avatar.name)

    def test_authenticated_user_can_delete_avatar(self):
        self.user.avatar.save(
            "avatar.png",
            self.get_valid_png_file(),
            save=True,
        )

        self.client.force_authenticate(user=self.user)

        url = reverse("user-avatar")
        response = self.client.delete(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertIsNone(response.data["avatar_url"])

        self.user.refresh_from_db()

        self.assertFalse(self.user.avatar)

    def test_delete_avatar_without_existing_avatar_returns_user(self):
        self.client.force_authenticate(user=self.user)

        url = reverse("user-avatar")
        response = self.client.delete(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertIsNone(response.data["avatar_url"])

    def test_user_can_set_contact_nickname(self):
        other_user = self.create_other_user()

        self.client.force_authenticate(user=self.user)

        url = reverse("contact-nickname", kwargs={"user_id": other_user.id})
        response = self.client.patch(
            url,
            {"nickname": "Friend"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["detail"], "Contact nickname updated.")
        self.assertEqual(response.data["user"]["display_name"], "Friend")

        self.assertTrue(
            ContactNickname.objects.filter(
                owner=self.user,
                target_user=other_user,
                nickname="Friend",
            ).exists()
        )

    def test_user_can_remove_contact_nickname(self):
        other_user = self.create_other_user()

        ContactNickname.objects.create(
            owner=self.user,
            target_user=other_user,
            nickname="Friend",
        )

        self.client.force_authenticate(user=self.user)

        url = reverse("contact-nickname", kwargs={"user_id": other_user.id})
        response = self.client.patch(
            url,
            {"nickname": "   "},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["detail"], "Contact nickname removed.")
        self.assertEqual(response.data["user"]["display_name"], other_user.username)

        self.assertFalse(
            ContactNickname.objects.filter(
                owner=self.user,
                target_user=other_user,
            ).exists()
        )

    def test_contact_nickname_missing_user_returns_404(self):
        self.client.force_authenticate(user=self.user)

        url = reverse("contact-nickname", kwargs={"user_id": 999})
        response = self.client.patch(
            url,
            {"nickname": "Friend"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_404_NOT_FOUND)
        self.assertEqual(response.data["detail"], "User not found.")

    def test_user_cannot_set_contact_nickname_for_self(self):
        self.client.force_authenticate(user=self.user)

        url = reverse("contact-nickname", kwargs={"user_id": self.user.id})
        response = self.client.patch(
            url,
            {"nickname": "Me"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            response.data["detail"],
            "You cannot set a nickname for yourself.",
        )

    def test_contact_nickname_rejects_too_long_value(self):
        other_user = self.create_other_user()

        self.client.force_authenticate(user=self.user)

        url = reverse("contact-nickname", kwargs={"user_id": other_user.id})
        response = self.client.patch(
            url,
            {"nickname": "x" * 51},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            response.data["nickname"][0],
            "Nickname must be 50 characters or less.",
        )

    def test_logout_rejects_invalid_refresh_token(self):
        self.client.force_authenticate(user=self.user)

        url = reverse("logout")
        response = self.client.post(
            url,
            {"refresh": "invalid-token"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(response.data["refresh"][0], "Invalid refresh token.")

    def test_search_without_query_returns_users_except_current_user(self):
        first_user = self.create_other_user(
            username="user1",
            email="user1@test.ua",
        )
        second_user = self.create_other_user(
            username="user2",
            email="user2@test.ua",
        )

        self.client.force_authenticate(user=self.user)

        url = reverse("user-search")
        response = self.client.get(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)

        user_ids = [user["id"] for user in response.data]

        self.assertIn(first_user.id, user_ids)
        self.assertIn(second_user.id, user_ids)
        self.assertNotIn(self.user.id, user_ids)

    @override_settings(
        EMAIL_BACKEND="django.core.mail.backends.locmem.EmailBackend",
        FRONTEND_URL="http://frontend.test",
    )
    def test_register_creates_email_verification_token_and_sends_email(self):
        url = reverse("user-register")

        with self.captureOnCommitCallbacks(execute=True):
            response = self.client.post(
                url,
                {
                    "username": "emailuser",
                    "email": "emailuser@test.ua",
                    "password": "testpassword123",
                    "password_confirm": "testpassword123",
                },
                format="json",
            )

        self.assertEqual(response.status_code, status.HTTP_201_CREATED)

        User = get_user_model()
        user = User.objects.get(username="emailuser")
        verification_token = EmailVerificationToken.objects.get(user=user)

        self.assertFalse(user.is_email_verified)
        self.assertIsNone(user.email_verified_at)
        self.assertEqual(len(mail.outbox), 1)
        self.assertEqual(mail.outbox[0].to, ["emailuser@test.ua"])
        self.assertIn(verification_token.token, mail.outbox[0].body)
        self.assertIn("http://frontend.test/verify-email", mail.outbox[0].body)

    def test_email_verify_accepts_valid_token(self):
        verification_token = EmailVerificationToken.create_for_user(self.user)

        url = reverse("email-verify")
        response = self.client.post(
            url,
            {"token": verification_token.token},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["detail"], "Email verified successfully.")
        self.assertTrue(response.data["user"]["is_email_verified"])

        self.user.refresh_from_db()
        verification_token.refresh_from_db()

        self.assertTrue(self.user.is_email_verified)
        self.assertIsNotNone(self.user.email_verified_at)
        self.assertTrue(verification_token.is_used)

    def test_email_verify_rejects_invalid_token(self):
        url = reverse("email-verify")
        response = self.client.post(
            url,
            {"token": "invalid-token"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            response.data["token"][0],
            "Invalid verification token.",
        )

    def test_email_verify_rejects_missing_token(self):
        url = reverse("email-verify")
        response = self.client.post(url, {}, format="json")

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            response.data["token"][0],
            "Verification token is required.",
        )

    def test_email_verify_rejects_expired_token(self):
        verification_token = EmailVerificationToken.create_for_user(self.user)
        verification_token.expires_at = timezone.now() - timezone.timedelta(hours=1)
        verification_token.save(update_fields=["expires_at"])

        url = reverse("email-verify")
        response = self.client.post(
            url,
            {"token": verification_token.token},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            response.data["token"][0],
            "Verification token has expired.",
        )

        self.user.refresh_from_db()

        self.assertFalse(self.user.is_email_verified)

    def test_email_verify_rejects_used_token(self):
        verification_token = EmailVerificationToken.create_for_user(self.user)
        verification_token.mark_used()

        url = reverse("email-verify")
        response = self.client.post(
            url,
            {"token": verification_token.token},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            response.data["token"][0],
            "Verification token has already been used.",
        )

        self.user.refresh_from_db()

        self.assertFalse(self.user.is_email_verified)

    @override_settings(
        EMAIL_BACKEND="django.core.mail.backends.locmem.EmailBackend",
        FRONTEND_URL="http://frontend.test",
    )
    def test_authenticated_user_can_resend_verification_email(self):
        old_token = EmailVerificationToken.create_for_user(self.user)

        self.client.force_authenticate(user=self.user)

        url = reverse("email-resend")

        with self.captureOnCommitCallbacks(execute=True):
            response = self.client.post(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["detail"], "Verification email sent.")

        old_token.refresh_from_db()
        new_token = (
            EmailVerificationToken.objects.filter(user=self.user)
            .exclude(id=old_token.id)
            .latest("created_at")
        )

        self.assertTrue(old_token.is_used)
        self.assertFalse(new_token.is_used)
        self.assertEqual(len(mail.outbox), 1)
        self.assertEqual(mail.outbox[0].to, [self.user.email])
        self.assertIn(new_token.token, mail.outbox[0].body)

    def test_resend_verification_rejects_already_verified_user(self):
        self.user.mark_email_verified()

        self.client.force_authenticate(user=self.user)

        url = reverse("email-resend")
        response = self.client.post(url)

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(response.data["detail"], "Email is already verified.")

    @override_settings(
        EMAIL_BACKEND="django.core.mail.backends.locmem.EmailBackend",
        FRONTEND_URL="http://frontend.test",
    )
    def test_password_reset_request_sends_email_for_existing_user(self):
        url = reverse("password-reset-request")

        response = self.client.post(
            url,
            {"email": "  PAVLO@TEST.UA  "},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(
            response.data["detail"],
            "If an account with this email exists, "
            "a password reset email was sent.",
        )
        self.assertEqual(len(mail.outbox), 1)
        self.assertEqual(mail.outbox[0].to, [self.user.email])
        self.assertIn("Reset your Messenger password", mail.outbox[0].subject)
        self.assertIn("http://frontend.test/reset-password", mail.outbox[0].body)
        self.assertIn("uid=", mail.outbox[0].body)
        self.assertIn("token=", mail.outbox[0].body)

    @override_settings(EMAIL_BACKEND="django.core.mail.backends.locmem.EmailBackend")
    def test_password_reset_request_does_not_reveal_missing_email(self):
        url = reverse("password-reset-request")

        response = self.client.post(
            url,
            {"email": "missing@test.ua"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(
            response.data["detail"],
            "If an account with this email exists, "
            "a password reset email was sent.",
        )
        self.assertEqual(len(mail.outbox), 0)

    def test_password_reset_request_rejects_invalid_email(self):
        url = reverse("password-reset-request")

        response = self.client.post(
            url,
            {"email": "invalid-email"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            response.data["email"][0],
            "Enter a valid email address.",
        )

    def test_password_reset_confirm_accepts_valid_token(self):
        uid = urlsafe_base64_encode(force_bytes(self.user.pk))
        token = default_token_generator.make_token(self.user)

        url = reverse("password-reset-confirm")
        response = self.client.post(
            url,
            {
                "uid": uid,
                "token": token,
                "password": "newpassword123",
                "password_confirm": "newpassword123",
            },
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["detail"], "Password reset successfully.")

        self.user.refresh_from_db()

        self.assertTrue(self.user.check_password("newpassword123"))

    def test_password_reset_confirm_rejects_invalid_token(self):
        uid = urlsafe_base64_encode(force_bytes(self.user.pk))

        url = reverse("password-reset-confirm")
        response = self.client.post(
            url,
            {
                "uid": uid,
                "token": "invalid-token",
                "password": "newpassword123",
                "password_confirm": "newpassword123",
            },
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            response.data["token"][0],
            "Invalid or expired password reset token.",
        )

        self.user.refresh_from_db()

        self.assertTrue(self.user.check_password("testpassword123"))

    def test_password_reset_confirm_rejects_mismatched_passwords(self):
        uid = urlsafe_base64_encode(force_bytes(self.user.pk))
        token = default_token_generator.make_token(self.user)

        url = reverse("password-reset-confirm")
        response = self.client.post(
            url,
            {
                "uid": uid,
                "token": token,
                "password": "newpassword123",
                "password_confirm": "differentpassword123",
            },
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            response.data["password_confirm"][0],
            "Passwords do not match.",
        )

    def test_password_reset_confirm_rejects_reused_token(self):
        uid = urlsafe_base64_encode(force_bytes(self.user.pk))
        token = default_token_generator.make_token(self.user)

        url = reverse("password-reset-confirm")
        first_response = self.client.post(
            url,
            {
                "uid": uid,
                "token": token,
                "password": "newpassword123",
                "password_confirm": "newpassword123",
            },
            format="json",
        )

        second_response = self.client.post(
            url,
            {
                "uid": uid,
                "token": token,
                "password": "anotherpassword123",
                "password_confirm": "anotherpassword123",
            },
            format="json",
        )

        self.assertEqual(first_response.status_code, status.HTTP_200_OK)
        self.assertEqual(second_response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            second_response.data["token"][0],
            "Invalid or expired password reset token.",
        )
