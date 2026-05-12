import json

from channels.db import database_sync_to_async
from channels.generic.websocket import AsyncWebsocketConsumer
from channels.db import database_sync_to_async

from .models import Conversation, Message
from .serializers import MessageSerializer
from .models import Message

@database_sync_to_async
def is_conversation_participant(conversation_id, user):
    if user.is_anonymous:
        return False

    return Conversation.objects.filter(
        id=conversation_id,
        participants__user=user,
    ).exists()


@database_sync_to_async
def create_message(conversation_id, user, text):
    text = text.strip()

    if not text:
        return None, "Message text cannot be empty."

    conversation = Conversation.objects.filter(
        id=conversation_id,
        participants__user=user,
    ).first()

    if not conversation:
        return None, "Conversation not found."

    message = Message.objects.create(
        conversation=conversation,
        sender=user,
        text=text,
    )

    return MessageSerializer(message).data, None


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
                    "user": {
                        "id": self.user.id,
                        "username": self.user.username,
                        "email": self.user.email,
                    },
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
                    "user": {
                        "id": self.user.id,
                        "username": self.user.username,
                        "email": self.user.email,
                    },
                    "updated_count": updated_count,
                },
            )
            return
        
        text = data.get("text", "")

        message_data, error = await create_message(
            self.conversation_id,
            self.user,
            text,
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

    async def chat_message(self, event):
        await self.send(
            text_data=json.dumps(
                {
                    "type": "message",
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