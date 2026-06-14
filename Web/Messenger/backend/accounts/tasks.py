from celery import shared_task
from django.conf import settings
from django.contrib.auth import get_user_model
from django.contrib.auth.tokens import default_token_generator
from django.core.mail import send_mail
from django.utils.encoding import force_bytes
from django.utils.http import urlsafe_base64_encode

from .models import EmailVerificationToken


@shared_task
def debug_celery_task():
    return "Celery is working."


@shared_task(
    autoretry_for=(Exception,),
    retry_backoff=True,
    retry_kwargs={"max_retries": 3},
)
def send_email_verification_email_task(token_id: int) -> str:
    verification_token = (
        EmailVerificationToken.objects.select_related("user")
        .filter(id=token_id)
        .first()
    )

    if not verification_token:
        return "Skipped missing email verification token."

    if verification_token.is_used:
        return "Skipped used email verification token."

    if verification_token.is_expired:
        return "Skipped expired email verification token."

    user = verification_token.user

    if user.is_email_verified:
        return "Skipped already verified user."

    verification_url = (
        f"{settings.FRONTEND_URL}/verify-email?token={verification_token.token}"
    )

    send_mail(
        subject="Verify your Messenger email",
        message=(
            "Welcome to Messenger!\n\n"
            "Please verify your email address by opening this link:\n"
            f"{verification_url}\n\n"
            "If you did not create this account, you can ignore this email."
        ),
        from_email=settings.DEFAULT_FROM_EMAIL,
        recipient_list=[user.email],
        fail_silently=False,
    )

    return f"Email verification sent for user {user.id}."


@shared_task(
    autoretry_for=(Exception,),
    retry_backoff=True,
    retry_kwargs={"max_retries": 3},
)
def send_password_reset_email_task(user_id: int) -> str:
    User = get_user_model()
    user = User.objects.filter(id=user_id).first()

    if not user:
        return "Skipped missing password reset user."

    uid = urlsafe_base64_encode(force_bytes(user.pk))
    token = default_token_generator.make_token(user)
    reset_url = f"{settings.FRONTEND_URL}/reset-password?uid={uid}&token={token}"

    send_mail(
        subject="Reset your Messenger password",
        message=(
            "You requested a password reset for your Messenger account.\n\n"
            "Open this link to choose a new password:\n"
            f"{reset_url}\n\n"
            "If you did not request this, you can ignore this email."
        ),
        from_email=settings.DEFAULT_FROM_EMAIL,
        recipient_list=[user.email],
        fail_silently=False,
    )

    return f"Password reset email sent for user {user.id}."

