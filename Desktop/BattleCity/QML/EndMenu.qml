import QtQuick 2.5

Rectangle
{
    id: endMenu
    visible: true
    width: levelMap.sizeInPixels.height
    height: levelMap.sizeInPixels.width
    focus: true
    color: "#7f6a00"

    Text
    {
        width: parent.width
        height: parent.height
        text: "Game over"
        font.bold: true
        font.pointSize: 18
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
