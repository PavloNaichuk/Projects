#ifndef OPENEDCARDRENDERER_H
#define OPENEDCARDRENDERER_H

#include "CardRenderer.h"

class OpenedCardRenderer : public CardRenderer
{
public:
    void Render(QPainter* painter, const Card* card);
};

#endif // OPENEDCARDRENDERER_H
