from celery import shared_task
from django.conf import settings
from django.core.mail import send_mail

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
