from django.urls import path

from .views import (
    ContactNicknameView,
    CurrentUserView,
    LogoutView,
    UserAvatarView,
    UserBlockView,
    UserProfileView,
    UserRegistrationView,
    UserSearchView,
    UserUnblockView,
)


urlpatterns = [
    path("auth/register/", UserRegistrationView.as_view(), name="user-register"),
    path("auth/logout/", LogoutView.as_view(), name="logout"),
    path("auth/me/", CurrentUserView.as_view(), name="current-user"),
    path("auth/me/avatar/", UserAvatarView.as_view(), name="user-avatar"),
    path("auth/me/profile/", UserProfileView.as_view(), name="user-profile"),
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
