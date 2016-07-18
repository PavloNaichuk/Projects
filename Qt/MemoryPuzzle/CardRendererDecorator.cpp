#include "CardRendererDecorator.h"

CardRendererDecorator::CardRendererDecorator(CardRenderer* cardRenderer)
    : mCardRenderer(cardRenderer)
{}

void CardRendererDecorator::Render(QPainter* painter, const Card* card)
{
    mCardRenderer->Render(painter, card);
}
