#include "OpenedCardRenderer.h"
#include "Card.h"
#include <QPainter>

void OpenedCardRenderer::Render(QPainter* painter, const Card* card)
{
    const QRect& cardRect = card->getRect();
    painter->fillRect(cardRect, Qt::white);
    painter->drawText(cardRect, Qt::AlignCenter, QString(card->getValue()));
}
