import json

from channels.db import database_sync_to_async
from channels.generic.websocket import AsyncWebsocketConsumer
from django.utils import timezone

from .models import Conversation, ConversationParticipant, Message
from .serializers import MessageSerializer

ACTIVE_USER_CONNECTIONS = {}


def serialize_user_for_status(user):
    avatar_url = None

    if user.avatar:
        avatar_url = user.avatar.url

    last_seen_at = None

    if user.last_seen_at:
        last_seen_at = user.last_seen_at.isoformat()

    return {
        "id": user.id,
        "username": user.username,
        "email": user.email,
        "avatar_url": avatar_url,
        "last_seen_at": last_seen_at,
    }


@database_sync_to_async
def is_conversation_participant(conversation_id, user):
    if user.is_anonymous:
        return False

    return Conversation.objects.filter(
        id=conversation_id,
        participants__user=user,
    ).exists()


@database_sync_to_async
def create_message(conversation_id, user, text, reply_to_id=None):
    text = text.strip()

    if not text:
        return None, "Message text cannot be empty."

    conversation = Conversation.objects.filter(
        id=conversation_id,
        participants__user=user,
    ).first()

    if not conversation:
        return None, "Conversation not found."

    reply_to_message = None

    if reply_to_id:
        reply_to_message = Message.objects.filter(
            id=reply_to_id,
            conversation=conversation,
            conversation__participants__user=user,
        ).select_related("sender").first()

        if not reply_to_message:
            return None, "Reply message not found."

    conversation.hidden_for.clear()

    message = Message.objects.create(
        conversation=conversation,
        sender=user,
        text=text,
        reply_to=reply_to_message,
    )

    message = (
        Message.objects.select_related(
            "sender",
            "reply_to",
            "reply_to__sender",
            "forwarded_from",
            "forwarded_from__sender",
        )
        .prefetch_related("reactions__user")
        .get(id=message.id)
    )

    return MessageSerializer(message).data, None


@database_sync_to_async
def update_user_last_seen(user):
    user.last_seen_at = timezone.now()
    user.save(update_fields=["last_seen_at"])
    return serialize_user_for_status(user)


class ChatConsumer(AsyncWebsocketConsumer):
    async def connect(self):
        self.user = self.scope["user"]
        self.conversation_id = self.scope["url_route"]["kwargs"]["conversation_id"]
        self.room_group_name = f"conversation_{self.conversation_id}"

        is_participant = await is_conversation_participant(
            self.conversation_id,
            self.user,
        )

        if not is_participant:
            await self.close(code=4003)
            return

        await self.channel_layer.group_add(
            self.room_group_name,
            self.channel_name,
        )

        await self.accept()

    async def disconnect(self, close_code):
        if hasattr(self, "room_group_name"):
            await self.channel_layer.group_discard(
                self.room_group_name,
                self.channel_name,
            )

    async def receive(self, text_data):
        try:
            data = json.loads(text_data)
        except json.JSONDecodeError:
            await self.send(
                text_data=json.dumps(
                    {
                        "type": "error",
                        "detail": "Invalid JSON.",
                    }
                )
            )
            return

        if data.get("type") == "typing":
            await self.channel_layer.group_send(
                self.room_group_name,
                {
                    "type": "typing_event",
                    "user": serialize_user_for_status(self.user),
                    "is_typing": bool(data.get("is_typing", False)),
                },
            )
            return

        if data.get("type") == "read":
            updated_count = await self.mark_messages_as_read()

            await self.channel_layer.group_send(
                self.room_group_name,
                {
                    "type": "read_event",
                    "user": serialize_user_for_status(self.user),
                    "updated_count": updated_count,
                },
            )
            return

        text = data.get("text", "")
        reply_to_id = data.get("reply_to")

        message_data, error = await create_message(
            self.conversation_id,
            self.user,
            text,
            reply_to_id,
        )

        if error:
            await self.send(
                text_data=json.dumps(
                    {
                        "type": "error",
                        "detail": error,
                    }
                )
            )
            return

        await self.channel_layer.group_send(
            self.room_group_name,
            {
                "type": "chat_message",
                "message": message_data,
            },
        )

        participant_ids = await self.get_conversation_participant_ids()

        for user_id in participant_ids:
            await self.channel_layer.group_send(
                f"user_{user_id}_notifications",
                {
                    "type": "sidebar_message",
                    "message": message_data,
                },
            )

    async def chat_message(self, event):
        await self.send(
            text_data=json.dumps(
                {
                    "type": "message",
                    "message": event["message"],
                }
            )
        )

    async def message_updated(self, event):
        await self.send(
            text_data=json.dumps(
                {
                    "type": "message_updated",
                    "message": event["message"],
                }
            )
        )

    async def typing_event(self, event):
        await self.send(
            text_data=json.dumps(
                {
                    "type": "typing",
                    "user": event["user"],
                    "is_typing": event["is_typing"],
                }
            )
        )

    async def read_event(self, event):
        await self.send(
            text_data=json.dumps(
                {
                    "type": "read",
                    "user": event["user"],
                    "updated_count": event["updated_count"],
                }
            )
        )

    @database_sync_to_async
    def mark_messages_as_read(self):
        return Message.objects.filter(
            conversation_id=self.conversation_id,
            is_read=False,
        ).exclude(sender=self.user).update(is_read=True)

    @database_sync_to_async
    def get_conversation_participant_ids(self):
        return list(
            ConversationParticipant.objects.filter(
                conversation_id=self.conversation_id
            ).values_list("user_id", flat=True)
        )


