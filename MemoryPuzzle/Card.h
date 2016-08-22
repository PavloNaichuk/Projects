#ifndef CARD_H
#define CARD_H

#include <QColor>
#include <QRect>

class Card
{
public:
    enum State
    {
        Closed,
        Selected,
        Opened
    };

    Card();
    Card(const QColor& color, const char value, const QRect& rect, State state);

    void setColor(const QColor& color);
    const QColor& getColor() const;

    void setValue(char value);
    char getValue() const;

    void setRect(const QRect& rect);
    const QRect& getRect() const;

    void setState(State state);
    State getState() const;

private:
    QColor mColor;
    char mValue;
    QRect mRect;
    State mState;
};

#endif // CARD_H
