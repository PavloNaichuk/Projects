from django.conf import settings
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
        ("messaging", "0010_message_delivery_status"),
    ]

    operations = [
        migrations.AddField(
            model_name="conversation",
            name="muted_for",
            field=models.ManyToManyField(
                blank=True,
                related_name="muted_conversations",
                to=settings.AUTH_USER_MODEL,
            ),
        ),
    ]