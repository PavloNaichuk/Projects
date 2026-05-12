from django.urls import path

from .consumers import ChatConsumer, NotificationConsumer

urlpatterns = [
    path("ws/conversations/<int:conversation_id>/", ChatConsumer.as_asgi()),
    path("ws/notifications/", NotificationConsumer.as_asgi()),
]