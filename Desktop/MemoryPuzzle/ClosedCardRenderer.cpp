#include "ClosedCardRenderer.h"
#include "Card.h"
#include <QPainter>

void ClosedCardRenderer::Render(QPainter* painter, const Card* card)
{
     painter->fillRect(card->getRect(), card->getColor());
}
