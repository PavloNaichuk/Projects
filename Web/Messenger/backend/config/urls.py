from django.conf import settings
from django.conf.urls.static import static
from django.contrib import admin
from django.urls import include, path
from accounts.views import LoginView, TokenRefreshAPIView


urlpatterns = [
    path("admin/", admin.site.urls),

    path("api/", include("messaging.urls")),
    path("api/", include("accounts.urls")),

    path("api/auth/token/", LoginView.as_view(), name="token_obtain_pair"),
    path("api/auth/token/refresh/", TokenRefreshAPIView.as_view(), name="token_refresh"),
]

if settings.DEBUG:
    urlpatterns += static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)