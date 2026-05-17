from django.urls import path

from .views import (
    ConversationDetailView,
    ConversationListView,
    ConversationMarkReadView,
    ConversationMessagesView,
    ConversationMuteView,
    MessageDetailView,
    MessageForwardView,
    MessageReactionToggleView,
)


urlpatterns = [
    path("conversations/", ConversationListView.as_view(), name="conversation-list"),
    path(
        "conversations/<int:conversation_id>/",
        ConversationDetailView.as_view(),
        name="conversation-detail",
    ),
    path(
        "conversations/<int:conversation_id>/messages/",
        ConversationMessagesView.as_view(),
        name="conversation-messages",
    ),
    path(
        "conversations/<int:conversation_id>/mark-read/",
        ConversationMarkReadView.as_view(),
        name="conversation-mark-read",
    ),
    path(
        "conversations/<int:conversation_id>/mute/",
        ConversationMuteView.as_view(),
        name="conversation-mute",
    ),
    path(
        "messages/<int:message_id>/",
        MessageDetailView.as_view(),
        name="message-detail",
    ),
    path(
        "messages/<int:message_id>/reactions/",
        MessageReactionToggleView.as_view(),
        name="message-reaction-toggle",
    ),
    path(
        "messages/<int:message_id>/forward/",
        MessageForwardView.as_view(),
        name="message-forward",
    ),
]