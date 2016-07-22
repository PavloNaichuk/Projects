#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>

class Card;
class CardBoard;
class GameState;
class SelectFirstCardState;
class SelectSecondCardState;
class WaitBeforeClosingState;

class GameLogic : public QObject
{
    Q_OBJECT

public:
    GameLogic();
    ~GameLogic();

    CardBoard* getCardBoard();
    void setCardBoard(CardBoard* cardBoard);

    int getCardBoardWidth() const;
    void setCardBoardWidth(int cardBoardWidth);

    int getCardBoardHeight() const;
    void setCardBoardHeight(int cardBoardHeight);

    bool redrawBoard() const;
    void setRedrawBoard(bool redrawBoard);

    Card* getSelectedFirstCard();
    void setSelectedFirstCard(Card* card);

    Card* getSelectedSecondCard();
    void setSelectedSecondCard(Card* card);

    GameState* getCurrentState();
    void setCurrentState(GameState* nextState);

    SelectFirstCardState* getSelectFirstCardState();
    SelectSecondCardState* getSelectSecondCardState();
    WaitBeforeClosingState* getWaitBeforeClosingState();

    int getTotalScore() const;

signals:
    void scoreChanged();

private slots:
    void selectionCompleted(bool success);

private:
    int calcScore() const;

    CardBoard* mCardBoard;
    int mCardBoardWidth;
    int mCardBoardHeight;

    bool mRedrawBoard;

    Card* mSelectedFirstCard;
    Card* mSelectedSecondCard;

    SelectFirstCardState* mSelectFirstCardState;
    SelectSecondCardState* mSelectSecondCardState;
    WaitBeforeClosingState* mWaitBeforeClosingState;
    GameState* mCurrentState;
    int mTotalScore;
    unsigned mNumAttempts;
};

#endif // GAMELOGIC_H
