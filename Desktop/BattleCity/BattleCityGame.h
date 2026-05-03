#pragma once

#include <QObject>

#include "Core/LevelMap.h"
#include "Core/GameUnitModel.h"
#include "Core/PlayGameLevelLogic.h"

class BattleCityGame : public QObject
{
    Q_OBJECT

public:
    BattleCityGame();
    int start(int argc, char** argv);

public slots:
    void playNextLevel();

    void handleLevelCompleted();
    void handleLevelFailed();

signals:
    void gameEnded();

private:
    LevelMap mLevelMap;
    GameUnitModel mGameUnitModel;
    PlayGameLevelLogic mPlayLevelLogic;
};