class NotificationConsumer(AsyncWebsocketConsumer):
    async def connect(self):
        self.user = self.scope["user"]

        if not self.user or self.user.is_anonymous:
            await self.close()
            return

        self.user_group_name = f"user_{self.user.id}_notifications"

        await self.channel_layer.group_add(
            self.user_group_name,
            self.channel_name,
        )

        await self.accept()

        partner_ids = await self.get_conversation_partner_ids()
        online_partner_ids = [
            partner_id
            for partner_id in partner_ids
            if ACTIVE_USER_CONNECTIONS.get(partner_id, 0) > 0
        ]

        await self.send(
            text_data=json.dumps(
                {
                    "type": "online_users",
                    "user_ids": online_partner_ids,
                }
            )
        )

        previous_connection_count = ACTIVE_USER_CONNECTIONS.get(self.user.id, 0)
        ACTIVE_USER_CONNECTIONS[self.user.id] = previous_connection_count + 1

        if previous_connection_count == 0:
            await self.broadcast_online_status_to_partners(True)

    async def disconnect(self, close_code):
        if hasattr(self, "user_group_name"):
            await self.channel_layer.group_discard(
                self.user_group_name,
                self.channel_name,
            )

        if hasattr(self, "user") and self.user and not self.user.is_anonymous:
            current_connection_count = ACTIVE_USER_CONNECTIONS.get(self.user.id, 0)

            if current_connection_count <= 1:
                ACTIVE_USER_CONNECTIONS.pop(self.user.id, None)
                user_data = await update_user_last_seen(self.user)
                await self.broadcast_online_status_to_partners(False, user_data)
            else:
                ACTIVE_USER_CONNECTIONS[self.user.id] = current_connection_count - 1

    @database_sync_to_async
    def get_conversation_partner_ids(self):
        conversation_ids = ConversationParticipant.objects.filter(
            user=self.user
        ).values_list("conversation_id", flat=True)

        return list(
            ConversationParticipant.objects.filter(
                conversation_id__in=conversation_ids
            )
            .exclude(user=self.user)
            .values_list("user_id", flat=True)
            .distinct()
        )

    async def broadcast_online_status_to_partners(self, is_online, user_data=None):
        partner_ids = await self.get_conversation_partner_ids()

        if user_data is None:
            user_data = serialize_user_for_status(self.user)

        for partner_id in partner_ids:
            await self.channel_layer.group_send(
                f"user_{partner_id}_notifications",
                {
                    "type": "online_status",
                    "user": user_data,
                    "is_online": is_online,
                },
            )

    async def sidebar_message(self, event):
        await self.send(
            text_data=json.dumps(
                {
                    "type": "sidebar_message",
                    "message": event["message"],
                }
            )
        )

    async def online_status(self, event):
        await self.send(
            text_data=json.dumps(
                {
                    "type": "online_status",
                    "user": event["user"],
                    "is_online": event["is_online"],
                }
            )
        )

    async def conversation_deleted(self, event):
        await self.send(
            text_data=json.dumps(
                {
                    "type": "conversation_deleted",
                    "conversation_id": event["conversation_id"],
                }
            )
        )

    async def user_profile_updated(self, event):
        await self.send(
            text_data=json.dumps(
                {
                    "type": "user_profile_updated",
                    "user": event["user"],
                }
            )
        )