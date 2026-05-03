#include "FindFilesWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    FindFilesWidget findFilesWidget;
    findFilesWidget.show();

    return app.exec();
}
