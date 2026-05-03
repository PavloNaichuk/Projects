#ifndef CARDBOARD_H
#define CARDBOARD_H

#include <QFile>
#include "Card.h"

class CardBoard
{
public:
    CardBoard(const QString& fileName);
    CardBoard(const Card* firstCard);

    void save(const QString& fileName) const;
    void load(const QString& fileName);

    enum
    {
        kNumRows = 4,
        kNumCols = 4
    };

    Card& GetCard(int row, int col);
    const Card& GetCard(int row, int col) const;

    void resizeBoard(int boardWidth, int boardHeight);

private:
    Card mCards[kNumRows][kNumCols];
};

#endif // CARDBOARD_H
