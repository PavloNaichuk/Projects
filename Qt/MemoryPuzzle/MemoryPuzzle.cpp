#include "MemoryPuzzle.h"
#include "CardBoard.h"
#include "CardBoardWidget.h"
#include "LevelGenerator.h"
#include <QApplication>
#include <memory>

int MemoryPuzzle::runGame(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //QString fileName("D:/alphabet.txt");
    //CardBoard* cardBoard = new CardBoard(fileName);

    std::shared_ptr<Card> cards(LevelGenerator::create());
    CardBoard cardBoard(cards.get());
    QString fileName("D:/level.txt");
    cardBoard.save(fileName);

    CardBoardWidget cardBoardWidget;
    cardBoardWidget.setCardBoard(&cardBoard);
    cardBoardWidget.show();

    return app.exec();
}


