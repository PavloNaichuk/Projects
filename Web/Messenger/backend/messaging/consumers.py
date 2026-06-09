import json

from channels.db import database_sync_to_async
from channels.generic.websocket import AsyncWebsocketConsumer
from django.utils import timezone

from accounts.models import BlockedUser

from .models import Conversation, ConversationParticipant, Message
from .serializers import MessageSerializer

ACTIVE_USER_CONNECTIONS = {}


def is_user_online(user_id):
    return ACTIVE_USER_CONNECTIONS.get(user_id, 0) > 0


def add_active_user_connection(user_id):
    previous_connection_count = ACTIVE_USER_CONNECTIONS.get(user_id, 0)
    ACTIVE_USER_CONNECTIONS[user_id] = previous_connection_count + 1

    return previous_connection_count


def remove_active_user_connection(user_id):
    current_connection_count = ACTIVE_USER_CONNECTIONS.get(user_id, 0)

    if current_connection_count <= 1:
        ACTIVE_USER_CONNECTIONS.pop(user_id, None)
        return 0

    ACTIVE_USER_CONNECTIONS[user_id] = current_connection_count - 1
    return ACTIVE_USER_CONNECTIONS[user_id]


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


def get_conversation_other_user(conversation, current_user):
    participant = (
        conversation.participants.exclude(user=current_user)
        .select_related("user")
        .first()
    )

    if not participant:
        return None

    return participant.user


def get_blocking_error_detail(sender, receiver):
    if not receiver:
        return ""

    if BlockedUser.objects.filter(blocker=sender, blocked=receiver).exists():
        return "You blocked this user. Unblock them to send messages."

    if BlockedUser.objects.filter(blocker=receiver, blocked=sender).exists():
        return "You cannot send messages to this user."

    return ""


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

    other_user = get_conversation_other_user(conversation, user)
    blocking_error = get_blocking_error_detail(user, other_user)

    if blocking_error:
        return None, blocking_error

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


@database_sync_to_async
def mark_message_as_delivered_for_user(message_id, user_id):
    message = (
        Message.objects.filter(
            id=message_id,
            conversation__participants__user_id=user_id,
            is_delivered=False,
            is_deleted=False,
        )
        .exclude(sender_id=user_id)
        .first()
    )

    if not message:
        return None

    message.is_delivered = True
    message.delivered_at = timezone.now()
    message.save(update_fields=["is_delivered", "delivered_at", "updated_at"])

    return {
        "conversation_id": message.conversation_id,
        "message_ids": [message.id],
        "user_id": user_id,
    }


@database_sync_to_async
def mark_undelivered_messages_as_delivered_for_user(user):
    messages = (
        Message.objects.filter(
            conversation__participants__user=user,
            is_delivered=False,
            is_deleted=False,
        )
        .exclude(sender=user)
        .values("id", "conversation_id")
    )

    messages_by_conversation = {}

    for message in messages:
        conversation_id = message["conversation_id"]

        if conversation_id not in messages_by_conversation:
            messages_by_conversation[conversation_id] = []

        messages_by_conversation[conversation_id].append(message["id"])

    if not messages_by_conversation:
        return []

    message_ids = [
        message_id
        for conversation_message_ids in messages_by_conversation.values()
        for message_id in conversation_message_ids
    ]

    Message.objects.filter(id__in=message_ids).update(
        is_delivered=True,
        delivered_at=timezone.now(),
    )

    return [
        {
            "conversation_id": conversation_id,
            "message_ids": conversation_message_ids,
            "user_id": user.id,
        }
        for conversation_id, conversation_message_ids in (
            messages_by_conversation.items()
        )
    ]


class JsonWebsocketConsumer(AsyncWebsocketConsumer):
    async def send_json_payload(self, payload):
        await self.send(text_data=json.dumps(payload))

    async def send_group_event(self, group_name, payload):
        await self.channel_layer.group_send(group_name, payload)


