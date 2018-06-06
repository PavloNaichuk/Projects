#include "StudyTests.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    StudyTests studyTests;
    studyTests.runGame();

    return app.exec();

}
