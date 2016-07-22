#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QTime>
#include <QTimer>
#include <QLabel>

class GameLogic;
class CardBoard;
class CardBoardWidget;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(GameLogic* gameLogic, QWidget* parent = 0);
    ~MainWidget();

    void setCardBoard(CardBoard* cardBoard);
    const CardBoard* getCardBoard() const;

private slots:
    void updateTime();
    void updateScore();

private:
    GameLogic* mGameLogic;
    QTime* mTime;
    QTimer* mTimer;
    CardBoardWidget* mCardBoardWidget;
    QLabel* mLevelLabel;
    QLabel* mScoreLabel;
    QLabel* mTimeLabel;
};

#endif // MAINWIDGET_H
