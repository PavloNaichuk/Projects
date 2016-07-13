#ifndef TEMPOPENEDCARDRENDERER_H
#define TEMPOPENEDCARDRENDERER_H

#include "CardRenderer.h"

class TempOpenedCardRenderer : public CardRenderer
{
public:
    void Render(QPainter* painter, const Card* card);
};

#endif // TEMPOPENEDCARDRENDERER_H
