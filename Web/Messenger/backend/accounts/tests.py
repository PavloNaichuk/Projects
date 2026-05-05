from django.contrib.auth import get_user_model
from django.urls import reverse
from rest_framework import status
from rest_framework.test import APIClient
from django.test import TestCase
from rest_framework_simplejwt.tokens import RefreshToken


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