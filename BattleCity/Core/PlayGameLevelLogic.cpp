#include "PlayGameLevelLogic.h"
#include "Core/GameUnitModel.h"
#include "Core/Utilities.h"
#include "Core/Config.h"
#include "Core/LevelMap.h"
#include "MobileUnits/Missile.h"
#include "MobileUnits/Tank.h"
#include "StationaryUnits/Wall.h"
#include "StationaryUnits/PlayerBase.h"
#include "AI/SmartTankAI.h"
#include "AI/DummyTankAI.h"

PlayGameLevelLogic::PlayGameLevelLogic(LevelMap& levelMap, GameUnitModel& gameUnitModel)
    : mLevelMap(levelMap)
    , mGameUnitModel(gameUnitModel)
    , mMobileUnitStates(levelMap, gameUnitModel)
    , mNumActiveEnemyTanks(0)
    , mNumSpawnedEnemyTanks(0)
{
    QObject::connect(&mTimer, SIGNAL(timeout()), this, SLOT(update()));
}

void PlayGameLevelLogic::startPlay()
{
    spawnPlayerTank();

    for (int i = 0; i < mGameUnitModel.numBrickWalls(); ++i)
        QObject::connect(mGameUnitModel.brickWallAt(i), SIGNAL(damaged(Wall*)),
            this, SLOT(brickWallDamaged(Wall*)));

    QObject::connect(mGameUnitModel.playerTank(), SIGNAL(fired(Tank*)),
        this, SLOT(tankFired(Tank*)));

    mNumActiveEnemyTanks = 0;
    mNumSpawnedEnemyTanks = 0;

    mTimer.start(Config::UPDATE_DELAY_IN_MS);
    mEnemyTankSpawnTime.start();
}

void PlayGameLevelLogic::stopPlay()
{
    mTimer.stop();
}

void PlayGameLevelLogic::playerTankStopped()
{
    mGameUnitModel.playerTank()->stop();
}

void PlayGameLevelLogic::playerTankMovedLeft()
{
    Tank* pTank = mGameUnitModel.playerTank();
    pTank->setHeading(QVector2D(-1.0, 0.0));
    pTank->moveForward();
}

void PlayGameLevelLogic::playerTankMovedRight()
{
    Tank* pTank = mGameUnitModel.playerTank();
    pTank->setHeading(QVector2D(1.0, 0.0));
    pTank->moveForward();
}

void PlayGameLevelLogic::playerTankMovedUp()
{
    Tank* pTank = mGameUnitModel.playerTank();
    pTank->setHeading(QVector2D(0.0, -1.0));
    pTank->moveForward();
}

void PlayGameLevelLogic::playerTankMovedDown()
{
    Tank* pTank = mGameUnitModel.playerTank();
    pTank->setHeading(QVector2D(0.0, 1.0));
    pTank->moveForward();
}

void PlayGameLevelLogic::playerTankFired()
{
    Tank* pTank = mGameUnitModel.playerTank();
    if (pTank->readyToFire())
        pTank->fire();
}

void PlayGameLevelLogic::update()
{
    if ((mNumSpawnedEnemyTanks == Config::MAX_NUM_ENEMY_TANKS) && (mNumActiveEnemyTanks == 0))
    {
        stopPlay();
        emit levelCompleted();
    }
    else
    {
        mGameUnitModel.update();

        handleDamagedGameUnits();
        spawnEnemyTankIfNeeded();

        mGameUnitModel.notifyModelChanges();
    }
}

void PlayGameLevelLogic::missileDamaged(Missile* pMissile)
{
    if (!mDamagedMissiles.contains(pMissile))
        mDamagedMissiles.append(pMissile);
}

void PlayGameLevelLogic::enemyTankDamaged(Tank* pEnemyTank)
{
    if (!mDamagedEnemyTanks.contains(pEnemyTank))
        mDamagedEnemyTanks.append(pEnemyTank);
}

void PlayGameLevelLogic::brickWallDamaged(Wall* pBrickWall)
{
    if (!mDamagedBrickWalls.contains(pBrickWall))
        mDamagedBrickWalls.append(pBrickWall);
}

