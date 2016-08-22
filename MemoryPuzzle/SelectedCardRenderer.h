#ifndef SELECTEDCARDRENDERER_H
#define SELECTEDCARDRENDERER_H

#include "CardRendererDecorator.h"

class CardRenderer;

class SelectedCardRenderer : public CardRendererDecorator
{
public:
    SelectedCardRenderer(CardRenderer* cardRenderer);
    void Render(QPainter* painter, const Card* card);
};

#endif // SELECTEDCARDRENDERER_H
