from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ("messaging", "0006_message_attachments"),
    ]

    operations = [
        migrations.AddField(
            model_name="message",
            name="reply_to",
            field=models.ForeignKey(
                blank=True,
                null=True,
                on_delete=models.SET_NULL,
                related_name="replies",
                to="messaging.message",
            ),
        ),
    ]