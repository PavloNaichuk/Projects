import QtQuick 2.5

Item
{
    id: playGame
    visible: true
    width: levelMap.sizeInPixels.height
    height: levelMap.sizeInPixels.width
    focus: true

    Keys.onPressed:
    {
        if (event.key === Qt.Key_Right)
        {
            playLevelLogic.playerTankMovedRight();
            event.accepted = true;
            return;
        }
        if (event.key === Qt.Key_Down)
        {
            playLevelLogic.playerTankMovedDown();
            event.accepted = true;
            return;
        }
        if (event.key === Qt.Key_Left)
        {
            playLevelLogic.playerTankMovedLeft();
            event.accepted = true;
            return;
        }
        if (event.key === Qt.Key_Up)
        {
            playLevelLogic.playerTankMovedUp();
            event.accepted = true;
            return;
        }
        if (event.key === Qt.Key_Space)
        {
            playLevelLogic.playerTankFired();
            event.accepted = true;
            return;
        }
    }

    Keys.onReleased:
    {
        if ((event.key === Qt.Key_Left) ||
            (event.key === Qt.Key_Right) ||
            (event.key === Qt.Key_Up) ||
            (event.key === Qt.Key_Down))
        {
            playLevelLogic.playerTankStopped();
            event.accepted = true;
            return;
        }
    }

    Rectangle
    {
        color: "#7f6a00"
        width: parent.width
        height: parent.height

        Repeater
        {
            model: gameUnitModel.brickWalls
            BrickWall
            {
            }
        }
        Repeater
        {
            model: gameUnitModel.steelWalls
            SteelWall
            {
            }
        }
        Repeater
        {
            model: gameUnitModel.missiles
            Missile
            {
            }
        }
        Repeater
        {
            model: gameUnitModel.enemyTanks
            EnemyTank
            {
            }
        }
        PlayerBase
        {
            x: gameUnitModel.playerBase.bounds.left
            y: gameUnitModel.playerBase.bounds.top
            width: gameUnitModel.playerBase.bounds.width
            height: gameUnitModel.playerBase.bounds.height
        }
        PlayerTank
        {
            x: gameUnitModel.playerTank.bounds.left
            y: gameUnitModel.playerTank.bounds.top
            width: gameUnitModel.playerTank.bounds.width
            height: gameUnitModel.playerTank.bounds.height
            rotation: gameUnitModel.playerTank.rotationAngle
        }
    }
}
