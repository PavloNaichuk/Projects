from django.contrib.auth import get_user_model
from django.test import TestCase, override_settings
from django.urls import reverse
from rest_framework import status
from rest_framework.test import APIClient
from rest_framework_simplejwt.tokens import RefreshToken

from .models import BlockedUser

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