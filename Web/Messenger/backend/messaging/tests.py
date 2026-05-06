from django.contrib.auth import get_user_model
from django.test import TestCase, TransactionTestCase
from django.urls import reverse
from rest_framework import status
from rest_framework.test import APIClient

from .models import Conversation, Message

from asgiref.sync import async_to_sync
from channels.testing import WebsocketCommunicator
from rest_framework_simplejwt.tokens import AccessToken

from config.asgi import application


class MessagingAPITests(TestCase):
    def setUp(self):
        User = get_user_model()

        self.user = User.objects.create_user(
            username="pavlo",
            email="pavlo@test.ua",
            password="testpassword123",
        )
        self.other_user = User.objects.create_user(
            username="user1",
            email="user1@test.ua",
            password="testpassword123",
        )
        self.outsider_user = User.objects.create_user(
            username="outsider",
            email="outsider@test.ua",
            password="testpassword123",
        )

        self.conversation = Conversation.objects.create()
        self.conversation.participants.create(user=self.user)
        self.conversation.participants.create(user=self.other_user)

        self.client = APIClient()
        self.client.force_authenticate(user=self.user)

    def test_participant_can_create_message(self):
        url = reverse(
            "conversation-messages",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.post(
            url,
            {"text": "Hello from test"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_201_CREATED)
        self.assertEqual(Message.objects.count(), 1)

        message = Message.objects.first()
        self.assertEqual(message.text, "Hello from test")
        self.assertEqual(message.sender, self.user)
        self.assertEqual(message.conversation, self.conversation)

    def test_non_participant_cannot_create_message(self):
        self.client.force_authenticate(user=self.outsider_user)

        url = reverse(
            "conversation-messages",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.post(
            url,
            {"text": "I should not be allowed"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_404_NOT_FOUND)
        self.assertEqual(Message.objects.count(), 0)
        self.assertEqual(response.data["detail"], "Conversation not found.")

    def test_cannot_create_empty_message(self):
        url = reverse(
            "conversation-messages",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.post(
            url,
            {"text": "   "},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(Message.objects.count(), 0)
        self.assertEqual(
            response.data["text"][0],
            "Message text cannot be empty.",
        )

    def test_participant_can_list_messages(self):
        Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="First message",
        )
        Message.objects.create(
            conversation=self.conversation,
            sender=self.other_user,
            text="Second message",
        )

        url = reverse(
            "conversation-messages",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.get(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(len(response.data), 2)
        self.assertEqual(response.data[0]["text"], "First message")
        self.assertEqual(response.data[1]["text"], "Second message")

    def test_non_participant_cannot_list_messages(self):
        self.client.force_authenticate(user=self.outsider_user)

        Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Private message",
        )

        url = reverse(
            "conversation-messages",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.get(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(len(response.data), 0)

    def test_participant_can_get_conversation_detail(self):
        url = reverse(
            "conversation-detail",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.get(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["id"], self.conversation.id)
        self.assertEqual(len(response.data["participants"]), 2)

    def test_non_participant_cannot_get_conversation_detail(self):
        self.client.force_authenticate(user=self.outsider_user)

        url = reverse(
            "conversation-detail",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.get(url)

        self.assertEqual(response.status_code, status.HTTP_404_NOT_FOUND)
        self.assertEqual(response.data["detail"], "Conversation not found.")

    def test_user_can_list_only_own_conversations(self):
        other_conversation = Conversation.objects.create()
        other_conversation.participants.create(user=self.other_user)
        other_conversation.participants.create(user=self.outsider_user)

        url = reverse("conversation-list")

        response = self.client.get(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)

        conversation_ids = [conversation["id"] for conversation in response.data]

        self.assertIn(self.conversation.id, conversation_ids)
        self.assertNotIn(other_conversation.id, conversation_ids)

    def test_user_can_create_conversation_with_existing_user(self):
        url = reverse("conversation-list")

        response = self.client.post(
            url,
            {"user_id": self.outsider_user.id},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_201_CREATED)
        self.assertEqual(Conversation.objects.count(), 2)

        new_conversation_id = response.data["id"]
        new_conversation = Conversation.objects.get(id=new_conversation_id)

        participant_ids = list(
            new_conversation.participants.values_list("user_id", flat=True)
        )

        self.assertIn(self.user.id, participant_ids)
        self.assertIn(self.outsider_user.id, participant_ids)

    def test_create_conversation_returns_existing_conversation(self):
        url = reverse("conversation-list")

        response = self.client.post(
            url,
            {"user_id": self.other_user.id},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["id"], self.conversation.id)
        self.assertEqual(Conversation.objects.count(), 1)

    def test_cannot_create_conversation_with_self(self):
        url = reverse("conversation-list")

        response = self.client.post(
            url,
            {"user_id": self.user.id},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            response.data["user_id"][0],
            "You cannot create a conversation with yourself.",
        )

    def test_cannot_create_conversation_with_missing_user(self):
        url = reverse("conversation-list")

        response = self.client.post(
            url,
            {"user_id": 999},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(response.data["user_id"][0], "User not found.")

    def test_conversation_list_contains_last_message(self):
        Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="First message",
        )
        Message.objects.create(
            conversation=self.conversation,
            sender=self.other_user,
            text="Last message",
        )

        url = reverse("conversation-list")

        response = self.client.get(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data[0]["last_message"]["text"], "Last message")

    def test_conversation_list_contains_unread_count(self):
        Message.objects.create(
            conversation=self.conversation,
            sender=self.other_user,
            text="Unread message from other user",
            is_read=False,
        )
        Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="My own unread message should not count",
            is_read=False,
        )

        url = reverse("conversation-list")

        response = self.client.get(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data[0]["unread_count"], 1)

    def test_mark_read_marks_only_other_user_messages(self):
        other_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.other_user,
            text="Unread message",
            is_read=False,
        )
        own_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Own message",
            is_read=False,
        )

        url = reverse(
            "conversation-mark-read",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.post(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["updated_count"], 1)

        other_message.refresh_from_db()
        own_message.refresh_from_db()

        self.assertTrue(other_message.is_read)
        self.assertFalse(own_message.is_read)

    def test_mark_read_ignores_deleted_messages(self):
        active_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.other_user,
            text="Unread active message",
            is_read=False,
            is_deleted=False,
        )
        deleted_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.other_user,
            text="",
            is_read=False,
            is_deleted=True,
        )

        url = reverse(
            "conversation-mark-read",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.post(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["updated_count"], 1)

        active_message.refresh_from_db()
        deleted_message.refresh_from_db()

        self.assertTrue(active_message.is_read)
        self.assertFalse(deleted_message.is_read)

    def test_non_participant_cannot_mark_read(self):
        self.client.force_authenticate(user=self.outsider_user)

        url = reverse(
            "conversation-mark-read",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.post(url)

        self.assertEqual(response.status_code, status.HTTP_404_NOT_FOUND)
        self.assertEqual(response.data["detail"], "Conversation not found.")

    def test_sender_can_edit_message(self):
        message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Before edit",
        )

        url = reverse(
            "message-detail",
            kwargs={"message_id": message.id},
        )

        response = self.client.patch(
            url,
            {"text": "After edit"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_200_OK)

        message.refresh_from_db()

        self.assertEqual(message.text, "After edit")
        self.assertIsNotNone(message.edited_at)
        self.assertEqual(response.data["text"], "After edit")
        self.assertIsNotNone(response.data["edited_at"])

    def test_edit_requires_text(self):
        message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Before edit",
        )

        url = reverse(
            "message-detail",
            kwargs={"message_id": message.id},
        )

        response = self.client.patch(
            url,
            {},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(response.data["text"][0], "Message text is required.")

    def test_cannot_edit_empty_message(self):
        message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Before edit",
        )

        url = reverse(
            "message-detail",
            kwargs={"message_id": message.id},
        )

        response = self.client.patch(
            url,
            {"text": "   "},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            response.data["text"][0],
            "Message text cannot be empty.",
        )

    def test_other_user_cannot_edit_message(self):
        message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Pavlo message",
        )

        self.client.force_authenticate(user=self.other_user)

        url = reverse(
            "message-detail",
            kwargs={"message_id": message.id},
        )

        response = self.client.patch(
            url,
            {"text": "User1 tries to edit"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_404_NOT_FOUND)
        self.assertEqual(response.data["detail"], "Message not found.")

        message.refresh_from_db()

        self.assertEqual(message.text, "Pavlo message")

    def test_deleted_message_cannot_be_edited(self):
        message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="",
            is_deleted=True,
        )

        url = reverse(
            "message-detail",
            kwargs={"message_id": message.id},
        )

        response = self.client.patch(
            url,
            {"text": "Trying to edit deleted message"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(response.data["detail"], "Deleted message cannot be edited.")

    def test_sender_can_soft_delete_message(self):
        message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Message for delete",
        )

        url = reverse(
            "message-detail",
            kwargs={"message_id": message.id},
        )

        response = self.client.delete(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)

        message.refresh_from_db()

        self.assertEqual(message.text, "")
        self.assertTrue(message.is_deleted)
        self.assertIsNotNone(message.deleted_at)

        self.assertEqual(response.data["text"], "")
        self.assertTrue(response.data["is_deleted"])
        self.assertIsNotNone(response.data["deleted_at"])

    def test_other_user_cannot_delete_message(self):
        message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Pavlo message",
        )

        self.client.force_authenticate(user=self.other_user)

        url = reverse(
            "message-detail",
            kwargs={"message_id": message.id},
        )

        response = self.client.delete(url)

        self.assertEqual(response.status_code, status.HTTP_404_NOT_FOUND)
        self.assertEqual(response.data["detail"], "Message not found.")

        message.refresh_from_db()

        self.assertFalse(message.is_deleted)
        self.assertEqual(message.text, "Pavlo message")
        
class MessagingWebSocketTests(TransactionTestCase):
    def setUp(self):
        User = get_user_model()

        self.user = User.objects.create_user(
            username="pavlo",
            email="pavlo@test.ua",
            password="testpassword123",
        )
        self.other_user = User.objects.create_user(
            username="user1",
            email="user1@test.ua",
            password="testpassword123",
        )
        self.outsider_user = User.objects.create_user(
            username="outsider",
            email="outsider@test.ua",
            password="testpassword123",
        )

        self.conversation = Conversation.objects.create()
        self.conversation.participants.create(user=self.user)
        self.conversation.participants.create(user=self.other_user)

    def get_access_token(self, user):
        return str(AccessToken.for_user(user))

    async def connect_to_conversation(self, user):
        access_token = self.get_access_token(user)

        communicator = WebsocketCommunicator(
            application,
            f"/ws/conversations/{self.conversation.id}/?token={access_token}",
        )

        connected, _ = await communicator.connect()
        return communicator, connected

    def test_participant_can_connect_to_websocket(self):
        async def test():
            communicator, connected = await self.connect_to_conversation(self.user)

            self.assertTrue(connected)

            await communicator.disconnect()

        async_to_sync(test)()

    def test_non_participant_cannot_connect_to_websocket(self):
        async def test():
            communicator, connected = await self.connect_to_conversation(
                self.outsider_user
            )

            self.assertFalse(connected)

            await communicator.disconnect()

        async_to_sync(test)()

    def test_websocket_message_is_saved_to_database(self):
        async def test():
            communicator, connected = await self.connect_to_conversation(self.user)

            self.assertTrue(connected)

            await communicator.send_json_to(
                {
                    "text": "Hello from WebSocket test",
                }
            )

            response = await communicator.receive_json_from()

            self.assertEqual(response["type"], "message")
            self.assertEqual(response["message"]["text"], "Hello from WebSocket test")
            self.assertEqual(response["message"]["conversation"], self.conversation.id)
            self.assertEqual(response["message"]["sender"]["id"], self.user.id)

            await communicator.disconnect()

        async_to_sync(test)()

        self.assertEqual(Message.objects.count(), 1)

        message = Message.objects.first()

        self.assertEqual(message.text, "Hello from WebSocket test")
        self.assertEqual(message.sender, self.user)
        self.assertEqual(message.conversation, self.conversation)

    def test_websocket_rejects_empty_message(self):
        async def test():
            communicator, connected = await self.connect_to_conversation(self.user)

            self.assertTrue(connected)

            await communicator.send_json_to(
                {
                    "text": "   ",
                }
            )

            response = await communicator.receive_json_from()

            self.assertEqual(response["type"], "error")
            self.assertEqual(response["detail"], "Message text cannot be empty.")

            await communicator.disconnect()

        async_to_sync(test)()

        self.assertEqual(Message.objects.count(), 0)