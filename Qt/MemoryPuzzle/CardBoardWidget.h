#ifndef CARDBOARDWIDGET_H
#define CARDBOARDWIDGET_H

#include "CardBoard.h"
#include <QWidget>

class CardBoardWidget : public QWidget
{
    Q_OBJECT

public:
    CardBoardWidget(QWidget* parent = 0);

    enum
    {
        Width = 600,
        Height = 400
    };

    const CardBoard* getCardBoard() const;
    void setCardBoard(CardBoard* cardBoard);

    void paintEvent(QPaintEvent* paintEvent);

private:
    CardBoard* mCardBoard;

protected:
    virtual void resizeEvent(QResizeEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
};

#endif // CARDBOARDWIDGET_H
