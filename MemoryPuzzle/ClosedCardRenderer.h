#ifndef CLOSEDCARDRENDERER_H
#define CLOSEDCARDRENDERER_H

#include "CardRenderer.h"

class ClosedCardRenderer : public CardRenderer
{
public:
    void Render(QPainter* painter, const Card* card);
};

#endif // CLOSEDCARDRENDERER_H
