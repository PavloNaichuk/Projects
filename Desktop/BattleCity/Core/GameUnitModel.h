#pragma once

#include <QList>
#include <QQmlListProperty>

class Tank;
class Missile;
class Wall;
class PlayerBase;

enum GameUnitChangedFlags
{
    EnemyTanksChangedFlag = 1 << 0,
    MissilesChangedFlag = 1 << 1,
    BrickWallsChangedFlag = 1 << 2,
    SteelWallsChangedFlag = 1 << 3,
    PlayerBaseChangedFlag = 1 << 4,
    PlayerTankChangedFlag = 1 << 5
};

class GameUnitModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<Tank> enemyTanks READ enemyTanks NOTIFY enemyTanksChanged)
    Q_PROPERTY(QQmlListProperty<Missile> missiles READ missiles NOTIFY missilesChanged)
    Q_PROPERTY(QQmlListProperty<Wall> brickWalls READ brickWalls NOTIFY brickWallsChanged)
    Q_PROPERTY(QQmlListProperty<Wall> steelWalls READ steelWalls NOTIFY steelWallsChanged)
    Q_PROPERTY(PlayerBase* playerBase READ playerBase NOTIFY playerBaseChanged)
    Q_PROPERTY(Tank* playerTank READ playerTank NOTIFY playerTankChanged)

public:
    GameUnitModel(QObject* pParent = nullptr);
    ~GameUnitModel();

    void clear();
    void update();

    QQmlListProperty<Tank> enemyTanks();
    int numEnemyTanks() const;
    Tank* enemyTankAt(int index);
    void addEnemyTank(Tank* pEnemyTank);
    void removeEnemyTank(Tank* pEnemyTank);

    QQmlListProperty<Missile> missiles();
    int numMissiles() const;
    Missile* missileAt(int index);
    void addMissile(Missile* pMissile);
    void removeMissile(Missile* pMissile);

    QQmlListProperty<Wall> brickWalls();
    int numBrickWalls() const;
    Wall* brickWallAt(int index);
    void addBrickWall(Wall* pWall);
    void removeBrickWall(Wall* pWall);

    QQmlListProperty<Wall> steelWalls();
    int numSteelWalls() const;
    Wall* steelWallAt(int index);
    void addSteelWall(Wall* pWall);
    void removeSteelWall(Wall* pWall);

    PlayerBase* playerBase();
    void setPlayerBase(PlayerBase* pPlayerBase);

    Tank* playerTank();
    void setPlayerTank(Tank* pPlayerTank);

    void notifyModelChanges();

signals:
    void enemyTanksChanged(QQmlListProperty<Tank> enemyTanks);
    void missilesChanged(QQmlListProperty<Missile> missiles);
    void brickWallsChanged(QQmlListProperty<Wall> brickWalls);
    void steelWallsChanged(QQmlListProperty<Wall> steelWalls);
    void playerBaseChanged(PlayerBase* playerBase);
    void playerTankChanged(Tank* playerTank);

private:
    QList<Tank*> mEnemyTanks;
    QList<Missile*> mMissiles;
    QList<Wall*> mBrickWalls;
    QList<Wall*> mSteelWalls;
    PlayerBase* mpPlayerBase;
    Tank* mpPlayerTank;

    unsigned mChangedGameUnitFlags;
};
