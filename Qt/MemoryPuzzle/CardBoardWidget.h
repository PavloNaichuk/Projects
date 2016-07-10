#ifndef CARDBOARDWIDGET_H
#define CARDBOARDWIDGET_H

#include <QWidget>
#include <QTimer>

class GameLogic;
class CardBoard;

class CardBoardWidget : public QWidget
{
    Q_OBJECT

public:
    CardBoardWidget(QWidget* parent = 0);
    ~CardBoardWidget();

    enum
    {
        Width = 600,
        Height = 400
    };

    const CardBoard* getCardBoard() const;
    void setCardBoard(CardBoard* cardBoard);
    void paintEvent(QPaintEvent* paintEvent);

protected:
    virtual void resizeEvent(QResizeEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);

public slots:
    void updateGameLogic();

private:
    GameLogic* mGameLogic;
    QTimer* mTimer;
};

#endif // CARDBOARDWIDGET_H
