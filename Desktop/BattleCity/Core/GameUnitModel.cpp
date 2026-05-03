#include "GameUnitModel.h"
#include "MobileUnits/Missile.h"
#include "MobileUnits/Tank.h"
#include "StationaryUnits/Wall.h"
#include "StationaryUnits/PlayerBase.h"
#include <QtAlgorithms>
#include <QtGlobal>

GameUnitModel::GameUnitModel(QObject* pParent)
    : QObject(pParent)
    , mpPlayerBase(nullptr)
    , mpPlayerTank(nullptr)
    , mChangedGameUnitFlags(0)
{
}

GameUnitModel::~GameUnitModel()
{
    clear();
}

void GameUnitModel::clear()
{
    qDeleteAll(mEnemyTanks);
    mEnemyTanks.clear();

    qDeleteAll(mMissiles);
    mMissiles.clear();

    qDeleteAll(mBrickWalls);
    mBrickWalls.clear();

    qDeleteAll(mSteelWalls);
    mSteelWalls.clear();

    setPlayerBase(nullptr);
    setPlayerTank(nullptr);

    mChangedGameUnitFlags = 0;
}

QQmlListProperty<Tank> GameUnitModel::enemyTanks()
{
    return QQmlListProperty<Tank>(this, mEnemyTanks);
}

void GameUnitModel::addEnemyTank(Tank* pEnemyTank)
{
    Q_ASSERT(!mEnemyTanks.contains(pEnemyTank));
    mEnemyTanks.append(pEnemyTank);

    mChangedGameUnitFlags |= EnemyTanksChangedFlag;
}

void GameUnitModel::removeEnemyTank(Tank* pEnemyTank)
{
    Q_ASSERT(mEnemyTanks.contains(pEnemyTank));
    mEnemyTanks.removeOne(pEnemyTank);
    //delete pEnemyTank;

    mChangedGameUnitFlags |= EnemyTanksChangedFlag;
}

int GameUnitModel::numEnemyTanks() const
{
    return mEnemyTanks.size();
}

Tank* GameUnitModel::enemyTankAt(int index)
{
    return mEnemyTanks.at(index);
}

QQmlListProperty<Missile> GameUnitModel::missiles()
{
    return QQmlListProperty<Missile>(this, mMissiles);
}

void GameUnitModel::addMissile(Missile* pMissile)
{
    Q_ASSERT(!mMissiles.contains(pMissile));
    mMissiles.append(pMissile);

    mChangedGameUnitFlags |= MissilesChangedFlag;
}

void GameUnitModel::removeMissile(Missile* pMissile)
{
    Q_ASSERT(mMissiles.contains(pMissile));
    mMissiles.removeOne(pMissile);
    //delete pMissile;

    mChangedGameUnitFlags |= MissilesChangedFlag;
}

int GameUnitModel::numMissiles() const
{
    return mMissiles.size();
}

Missile* GameUnitModel::missileAt(int index)
{
    return mMissiles.at(index);
}

QQmlListProperty<Wall> GameUnitModel::brickWalls()
{
    return QQmlListProperty<Wall>(this, mBrickWalls);
}

void GameUnitModel::addBrickWall(Wall* pWall)
{
    Q_ASSERT(!mBrickWalls.contains(pWall));
    mBrickWalls.append(pWall);

    mChangedGameUnitFlags |= BrickWallsChangedFlag;
}

void GameUnitModel::removeBrickWall(Wall* pWall)
{
    Q_ASSERT(mBrickWalls.contains(pWall));
    mBrickWalls.removeOne(pWall);
    //delete pBrickWall;

    mChangedGameUnitFlags |= BrickWallsChangedFlag;
}

int GameUnitModel::numBrickWalls() const
{
    return mBrickWalls.size();
}

Wall* GameUnitModel::brickWallAt(int index)
{
    return mBrickWalls.at(index);
}

QQmlListProperty<Wall> GameUnitModel::steelWalls()
{
    return QQmlListProperty<Wall>(this, mSteelWalls);
}

void GameUnitModel::addSteelWall(Wall* pWall)
{
    Q_ASSERT(!mSteelWalls.contains(pWall));
    mSteelWalls.append(pWall);

    mChangedGameUnitFlags |= SteelWallsChangedFlag;
}

void GameUnitModel::removeSteelWall(Wall* pWall)
{
    Q_ASSERT(mSteelWalls.contains(pWall));
    mSteelWalls.removeOne(pWall);
    //delete pWall;

    mChangedGameUnitFlags |= SteelWallsChangedFlag;
}

int GameUnitModel::numSteelWalls() const
{
    return mSteelWalls.size();
}

Wall* GameUnitModel::steelWallAt(int index)
{
    return mSteelWalls.at(index);
}

PlayerBase* GameUnitModel::playerBase()
{
    return mpPlayerBase;
}

void GameUnitModel::setPlayerBase(PlayerBase* pPlayerBase)
{
    if (mpPlayerBase != pPlayerBase)
    {
        delete mpPlayerBase;
        mpPlayerBase = pPlayerBase;

        mChangedGameUnitFlags |= PlayerBaseChangedFlag;
    }
}

Tank* GameUnitModel::playerTank()
{
    return mpPlayerTank;
}

void GameUnitModel::setPlayerTank(Tank* pPlayerTank)
{
    if (mpPlayerTank != pPlayerTank)
    {
        delete mpPlayerTank;
        mpPlayerTank = pPlayerTank;

        mChangedGameUnitFlags |= PlayerTankChangedFlag;
    }
}

void GameUnitModel::notifyModelChanges()
{
    //if ((mChangedGameUnitFlags & EnemyTanksChangedFlag) != 0)
        enemyTanksChanged(enemyTanks());

    //if ((mChangedGameUnitFlags & MissilesChangedFlag) != 0)
        missilesChanged(missiles());

    if ((mChangedGameUnitFlags & BrickWallsChangedFlag) != 0)
        brickWallsChanged(brickWalls());

    if ((mChangedGameUnitFlags & SteelWallsChangedFlag) != 0)
        steelWallsChanged(steelWalls());

    if ((mChangedGameUnitFlags & PlayerBaseChangedFlag) != 0)
        playerBaseChanged(playerBase());

    //if ((mChangedGameUnitFlags & PlayerTankChangedFlag) != 0)
        playerTankChanged(playerTank());

    mChangedGameUnitFlags = 0;
}

void GameUnitModel::update()
{
    for (int i = 0; i < numEnemyTanks(); ++i)
        enemyTankAt(i)->update();

    for (int i = 0; i < numMissiles(); ++i)
        missileAt(i)->update();

    for (int i = 0; i < numBrickWalls(); ++i)
        brickWallAt(i)->update();

    for (int i = 0; i < numSteelWalls(); ++i)
        steelWallAt(i)->update();

    Q_ASSERT(mpPlayerBase != nullptr);
    mpPlayerBase->update();

    Q_ASSERT(mpPlayerTank != nullptr);
    mpPlayerTank->update();
}
