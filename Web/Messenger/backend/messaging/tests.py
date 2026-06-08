from asgiref.sync import async_to_sync
from channels.testing import WebsocketCommunicator
from django.contrib.auth import get_user_model
from django.core.files.uploadedfile import SimpleUploadedFile
from django.test import TestCase, TransactionTestCase, override_settings
from django.urls import reverse
from rest_framework import status
from rest_framework.test import APIClient
from rest_framework_simplejwt.tokens import AccessToken

from accounts.models import BlockedUser
from config.asgi import application

from .consumers import ACTIVE_USER_CONNECTIONS
from .models import Conversation, Message, MessageReaction

TEST_CHANNEL_LAYERS = {
    "default": {
        "BACKEND": "channels.layers.InMemoryChannelLayer",
    },
}


@override_settings(CHANNEL_LAYERS=TEST_CHANNEL_LAYERS)
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

    def test_cannot_create_message_with_unsupported_attachment_type(self):
        url = reverse(
            "conversation-messages",
            kwargs={"conversation_id": self.conversation.id},
        )

        attachment = SimpleUploadedFile(
            "virus.exe",
            b"fake executable content",
            content_type="application/x-msdownload",
        )

        response = self.client.post(
            url,
            {
                "text": "File test",
                "attachment": attachment,
            },
            format="multipart",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(Message.objects.count(), 0)
        self.assertEqual(
            response.data["attachment"][0],
            "Attachment must be JPG, PNG, WEBP, PDF, TXT, DOC, or DOCX.",
        )

    def test_cannot_create_message_with_too_large_attachment(self):
        url = reverse(
            "conversation-messages",
            kwargs={"conversation_id": self.conversation.id},
        )

        attachment = SimpleUploadedFile(
            "large.txt",
            b"x" * (10 * 1024 * 1024 + 1),
            content_type="text/plain",
        )

        response = self.client.post(
            url,
            {
                "text": "Large file test",
                "attachment": attachment,
            },
            format="multipart",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(Message.objects.count(), 0)
        self.assertEqual(
            response.data["attachment"][0],
            "Attachment file size must be 10 MB or less.",
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

        response = self.client.delete(f"{url}?mode=for_everyone")

        self.assertEqual(response.status_code, status.HTTP_200_OK)

        message.refresh_from_db()

        self.assertEqual(message.text, "")
        self.assertTrue(message.is_deleted)
        self.assertIsNotNone(message.deleted_at)

    def test_other_user_cannot_delete_message_for_everyone(self):
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

        response = self.client.delete(f"{url}?mode=for_everyone")

        self.assertEqual(response.status_code, status.HTTP_403_FORBIDDEN)

        message.refresh_from_db()

        self.assertFalse(message.is_deleted)
        self.assertEqual(message.text, "Pavlo message")

    def test_participant_can_delete_message_for_me(self):
        message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Message visible to both users",
        )

        self.client.force_authenticate(user=self.other_user)

        url = reverse(
            "message-detail",
            kwargs={"message_id": message.id},
        )

        response = self.client.delete(f"{url}?mode=for_me")

        self.assertEqual(response.status_code, status.HTTP_200_OK)

        message.refresh_from_db()

        self.assertFalse(message.is_deleted)
        self.assertEqual(message.text, "Message visible to both users")
        self.assertTrue(message.hidden_for.filter(id=self.other_user.id).exists())
        self.assertFalse(message.hidden_for.filter(id=self.user.id).exists())

    def test_user_cannot_send_message_to_user_they_blocked(self):
        BlockedUser.objects.create(
            blocker=self.user,
            blocked=self.other_user,
        )

        url = reverse(
            "conversation-messages",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.post(
            url,
            {"text": "Blocked message"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_403_FORBIDDEN)
        self.assertEqual(
            response.data["detail"],
            "You blocked this user. Unblock them to send messages.",
        )
        self.assertEqual(Message.objects.count(), 0)

    def test_user_cannot_send_message_to_user_who_blocked_them(self):
        BlockedUser.objects.create(
            blocker=self.other_user,
            blocked=self.user,
        )

        url = reverse(
            "conversation-messages",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.post(
            url,
            {"text": "Blocked message"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_403_FORBIDDEN)
        self.assertEqual(
            response.data["detail"],
            "You cannot send messages to this user.",
        )
        self.assertEqual(Message.objects.count(), 0)

    def test_user_cannot_create_conversation_with_user_they_blocked(self):
        BlockedUser.objects.create(
            blocker=self.user,
            blocked=self.outsider_user,
        )

        url = reverse("conversation-list")

        response = self.client.post(
            url,
            {"user_id": self.outsider_user.id},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_403_FORBIDDEN)
        self.assertEqual(
            response.data["detail"],
            "You blocked this user. Unblock them to send messages.",
        )

    def test_user_cannot_create_conversation_with_user_who_blocked_them(self):
        BlockedUser.objects.create(
            blocker=self.outsider_user,
            blocked=self.user,
        )

        url = reverse("conversation-list")

        response = self.client.post(
            url,
            {"user_id": self.outsider_user.id},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_403_FORBIDDEN)
        self.assertEqual(
            response.data["detail"],
            "You cannot send messages to this user.",
        )

    def test_user_cannot_forward_message_to_user_they_blocked(self):
        target_conversation = Conversation.objects.create()
        target_conversation.participants.create(user=self.user)
        target_conversation.participants.create(user=self.outsider_user)

        source_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.other_user,
            text="Message to forward",
        )

        BlockedUser.objects.create(
            blocker=self.user,
            blocked=self.outsider_user,
        )

        url = reverse(
            "message-forward",
            kwargs={"message_id": source_message.id},
        )

        response = self.client.post(
            url,
            {"conversation_id": target_conversation.id},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_403_FORBIDDEN)
        self.assertEqual(
            response.data["detail"],
            "You blocked this user. Unblock them to send messages.",
        )
        self.assertEqual(Message.objects.count(), 1)

    def test_user_cannot_forward_message_to_user_who_blocked_them(self):
        target_conversation = Conversation.objects.create()
        target_conversation.participants.create(user=self.user)
        target_conversation.participants.create(user=self.outsider_user)

        source_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.other_user,
            text="Message to forward",
        )

        BlockedUser.objects.create(
            blocker=self.outsider_user,
            blocked=self.user,
        )

        url = reverse(
            "message-forward",
            kwargs={"message_id": source_message.id},
        )

        response = self.client.post(
            url,
            {"conversation_id": target_conversation.id},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_403_FORBIDDEN)
        self.assertEqual(
            response.data["detail"],
            "You cannot send messages to this user.",
        )
        self.assertEqual(Message.objects.count(), 1)

    def test_user_can_add_message_reaction(self):
        message = Message.objects.create(
            conversation=self.conversation,
            sender=self.other_user,
            text="React to me",
        )

        url = reverse(
            "message-reaction-toggle",
            kwargs={"message_id": message.id},
        )

        response = self.client.post(
            url,
            {"emoji": "👍"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["action"], "added")
        self.assertTrue(
            MessageReaction.objects.filter(
                message=message,
                user=self.user,
                emoji="👍",
            ).exists()
        )

    def test_user_can_remove_message_reaction(self):
        message = Message.objects.create(
            conversation=self.conversation,
            sender=self.other_user,
            text="React to me",
        )

        url = reverse(
            "message-reaction-toggle",
            kwargs={"message_id": message.id},
        )

        self.client.post(
            url,
            {"emoji": "👍"},
            format="json",
        )
        response = self.client.post(
            url,
            {"emoji": "👍"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["action"], "removed")
        self.assertFalse(
            MessageReaction.objects.filter(
                message=message,
                user=self.user,
                emoji="👍",
            ).exists()
        )

    def test_user_cannot_react_with_unsupported_emoji(self):
        message = Message.objects.create(
            conversation=self.conversation,
            sender=self.other_user,
            text="React to me",
        )

        url = reverse(
            "message-reaction-toggle",
            kwargs={"message_id": message.id},
        )

        response = self.client.post(
            url,
            {"emoji": "🔥"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(response.data["emoji"][0], "Unsupported reaction.")
        self.assertEqual(MessageReaction.objects.count(), 0)

    def test_non_participant_cannot_react_to_message(self):
        message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Private message",
        )

        self.client.force_authenticate(user=self.outsider_user)

        url = reverse(
            "message-reaction-toggle",
            kwargs={"message_id": message.id},
        )

        response = self.client.post(
            url,
            {"emoji": "👍"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_404_NOT_FOUND)
        self.assertEqual(response.data["detail"], "Message not found.")
        self.assertEqual(MessageReaction.objects.count(), 0)

    def test_user_cannot_react_to_deleted_message(self):
        message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="",
            is_deleted=True,
        )

        url = reverse(
            "message-reaction-toggle",
            kwargs={"message_id": message.id},
        )

        response = self.client.post(
            url,
            {"emoji": "👍"},
            format="json",
        )

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(response.data["detail"], "Cannot react to deleted message.")
        self.assertEqual(MessageReaction.objects.count(), 0)

    def test_participant_can_mark_conversation_as_unread(self):
        url = reverse(
            "conversation-mark-unread",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.post(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["detail"], "Conversation marked as unread.")

        self.conversation.refresh_from_db()

        self.assertTrue(
            self.conversation.unread_for.filter(id=self.user.id).exists()
        )

    def test_non_participant_cannot_mark_conversation_as_unread(self):
        self.client.force_authenticate(user=self.outsider_user)

        url = reverse(
            "conversation-mark-unread",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.post(url)

        self.assertEqual(response.status_code, status.HTTP_404_NOT_FOUND)
        self.assertEqual(response.data["detail"], "Conversation not found.")

    def test_participant_can_clear_history_for_self(self):
        first_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="First message",
        )
        second_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.other_user,
            text="Second message",
        )

        url = reverse(
            "conversation-clear-history",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.post(url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["detail"], "Chat history cleared for you.")

        self.assertTrue(first_message.hidden_for.filter(id=self.user.id).exists())
        self.assertTrue(second_message.hidden_for.filter(id=self.user.id).exists())
        self.assertFalse(first_message.hidden_for.filter(id=self.other_user.id).exists())
        self.assertFalse(second_message.hidden_for.filter(id=self.other_user.id).exists())

    def test_non_participant_cannot_clear_history(self):
        self.client.force_authenticate(user=self.outsider_user)

        url = reverse(
            "conversation-clear-history",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.post(url)

        self.assertEqual(response.status_code, status.HTTP_404_NOT_FOUND)
        self.assertEqual(response.data["detail"], "Conversation not found.")

    def test_message_pagination_returns_latest_messages_with_has_more(self):
        first_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="First message",
        )
        second_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Second message",
        )
        third_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Third message",
        )

        url = reverse(
            "conversation-messages",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.get(f"{url}?limit=2")

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["has_more"], True)
        self.assertEqual(response.data["next_before"], second_message.id)
        self.assertEqual(len(response.data["results"]), 2)
        self.assertEqual(response.data["results"][0]["id"], second_message.id)
        self.assertEqual(response.data["results"][1]["id"], third_message.id)
        self.assertNotEqual(response.data["results"][0]["id"], first_message.id)

    def test_message_pagination_before_returns_older_messages(self):
        first_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="First message",
        )
        second_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Second message",
        )
        third_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Third message",
        )

        url = reverse(
            "conversation-messages",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.get(f"{url}?limit=2&before={third_message.id}")

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["has_more"], False)
        self.assertIsNone(response.data["next_before"])
        self.assertEqual(len(response.data["results"]), 2)
        self.assertEqual(response.data["results"][0]["id"], first_message.id)
        self.assertEqual(response.data["results"][1]["id"], second_message.id)

    def test_message_pagination_rejects_invalid_limit(self):
        url = reverse(
            "conversation-messages",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.get(f"{url}?limit=abc")

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(response.data["limit"][0], "Limit must be a number.")

    def test_message_pagination_rejects_zero_limit(self):
        url = reverse(
            "conversation-messages",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.get(f"{url}?limit=0")

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(response.data["limit"][0], "Limit must be greater than 0.")

    def test_message_pagination_rejects_invalid_before(self):
        url = reverse(
            "conversation-messages",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.get(f"{url}?limit=20&before=abc")

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(response.data["before"][0], "Before must be a message id.")

    def test_message_search_returns_matching_active_messages(self):
        matching_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Hello Django",
        )
        Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Another message",
        )
        Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Hello deleted",
            is_deleted=True,
        )

        url = reverse(
            "conversation-messages",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.get(f"{url}?search=hello")

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["has_more"], False)
        self.assertIsNone(response.data["next_before"])
        self.assertEqual(len(response.data["results"]), 1)
        self.assertEqual(response.data["results"][0]["id"], matching_message.id)
        self.assertEqual(response.data["results"][0]["text"], "Hello Django")

    def test_message_page_excludes_messages_hidden_for_current_user(self):
        visible_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.user,
            text="Visible message",
        )
        hidden_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.other_user,
            text="Hidden message",
        )
        hidden_message.hidden_for.add(self.user)

        url = reverse(
            "conversation-messages",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.get(f"{url}?limit=20")

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(len(response.data["results"]), 1)
        self.assertEqual(response.data["results"][0]["id"], visible_message.id)

    def test_delete_conversation_rejects_invalid_mode(self):
        url = reverse(
            "conversation-detail",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.delete(f"{url}?mode=invalid")

        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)
        self.assertEqual(
            response.data["mode"][0],
            "Mode must be 'for_me' or 'for_everyone'.",
        )

    def test_delete_conversation_for_me_hides_conversation_only_for_current_user(self):
        url = reverse(
            "conversation-detail",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.delete(f"{url}?mode=for_me")

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["mode"], "for_me")

        self.conversation.refresh_from_db()

        self.assertTrue(
            self.conversation.hidden_for.filter(id=self.user.id).exists()
        )
        self.assertFalse(
            self.conversation.hidden_for.filter(id=self.other_user.id).exists()
        )

    def test_delete_conversation_for_everyone_removes_conversation(self):
        conversation_id = self.conversation.id

        url = reverse(
            "conversation-detail",
            kwargs={"conversation_id": conversation_id},
        )

        response = self.client.delete(f"{url}?mode=for_everyone")

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data["mode"], "for_everyone")
        self.assertFalse(Conversation.objects.filter(id=conversation_id).exists())

    def test_participant_can_mute_and_unmute_conversation(self):
        url = reverse(
            "conversation-mute",
            kwargs={"conversation_id": self.conversation.id},
        )

        mute_response = self.client.post(url, {"is_muted": True}, format="json")

        self.assertEqual(mute_response.status_code, status.HTTP_200_OK)
        self.assertEqual(mute_response.data["detail"], "Conversation muted.")
        self.assertTrue(
            self.conversation.muted_for.filter(id=self.user.id).exists()
        )

        unmute_response = self.client.post(url, {"is_muted": False}, format="json")

        self.assertEqual(unmute_response.status_code, status.HTTP_200_OK)
        self.assertEqual(unmute_response.data["detail"], "Conversation unmuted.")
        self.assertFalse(
            self.conversation.muted_for.filter(id=self.user.id).exists()
        )

    def test_non_participant_cannot_mute_conversation(self):
        self.client.force_authenticate(user=self.outsider_user)

        url = reverse(
            "conversation-mute",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.post(url, {"is_muted": True}, format="json")

        self.assertEqual(response.status_code, status.HTTP_404_NOT_FOUND)
        self.assertEqual(response.data["detail"], "Conversation not found.")

    def test_participant_can_pin_and_unpin_conversation(self):
        url = reverse(
            "conversation-pin",
            kwargs={"conversation_id": self.conversation.id},
        )

        pin_response = self.client.post(url, {"is_pinned": True}, format="json")

        self.assertEqual(pin_response.status_code, status.HTTP_200_OK)
        self.assertEqual(pin_response.data["detail"], "Conversation pinned.")
        self.assertTrue(
            self.conversation.pinned_for.filter(id=self.user.id).exists()
        )

        unpin_response = self.client.post(url, {"is_pinned": False}, format="json")

        self.assertEqual(unpin_response.status_code, status.HTTP_200_OK)
        self.assertEqual(unpin_response.data["detail"], "Conversation unpinned.")
        self.assertFalse(
            self.conversation.pinned_for.filter(id=self.user.id).exists()
        )

    def test_non_participant_cannot_pin_conversation(self):
        self.client.force_authenticate(user=self.outsider_user)

        url = reverse(
            "conversation-pin",
            kwargs={"conversation_id": self.conversation.id},
        )

        response = self.client.post(url, {"is_pinned": True}, format="json")

        self.assertEqual(response.status_code, status.HTTP_404_NOT_FOUND)
        self.assertEqual(response.data["detail"], "Conversation not found.")


@override_settings(CHANNEL_LAYERS=TEST_CHANNEL_LAYERS)
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

    def test_websocket_rejects_invalid_json(self):
        async def test():
            communicator, connected = await self.connect_to_conversation(self.user)

            self.assertTrue(connected)

            await communicator.send_to(text_data="{invalid json")

            response = await communicator.receive_json_from()

            self.assertEqual(response["type"], "error")
            self.assertEqual(response["detail"], "Invalid JSON.")

            await communicator.disconnect()

        async_to_sync(test)()

        self.assertEqual(Message.objects.count(), 0)

    def test_websocket_broadcasts_message_to_conversation_participants(self):
        async def test():
            sender_communicator, sender_connected = await self.connect_to_conversation(
                self.user
            )
            (
                receiver_communicator,
                receiver_connected,
            ) = await self.connect_to_conversation(self.other_user)

            self.assertTrue(sender_connected)
            self.assertTrue(receiver_connected)

            await sender_communicator.send_json_to(
                {
                    "text": "Broadcast message",
                }
            )

            sender_response = await sender_communicator.receive_json_from()
            receiver_response = await receiver_communicator.receive_json_from()

            self.assertEqual(sender_response["type"], "message")
            self.assertEqual(receiver_response["type"], "message")

            self.assertEqual(
                sender_response["message"]["text"],
                "Broadcast message",
            )
            self.assertEqual(
                receiver_response["message"]["text"],
                "Broadcast message",
            )

            self.assertEqual(
                sender_response["message"]["id"],
                receiver_response["message"]["id"],
            )

            await sender_communicator.disconnect()
            await receiver_communicator.disconnect()

        async_to_sync(test)()

        self.assertEqual(Message.objects.count(), 1)

        message = Message.objects.first()

        self.assertEqual(message.text, "Broadcast message")
        self.assertEqual(message.sender, self.user)
        self.assertEqual(message.conversation, self.conversation)

    def test_websocket_message_can_reply_to_existing_message(self):
        original_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.other_user,
            text="Original message",
        )

        async def test():
            communicator, connected = await self.connect_to_conversation(self.user)

            self.assertTrue(connected)

            await communicator.send_json_to(
                {
                    "text": "Reply message",
                    "reply_to": original_message.id,
                }
            )

            response = await communicator.receive_json_from()

            self.assertEqual(response["type"], "message")
            self.assertEqual(response["message"]["text"], "Reply message")
            self.assertEqual(
                response["message"]["reply_to_message"]["id"],
                original_message.id,
            )
            self.assertEqual(
                response["message"]["reply_to_message"]["text"],
                "Original message",
            )

            await communicator.disconnect()

        async_to_sync(test)()

        self.assertEqual(Message.objects.count(), 2)

        reply_message = Message.objects.get(text="Reply message")

        self.assertEqual(reply_message.reply_to_id, original_message.id)

    def test_websocket_rejects_missing_reply_message(self):
        async def test():
            communicator, connected = await self.connect_to_conversation(self.user)

            self.assertTrue(connected)

            await communicator.send_json_to(
                {
                    "text": "Reply message",
                    "reply_to": 999,
                }
            )

            response = await communicator.receive_json_from()

            self.assertEqual(response["type"], "error")
            self.assertEqual(response["detail"], "Reply message not found.")

            await communicator.disconnect()

        async_to_sync(test)()

        self.assertEqual(Message.objects.count(), 0)

    def test_websocket_rejects_message_when_user_blocked_receiver(self):
        BlockedUser.objects.create(
            blocker=self.user,
            blocked=self.other_user,
        )

        async def test():
            communicator, connected = await self.connect_to_conversation(self.user)

            self.assertTrue(connected)

            await communicator.send_json_to(
                {
                    "text": "Blocked message",
                }
            )

            response = await communicator.receive_json_from()

            self.assertEqual(response["type"], "error")
            self.assertEqual(
                response["detail"],
                "You blocked this user. Unblock them to send messages.",
            )

            await communicator.disconnect()

        async_to_sync(test)()

        self.assertEqual(Message.objects.count(), 0)

    def test_websocket_rejects_message_when_receiver_blocked_user(self):
        BlockedUser.objects.create(
            blocker=self.other_user,
            blocked=self.user,
        )

        async def test():
            communicator, connected = await self.connect_to_conversation(self.user)

            self.assertTrue(connected)

            await communicator.send_json_to(
                {
                    "text": "Blocked message",
                }
            )

            response = await communicator.receive_json_from()

            self.assertEqual(response["type"], "error")
            self.assertEqual(
                response["detail"],
                "You cannot send messages to this user.",
            )

            await communicator.disconnect()

        async_to_sync(test)()

        self.assertEqual(Message.objects.count(), 0)

    def test_websocket_broadcasts_typing_event_to_participants(self):
        async def test():
            sender_communicator, sender_connected = await self.connect_to_conversation(
                self.user
            )
            (
                receiver_communicator,
                receiver_connected,
            ) = await self.connect_to_conversation(self.other_user)

            self.assertTrue(sender_connected)
            self.assertTrue(receiver_connected)

            await sender_communicator.send_json_to(
                {
                    "type": "typing",
                    "is_typing": True,
                }
            )

            sender_response = await sender_communicator.receive_json_from()
            receiver_response = await receiver_communicator.receive_json_from()

            self.assertEqual(sender_response["type"], "typing")
            self.assertEqual(receiver_response["type"], "typing")
            self.assertTrue(sender_response["is_typing"])
            self.assertTrue(receiver_response["is_typing"])
            self.assertEqual(sender_response["user"]["id"], self.user.id)
            self.assertEqual(receiver_response["user"]["id"], self.user.id)

            await sender_communicator.disconnect()
            await receiver_communicator.disconnect()

        async_to_sync(test)()

    def test_websocket_ignores_typing_event_when_conversation_is_blocked(self):
        BlockedUser.objects.create(
            blocker=self.user,
            blocked=self.other_user,
        )

        async def test():
            sender_communicator, sender_connected = await self.connect_to_conversation(
                self.user
            )
            (
                receiver_communicator,
                receiver_connected,
            ) = await self.connect_to_conversation(self.other_user)

            self.assertTrue(sender_connected)
            self.assertTrue(receiver_connected)

            await sender_communicator.send_json_to(
                {
                    "type": "typing",
                    "is_typing": True,
                }
            )

            self.assertTrue(await sender_communicator.receive_nothing(timeout=0.1))
            self.assertTrue(await receiver_communicator.receive_nothing(timeout=0.1))

            await sender_communicator.disconnect()
            await receiver_communicator.disconnect()

        async_to_sync(test)()

    def test_websocket_read_event_marks_messages_as_read(self):
        unread_message = Message.objects.create(
            conversation=self.conversation,
            sender=self.other_user,
            text="Unread message",
            is_read=False,
            is_delivered=False,
        )

        async def test():
            sender_communicator, sender_connected = await self.connect_to_conversation(
                self.user
            )
            (
                receiver_communicator,
                receiver_connected,
            ) = await self.connect_to_conversation(self.other_user)

            self.assertTrue(sender_connected)
            self.assertTrue(receiver_connected)

            await sender_communicator.send_json_to(
                {
                    "type": "read",
                }
            )

            sender_response = await sender_communicator.receive_json_from()
            receiver_response = await receiver_communicator.receive_json_from()

            self.assertEqual(sender_response["type"], "read")
            self.assertEqual(receiver_response["type"], "read")
            self.assertEqual(sender_response["updated_count"], 1)
            self.assertEqual(receiver_response["updated_count"], 1)
            self.assertEqual(sender_response["user"]["id"], self.user.id)
            self.assertEqual(receiver_response["user"]["id"], self.user.id)

            await sender_communicator.disconnect()
            await receiver_communicator.disconnect()

        async_to_sync(test)()

        unread_message.refresh_from_db()

        self.assertTrue(unread_message.is_read)
        self.assertTrue(unread_message.is_delivered)

    def test_websocket_sends_delivery_event_when_receiver_is_online(self):
        ACTIVE_USER_CONNECTIONS[self.other_user.id] = 1

        async def test():
            sender_communicator, sender_connected = await self.connect_to_conversation(
                self.user
            )
            (
                receiver_communicator,
                receiver_connected,
            ) = await self.connect_to_conversation(self.other_user)

            self.assertTrue(sender_connected)
            self.assertTrue(receiver_connected)

            await sender_communicator.send_json_to(
                {
                    "text": "Delivery message",
                }
            )

            sender_message_response = await sender_communicator.receive_json_from()
            receiver_message_response = await receiver_communicator.receive_json_from()
            sender_delivery_response = await sender_communicator.receive_json_from()
            receiver_delivery_response = await receiver_communicator.receive_json_from()

            self.assertEqual(sender_message_response["type"], "message")
            self.assertEqual(receiver_message_response["type"], "message")
            self.assertEqual(sender_delivery_response["type"], "delivered")
            self.assertEqual(receiver_delivery_response["type"], "delivered")
            self.assertEqual(
                sender_delivery_response["message_ids"],
                receiver_delivery_response["message_ids"],
            )
            self.assertEqual(sender_delivery_response["user_id"], self.other_user.id)
            self.assertEqual(receiver_delivery_response["user_id"], self.other_user.id)

            await sender_communicator.disconnect()
            await receiver_communicator.disconnect()

        try:
            async_to_sync(test)()
        finally:
            ACTIVE_USER_CONNECTIONS.clear()

        message = Message.objects.get(text="Delivery message")

        self.assertTrue(message.is_delivered)
        self.assertIsNotNone(message.delivered_at)

