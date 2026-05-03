#include "BattleCityGame.h"

#include "MobileUnits/Tank.h"
#include "MobileUnits/Missile.h"
#include "StationaryUnits/Wall.h"
#include "StationaryUnits/PlayerBase.h"

#include <QApplication>
#include <QString>
#include <QDesktopWidget>
#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWidget>

BattleCityGame::BattleCityGame()
    : mPlayLevelLogic(mLevelMap, mGameUnitModel)
{
    QObject::connect(&mPlayLevelLogic, SIGNAL(levelCompleted()), this, SLOT(handleLevelCompleted()));
    QObject::connect(&mPlayLevelLogic, SIGNAL(levelFailed()), this, SLOT(handleLevelFailed()));
}

int BattleCityGame::start(int argc, char** argv)
{
    QApplication app(argc, argv);

    qmlRegisterType<Tank>();
    qmlRegisterType<Missile>();
    qmlRegisterType<Wall>();
    qmlRegisterType<PlayerBase>();

    QQuickWidget mainWidget;

    QQmlContext* pRootContext = mainWidget.rootContext();
    pRootContext->setContextProperty("gameUnitModel", &mGameUnitModel);
    pRootContext->setContextProperty("playLevelLogic", &mPlayLevelLogic);
    pRootContext->setContextProperty("levelMap", &mLevelMap);
    pRootContext->setContextProperty("game", this);

    mainWidget.setWindowTitle(QString("Battle City by Pavlo Naichuk"));
    mainWidget.setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);
    mainWidget.setSource(QUrl(QStringLiteral("qrc:/QML/MainWidget.qml")));

    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width() - mainWidget.width()) / 2;
    int y = (screenGeometry.height() - mainWidget.height()) / 2;

    mainWidget.move(x, y);
    mainWidget.show();

    return app.exec();
}

void BattleCityGame::playNextLevel()
{
    mLevelMap.loadLevel(QStringLiteral(":/Levels/Level1.txt"), mGameUnitModel);
    mPlayLevelLogic.startPlay();
}

void BattleCityGame::handleLevelCompleted()
{
    emit gameEnded();
}

void BattleCityGame::handleLevelFailed()
{
    emit gameEnded();
}
