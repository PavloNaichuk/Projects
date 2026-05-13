from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ("messaging", "0005_conversation_hidden_for"),
    ]

    operations = [
        migrations.AlterField(
            model_name="message",
            name="text",
            field=models.TextField(blank=True),
        ),
        migrations.AddField(
            model_name="message",
            name="attachment",
            field=models.FileField(
                blank=True,
                null=True,
                upload_to="messages/attachments/",
            ),
        ),
        migrations.AddField(
            model_name="message",
            name="attachment_name",
            field=models.CharField(blank=True, max_length=255),
        ),
        migrations.AddField(
            model_name="message",
            name="attachment_content_type",
            field=models.CharField(blank=True, max_length=100),
        ),
        migrations.AddField(
            model_name="message",
            name="attachment_size",
            field=models.PositiveIntegerField(blank=True, null=True),
        ),
    ]