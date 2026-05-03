import QtQuick 2.5

Item
{
    x: modelData.bounds.left
    y: modelData.bounds.top
    width: modelData.bounds.width
    height: modelData.bounds.height
    focus: true

    Image
    {
        anchors.fill: parent
        width: parent.width
        height: parent.height
        source: "qrc:/Images/BrickWall.jpg"
    }
}
