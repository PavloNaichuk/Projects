FROM python:3.10-slim AS runtime

ENV PYTHONDONTWRITEBYTECODE=1 \
    PYTHONUNBUFFERED=1 \
    PIP_DISABLE_PIP_VERSION_CHECK=1 \
    PIP_NO_CACHE_DIR=1

WORKDIR /app

RUN addgroup --system appgroup \
    && adduser --system --ingroup appgroup --home /app appuser

COPY requirements.txt ./
RUN python -m pip install --upgrade pip \
    && python -m pip install -r requirements.txt

COPY --chown=appuser:appgroup alembic ./alembic
COPY --chown=appuser:appgroup alembic.ini pyproject.toml ./
COPY --chown=appuser:appgroup app ./app
COPY --chown=appuser:appgroup scripts ./scripts

USER appuser

EXPOSE 8000

CMD ["python", "-m", "uvicorn", "app.main:app", "--host", "0.0.0.0", "--port", "8000"]
