#ifndef CARDRENDERER_H
#define CARDRENDERER_H

class Card;
class QPainter;

class CardRenderer
{
public:
    virtual ~CardRenderer() {}
    virtual void Render(QPainter* painter, const Card* card) = 0;
};

#endif // CARDRENDERER_H
