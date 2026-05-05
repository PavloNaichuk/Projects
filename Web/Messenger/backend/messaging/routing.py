from django.urls import path

from .consumers import ChatConsumer


urlpatterns = [
    path(
        "ws/conversations/<int:conversation_id>/",
        ChatConsumer.as_asgi(),
        name="conversation-websocket",
    ),
]