class ChatConsumer(JsonWebsocketConsumer):
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
            await self.send_json_payload(
                {
                    "type": "error",
                    "detail": "Invalid JSON.",
                }
            )
            return

        if data.get("type") == "typing":
            is_blocked = await self.is_conversation_blocked()

            if is_blocked:
                return

            await self.send_group_event(
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

            await self.send_group_event(
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
            await self.send_json_payload(
                {
                    "type": "error",
                    "detail": error,
                }
            )
            return

        await self.send_group_event(
            self.room_group_name,
            {
                "type": "chat_message",
                "message": message_data,
            },
        )

        participant_ids = await self.get_conversation_participant_ids()

        for user_id in participant_ids:
            await self.send_group_event(
                f"user_{user_id}_notifications",
                {
                    "type": "sidebar_message",
                    "message": message_data,
                },
            )

            if user_id != self.user.id and is_user_online(user_id):
                delivery_data = await mark_message_as_delivered_for_user(
                    message_data["id"],
                    user_id,
                )

                if delivery_data:
                    await self.send_group_event(
                        f"conversation_{delivery_data['conversation_id']}",
                        {
                            "type": "delivery_event",
                            "message_ids": delivery_data["message_ids"],
                            "user_id": delivery_data["user_id"],
                        },
                    )

    async def chat_message(self, event):
        await self.send_json_payload(
            {
                "type": "message",
                "message": event["message"],
            }
        )

    async def message_updated(self, event):
        await self.send_json_payload(
            {
                "type": "message_updated",
                "message": event["message"],
            }
        )

    async def delivery_event(self, event):
        await self.send_json_payload(
            {
                "type": "delivered",
                "message_ids": event["message_ids"],
                "user_id": event["user_id"],
            }
        )

    async def typing_event(self, event):
        await self.send_json_payload(
            {
                "type": "typing",
                "user": event["user"],
                "is_typing": event["is_typing"],
            }
        )

    async def read_event(self, event):
        await self.send_json_payload(
            {
                "type": "read",
                "user": event["user"],
                "updated_count": event["updated_count"],
            }
        )

    @database_sync_to_async
    def is_conversation_blocked(self):
        conversation = (
            Conversation.objects.filter(
                id=self.conversation_id,
                participants__user=self.user,
            )
            .prefetch_related("participants__user")
            .first()
        )

        if not conversation:
            return True

        other_user = get_conversation_other_user(conversation, self.user)

        return bool(get_blocking_error_detail(self.user, other_user))

    @database_sync_to_async
    def mark_messages_as_read(self):
        messages = Message.objects.filter(
            conversation_id=self.conversation_id,
            is_read=False,
        ).exclude(sender=self.user)

        now = timezone.now()

        messages.filter(is_delivered=False).update(
            is_delivered=True,
            delivered_at=now,
        )

        return messages.update(is_read=True)

    @database_sync_to_async
    def get_conversation_participant_ids(self):
        return list(
            ConversationParticipant.objects.filter(
                conversation_id=self.conversation_id
            ).values_list("user_id", flat=True)
        )


class NotificationConsumer(JsonWebsocketConsumer):
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

        delivery_events = await mark_undelivered_messages_as_delivered_for_user(
            self.user
        )

        for delivery_event_data in delivery_events:
            await self.send_group_event(
                f"conversation_{delivery_event_data['conversation_id']}",
                {
                    "type": "delivery_event",
                    "message_ids": delivery_event_data["message_ids"],
                    "user_id": delivery_event_data["user_id"],
                },
            )

        partner_ids = await self.get_conversation_partner_ids()
        online_partner_ids = [
            partner_id
            for partner_id in partner_ids
            if is_user_online(partner_id)
        ]

        await self.send_json_payload(
            {
                "type": "online_users",
                "user_ids": online_partner_ids,
            }
        )

        previous_connection_count = add_active_user_connection(self.user.id)

        if previous_connection_count == 0:
            await self.broadcast_online_status_to_partners(True)

    async def disconnect(self, close_code):
        if hasattr(self, "user_group_name"):
            await self.channel_layer.group_discard(
                self.user_group_name,
                self.channel_name,
            )

        if hasattr(self, "user") and self.user and not self.user.is_anonymous:
            active_connection_count = remove_active_user_connection(self.user.id)

            if active_connection_count == 0:
                user_data = await update_user_last_seen(self.user)
                await self.broadcast_online_status_to_partners(False, user_data)

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
            await self.send_group_event(
                f"user_{partner_id}_notifications",
                {
                    "type": "online_status",
                    "user": user_data,
                    "is_online": is_online,
                },
            )

    async def sidebar_message(self, event):
        await self.send_json_payload(
            {
                "type": "sidebar_message",
                "message": event["message"],
            }
        )

    async def online_status(self, event):
        await self.send_json_payload(
            {
                "type": "online_status",
                "user": event["user"],
                "is_online": event["is_online"],
            }
        )

    async def conversation_deleted(self, event):
        await self.send_json_payload(
            {
                "type": "conversation_deleted",
                "conversation_id": event["conversation_id"],
            }
        )

    async def user_profile_updated(self, event):
        await self.send_json_payload(
            {
                "type": "user_profile_updated",
                "user": event["user"],
            }
        )
