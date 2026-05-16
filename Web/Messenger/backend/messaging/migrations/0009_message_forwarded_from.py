from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ("messaging", "0008_message_reaction"),
    ]

    operations = [
        migrations.AddField(
            model_name="message",
            name="forwarded_from",
            field=models.ForeignKey(
                blank=True,
                null=True,
                on_delete=django.db.models.deletion.SET_NULL,
                related_name="forwarded_messages",
                to="messaging.message",
            ),
        ),
    ]