from django.urls import path

from .views import CurrentUserView, LogoutView, UserRegistrationView, UserSearchView


urlpatterns = [
    path("auth/register/", UserRegistrationView.as_view(), name="user-register"),
    path("auth/logout/", LogoutView.as_view(), name="logout"),
    path("auth/me/", CurrentUserView.as_view(), name="current-user"),
    path("users/search/", UserSearchView.as_view(), name="user-search"),
]