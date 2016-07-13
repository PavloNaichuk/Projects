#include "TempOpenedCardRenderer.h"
#include "Card.h"
#include <QPainter>

void TempOpenedCardRenderer::Render(QPainter* painter, const Card* card)
{
    const QRect& cardRect = card->getRect();
    painter->drawRect(cardRect.x(), cardRect.y(), cardRect.width(), cardRect.height());
    painter->fillRect(cardRect, Qt::white);
    painter->drawText(cardRect, Qt::AlignCenter, QString(card->getValue()));
}
