from django.urls import path

from .views import CurrentUserView, UserSearchView


urlpatterns = [
    path("auth/me/", CurrentUserView.as_view(), name="current-user"),
    path("users/search/", UserSearchView.as_view(), name="user-search"),
]