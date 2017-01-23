#pragma once

#include "MobileUnitStates/MobileUnitStates.h"
#include <QObject>
#include <QList>
#include <QTimer>
#include <QTime>

class GameUnitModel;
class LevelMap;

class Tank;
class Missile;
class PlayerBase;
class Wall;

class PlayGameLevelLogic : public QObject
{
    Q_OBJECT

public:
    PlayGameLevelLogic(LevelMap& levelMap, GameUnitModel& gameUnitModel);

    void startPlay();
    void stopPlay();

signals:
    void levelCompleted();
    void levelFailed();

public slots:
    void update();

    void playerTankStopped();
    void playerTankMovedLeft();
    void playerTankMovedRight();
    void playerTankMovedUp();
    void playerTankMovedDown();
    void playerTankFired();

    void missileDamaged(Missile* pMissile);
    void enemyTankDamaged(Tank* pTank);
    void brickWallDamaged(Wall* pWall);

    void tankFired(Tank* pTank);

private:
    void spawnEnemyTankIfNeeded();
    void spawnPlayerTank();
    void handleDamagedGameUnits();

private:
    QTimer mTimer;
    QTime mEnemyTankSpawnTime;

    LevelMap& mLevelMap;
    GameUnitModel& mGameUnitModel;
    MobileUnitStates mMobileUnitStates;

    QList<Missile*> mDamagedMissiles;
    QList<Tank*> mDamagedEnemyTanks;
    QList<Wall*> mDamagedBrickWalls;

    unsigned mNumActiveEnemyTanks;
    unsigned mNumSpawnedEnemyTanks;
};
