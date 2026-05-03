#include "SelectedCardRenderer.h"
#include "Card.h"
#include <QPainter>

SelectedCardRenderer::SelectedCardRenderer(CardRenderer* cardRenderer)
    : CardRendererDecorator(cardRenderer)
{
}

void SelectedCardRenderer::Render(QPainter* painter, const Card* card)
{
    CardRendererDecorator::Render(painter, card);
    const QRect& cardRect = card->getRect();
    painter->drawRect(cardRect.x(), cardRect.y(), cardRect.width(), cardRect.height());
}
