import QtQuick 2.0

Rectangle
{
    property string sourceImg
    Image {
        id: image
        source: sourceImg
        sourceSize.width: parent.width
        sourceSize.height: parent.height
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
    }
}
