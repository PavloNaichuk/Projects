#include "MemoryPuzzle.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MemoryPuzzle memoryPuzzle;
    memoryPuzzle.runGame();
    return app.exec();
}

