#include "Card.h"

Card::Card()
    : mColor(Qt::black)
    , mValue('\0')
    , mIsOpened(false)
{}

Card::Card(const QColor& color, const char value, const QRect& rect, const bool isOpened)
    : mColor(color)
    , mValue(value)
    , mRect(rect)
    , mIsOpened(isOpened)
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

void Card::setOpened(bool isOpened)
{
    mIsOpened = isOpened;
}
bool Card::isOpened() const
{
    return mIsOpened;
}

