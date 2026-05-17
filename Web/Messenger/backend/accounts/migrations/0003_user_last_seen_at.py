from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ("accounts", "0002_user_avatar"),
    ]

    operations = [
        migrations.AddField(
            model_name="user",
            name="last_seen_at",
            field=models.DateTimeField(blank=True, null=True),
        ),
    ]