void PlayGameLevelLogic::handleDamagedGameUnits()
{
    if (!mDamagedMissiles.empty())
    {
        for (Missile* pMissile : mDamagedMissiles)
        {
            if (pMissile->health() <= 0)
            {
                mGameUnitModel.removeMissile(pMissile);
            }
        }
        mDamagedMissiles.clear();
    }

    if (!mDamagedEnemyTanks.empty())
    {
        for (Tank* pEnemyTank : mDamagedEnemyTanks)
        {
            if (pEnemyTank->health() <= 0)
            {
                mGameUnitModel.removeEnemyTank(pEnemyTank);
                --mNumActiveEnemyTanks;
                mEnemyTankSpawnTime.restart();
            }
        }
        mDamagedEnemyTanks.clear();
    }

    if (!mDamagedBrickWalls.empty())
    {
        for (Wall* pWall : mDamagedBrickWalls)
        {
            if (pWall->health() <= 0)
            {
                const QRectF& wallBounds = pWall->bounds();
                const QPointF wallCenter = wallBounds.center();

                int tileRow, tileCol;
                calcMapTileRowAndCol(wallCenter, tileRow, tileCol);

                MapTile& mapTile = mLevelMap.mapTileAt(tileRow, tileCol);
                Q_ASSERT(pWall == mapTile.mpStationaryUnit);
                mapTile.mpStationaryUnit = nullptr;

                mGameUnitModel.removeBrickWall(pWall);
            }
        }
        mDamagedBrickWalls.clear();
    }

    if ((mGameUnitModel.playerBase()->health() <= 0) || (mGameUnitModel.playerTank()->health() <= 0))
    {
        stopPlay();
        emit levelFailed();
    }
}

void PlayGameLevelLogic::tankFired(Tank* pPlayerTank)
{
    const QRectF& tankBounds(pPlayerTank->bounds());

    QPointF missileCenter(tankBounds.center());
    QPointF missileHalfSize(Config::MISSILE_HALF_SIZE, Config::MISSILE_HALF_SIZE);
    QRectF  missileBounds(missileCenter - missileHalfSize, missileCenter + missileHalfSize);

    Missile* pMissile = new Missile(mLevelMap, mGameUnitModel, missileBounds, pPlayerTank->heading());
    mGameUnitModel.addMissile(pMissile);

    QObject::connect(pMissile, SIGNAL(damaged(Missile*)), this, SLOT(missileDamaged(Missile*)));
}

void PlayGameLevelLogic::spawnPlayerTank()
{
    QPointF center = mLevelMap.playerTankSpawnLocation();

    QPointF halfSize(Config::PLAYER_TANK_HALF_SIZE, Config::PLAYER_TANK_HALF_SIZE);
    QRectF bounds(center - halfSize, center + halfSize);
    QVector2D heading(0.0f, -1.0f);

    auto tankAI = std::make_shared<DummyTankAI>();

    Tank* pTank = new Tank(GameUnitType::TANK, tankAI, mLevelMap, mGameUnitModel, mMobileUnitStates,
        bounds, heading, Config::PLAYER_TANK_FULL_HEALTH, Config::PLAYER_TANK_SPEED);

    mGameUnitModel.setPlayerTank(pTank);
    mLevelMap.setPlayerTankSpawnLocation(center);
}

void PlayGameLevelLogic::spawnEnemyTankIfNeeded()
{
    if (mNumSpawnedEnemyTanks == Config::MAX_NUM_ENEMY_TANKS)
        return;

    if ((mNumActiveEnemyTanks < Config::MAX_NUM_ACTIVE_ENEMY_TANKS) &&
        (mEnemyTankSpawnTime.elapsed() > Config::ENEMY_TANK_RESPAWN_IN_MS))
    {
        const QVector<QPointF>& spawnLocations = mLevelMap.enemyTankSpawnLocations();

        int spawnLocationIndex = mNumActiveEnemyTanks % spawnLocations.size();
        const QPointF& center = spawnLocations[spawnLocationIndex];

        QPointF halfSize(Config::ENEMY_TANK_HALF_SIZE, Config::ENEMY_TANK_HALF_SIZE);
        QRectF bounds(center - halfSize, center + halfSize);
        QVector2D heading(0.0f, 1.0f);

        auto tankAI = std::make_shared<SmartTankAI>(mLevelMap, mGameUnitModel);

        Tank* pTank = new Tank(GameUnitType::TANK, tankAI, mLevelMap, mGameUnitModel, mMobileUnitStates,
            bounds, heading, Config::ENEMY_TANK_FULL_HEALTH, Config::ENEMY_TANK_SPEED);

        mGameUnitModel.addEnemyTank(pTank);

        QObject::connect(pTank, SIGNAL(damaged(Tank*)), this, SLOT(enemyTankDamaged(Tank*)));
        QObject::connect(pTank, SIGNAL(fired(Tank*)), this, SLOT(tankFired(Tank*)));

        ++mNumActiveEnemyTanks;
        ++mNumSpawnedEnemyTanks;

        mEnemyTankSpawnTime.restart();
    }
}
