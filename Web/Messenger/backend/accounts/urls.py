from django.urls import path

from .views import (
    ContactNicknameView,
    CurrentUserView,
    EmailVerificationConfirmView,
    EmailVerificationResendView,
    LogoutView,
    PasswordResetConfirmView,
    PasswordResetRequestView,
    UserAvatarView,
    UserBlockedListView,
    UserBlockView,
    UserProfileView,
    UserRegistrationView,
    UserSearchView,
    UserUnblockView,
)

urlpatterns = [
    path("auth/register/", UserRegistrationView.as_view(), name="user-register"),
    path("auth/logout/", LogoutView.as_view(), name="logout"),
    path(
        "auth/password-reset/request/",
        PasswordResetRequestView.as_view(),
        name="password-reset-request",
    ),
    path(
        "auth/password-reset/confirm/",
        PasswordResetConfirmView.as_view(),
        name="password-reset-confirm",
    ),
    path(
        "auth/email/verify/",
        EmailVerificationConfirmView.as_view(),
        name="email-verify",
    ),
    path(
        "auth/email/resend/",
        EmailVerificationResendView.as_view(),
        name="email-resend",
    ),
    path("auth/me/", CurrentUserView.as_view(), name="current-user"),
    path("auth/me/avatar/", UserAvatarView.as_view(), name="user-avatar"),
    path("auth/me/profile/", UserProfileView.as_view(), name="user-profile"),
    path("users/blocked/", UserBlockedListView.as_view(), name="blocked-users"),
    path(
        "users/<int:user_id>/nickname/",
        ContactNicknameView.as_view(),
        name="contact-nickname",
    ),
    path(
        "users/<int:user_id>/block/",
        UserBlockView.as_view(),
        name="user-block",
    ),
    path(
        "users/<int:user_id>/unblock/",
        UserUnblockView.as_view(),
        name="user-unblock",
    ),
    path("users/search/", UserSearchView.as_view(), name="user-search"),
]
