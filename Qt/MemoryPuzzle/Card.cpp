#include "Card.h"

Card::Card()
    : mColor(Qt::black)
    , mValue('\0')
    , mState(Closed)
{}

Card::Card(const QColor& color, const char value, const QRect& rect, Card::State state)
    : mColor(color)
    , mValue(value)
    , mRect(rect)
    , mState(state)
{
}

void Card::setColor(const QColor& color)
{
    mColor = color;
}

const QColor& Card::getColor() const
{
    return mColor;
}

void Card::setValue(char value)
{
    mValue = value;
}

char Card::getValue() const
{
    return mValue;
}

void Card::setRect(const QRect& rect)
{
    mRect = rect;
}

const QRect& Card::getRect() const
{
    return mRect;
}

void Card::setState(State state)
{
    mState = state;
}
Card::State Card::getState() const
{
    return mState;
}

