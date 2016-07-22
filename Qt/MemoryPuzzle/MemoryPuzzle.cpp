#include "MemoryPuzzle.h"
#include "CardBoard.h"
#include "MainWidget.h"
#include "LevelGenerator.h"
#include "GameLogic.h"
#include <QApplication>
#include <memory>

int MemoryPuzzle::runGame(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //QString fileName("D:/alphabet.txt");
    //CardBoard* cardBoard = new CardBoard(fileName);

    std::unique_ptr<Card> cards(LevelGenerator::create());
    CardBoard cardBoard(cards.get());
    QString fileName("D:/level.txt");
    cardBoard.save(fileName);

    GameLogic gameLogic;
    MainWidget mainWidget(&gameLogic);
    mainWidget.setCardBoard(&cardBoard);
    mainWidget.show();

    return app.exec();
}


