from django.contrib import admin

from .models import Conversation, ConversationParticipant, Message


@admin.register(Conversation)
class ConversationAdmin(admin.ModelAdmin):
    list_display = ("id", "created_at", "updated_at")


@admin.register(ConversationParticipant)
class ConversationParticipantAdmin(admin.ModelAdmin):
    list_display = ("id", "conversation", "user", "joined_at")


@admin.register(Message)
class MessageAdmin(admin.ModelAdmin):
    list_display = (
        "id",
        "conversation",
        "sender",
        "text",
        "created_at",
        "updated_at",
        "edited_at",
        "is_deleted",
        "deleted_at",
        "is_read",
    )
    list_filter = (
        "is_read",
        "is_deleted",
        "created_at",
        "updated_at",
        "edited_at",
        "deleted_at",
    )
    search_fields = ("text", "sender__username", "sender__email")
    readonly_fields = (
        "created_at",
        "updated_at",
        "edited_at",
        "is_deleted",
        "deleted_at",
    )