#ifndef CARD_H
#define CARD_H

#include <QColor>
#include <QRect>

class Card
{
public:
    Card();
    Card(const QColor& color, const char value, const QRect& rect, const bool isOpened);

    void setColor(const QColor& color);
    const QColor& getColor() const;

    void setValue(char value);
    char getValue() const;

    void setRect(const QRect& rect);
    const QRect& getRect() const;

    void setOpened(bool isOpened);
    bool isOpened() const;

private:
    QColor mColor;
    char mValue;
    QRect mRect;
    bool mIsOpened;
};

#endif // CARD_H
