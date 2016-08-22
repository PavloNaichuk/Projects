#include "CardBoard.h"
#include <QTextStream>
#include <QString>

CardBoard::CardBoard(const QString& fileName)
{
    load(fileName);
}

CardBoard::CardBoard(const Card* firstCard)
{
    for (int row = 0; row < kNumRows; ++row)
    {
        for (int col = 0; col < kNumCols; ++col)
        {
            mCards[row][col] = *firstCard;
            ++firstCard;
        }
    }
}

void CardBoard::save(const QString& fileName) const
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for (int row = 0; row < kNumRows; ++row)
    {
        for (int col = 0; col < kNumCols; ++col)
        {
            const Card& card = mCards[row][col];
            QString color = card.getColor().name();
            char value = card.getValue();
            const QRect& rect = card.getRect();
            int state = card.getState();

            out << color << ';' << value << ';' << rect.x() << ',' << rect.y() << ','<< rect.width() << ',' << rect.height() << ';' << state << ";\n";
        }
    }
}

void CardBoard::load(const QString& fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        int row = 0, col = 0;

        QTextStream textStream(&file);
        while (!textStream.atEnd())
        {
            QString line = textStream.readLine();
            if (col == kNumCols)
            {
                col = 0;
                ++row;
            }

            QStringList stringList = line.split(';');
            QStringList stringRect = stringList.at(2).split(',');

            QColor color(stringList.at(0));
            char value(stringList.at(1).at(0).toLatin1());
            QRect rect(stringRect.at(0).toInt(), stringRect.at(1).toInt(), stringRect.at(2).toInt(), stringRect.at(3).toInt());
            int state(stringList.at(3).toInt());

            mCards[row][col] = Card(color, value, rect, (Card::State)state);
            ++col;
        }
    }
}

Card& CardBoard::GetCard(int row, int col)
{
    return mCards[row][col];
}

const Card& CardBoard::GetCard(int row, int col) const
{
    return mCards[row][col];
}

void CardBoard::resizeBoard(int boardWidth, int boardHeight)
{
    const int cellWidth = boardWidth / CardBoard::kNumCols;
    const int cellHeight = boardHeight / CardBoard::kNumRows;

    const int offset = 6;
    const int cardWidth = cellWidth - 2 * offset;
    const int cardHeight = cellHeight - 2 * offset;

    for (int row = 0; row < kNumRows; ++row)
    {
        for (int col = 0; col < kNumCols; ++col)
        {
            QRect cardRect(offset + cellWidth * col, offset + cellHeight * row, cardWidth, cardHeight);
            mCards[row][col].setRect(cardRect);
        }
    }
}
