import QtQuick 2.5

Item
{
    id: mainWidget
    visible: true
    width: levelMap.sizeInPixels.height
    height: levelMap.sizeInPixels.width

    Loader
    {
        id: pageLoader
        source: "StartMenu.qml"
        focus: true
    }

    Connections
    {
        target: pageLoader.item
        onPlayGameEntered:
        {
            game.playNextLevel();
            pageLoader.source = "PlayGame.qml"
        }
    }
    Connections
    {
        target: game
        onGameEnded:
        {
            pageLoader.source = "EndMenu.qml"
        }
    }
}
