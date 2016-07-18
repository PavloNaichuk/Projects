#ifndef CARDRENDERERDECORATOR_H
#define CARDRENDERERDECORATOR_H

#include "CardRenderer.h"

class CardRendererDecorator : public CardRenderer
{
public:
    CardRendererDecorator(CardRenderer* cardRenderer);
    void Render(QPainter* painter, const Card* card);

private:
    CardRenderer* mCardRenderer;
};

#endif // CARDRENDERERDECORATOR_H
