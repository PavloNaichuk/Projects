#ifndef MAINWIDGET_H
#define MAINWIDGET_H

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

signals:
    void gameMenuRequested();

private slots:
    void levelUpdated();
    void scoreUpdated(int);
    void timeUpdated(int);

private:
    void keyPressEvent(QKeyEvent* event);

private:
    CardBoardWidget* mCardBoardWidget;
    GameLogic* mGameLogic;

    QLabel* mLevelLabel;
    QLabel* mScoreLabel;
    QLabel* mTimeLabel;
};

#endif // MAINWIDGET_H
