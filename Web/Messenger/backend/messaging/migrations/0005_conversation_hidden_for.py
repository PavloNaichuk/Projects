from django.conf import settings
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
        ("messaging", "0004_message_deleted_at_message_is_deleted"),
    ]

    operations = [
        migrations.AddField(
            model_name="conversation",
            name="hidden_for",
            field=models.ManyToManyField(
                blank=True,
                related_name="hidden_conversations",
                to=settings.AUTH_USER_MODEL,
            ),
        ),
    ]
