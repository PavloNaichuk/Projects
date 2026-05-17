from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ("messaging", "0009_message_forwarded_from"),
    ]

    operations = [
        migrations.AddField(
            model_name="message",
            name="is_delivered",
            field=models.BooleanField(default=False),
        ),
        migrations.AddField(
            model_name="message",
            name="delivered_at",
            field=models.DateTimeField(blank=True, null=True),
        ),
    ]