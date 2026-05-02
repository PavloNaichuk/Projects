from django.urls import path

from .views import UserSearchView


urlpatterns = [
    path("users/search/", UserSearchView.as_view(), name="user-search"),
]