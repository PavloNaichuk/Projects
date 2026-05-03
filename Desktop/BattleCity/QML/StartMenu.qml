import QtQuick 2.5

Rectangle
{
    id: startMenu
    visible: true
    width: levelMap.sizeInPixels.height
    height: levelMap.sizeInPixels.width
    focus: true
    color: "#7f6a00"

    signal playGameEntered

    Text
    {
        id: battleCityText
        width: parent.width
        height: parent.height / 3
        text: "Battle City"
        font.bold: true
        font.pointSize: 40
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    Text
    {
        id: controlsText
        height: parent.height / 3
        anchors.left: battleCityText.left
        anchors.top: battleCityText.bottom
        anchors.right: battleCityText.right
        text: "Navigate tank: left, right, up and down keys<br>Open fire: space key"
        font.bold: true
        font.pointSize: 14
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.Wrap
    }
    Text
    {
        id: pressEnterText
        height: parent.height / 3
        anchors.left: controlsText.left
        anchors.top: controlsText.bottom
        anchors.right: controlsText.right
        text: "Press Enter"
        font.bold: true
        font.pointSize: 16
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Keys.onReturnPressed:
    {
        startMenu.playGameEntered()
    }
}
