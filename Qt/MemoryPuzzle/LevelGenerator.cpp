#include "LevelGenerator.h"
#include "CardBoard.h"
#include "CardBoardWidget.h"
#include <string>

Card* LevelGenerator::create()
{
    const int numValues = CardBoard::kNumRows * CardBoard::kNumCols;
    const int numUniqueValues = CardBoard::kNumRows * CardBoard::kNumCols / 2;

    std::string allValues = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::random_shuffle(std::begin(allValues), std::end(allValues));

    char cardValues[numValues];
    for (int index = 0; index < numUniqueValues; ++index)
    {
       cardValues[2 * index + 0] = allValues[index];
       cardValues[2 * index + 1] = allValues[index];
    }
    std::random_shuffle(std::begin(cardValues), std::end(cardValues));

    Card* cards = new Card[numValues];

    const int cellWidth = CardBoardWidget::Width / CardBoard::kNumCols;
    const int cellHeight = CardBoardWidget::Height / CardBoard::kNumRows;

    const int offset = 6;
    const int cardWidth = cellWidth - 2 * offset;
    const int cardHeight = cellHeight - 2 * offset;

    int index = 0;
    for (int row = 0; row < CardBoard::kNumRows; ++row)
    {
        for (int col = 0; col < CardBoard::kNumCols; ++col)
        {
            QRect cardRect(col * cellWidth + offset, row * cellHeight + offset, cardWidth, cardHeight);
            QColor color = (index % 2 != 0) ? Qt::yellow : Qt::blue;

            cards[index] = Card(color, cardValues[index], cardRect, false);
            ++index;
        }
    }
    return cards;
}
