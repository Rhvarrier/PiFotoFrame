import QtQuick 2.0

Rectangle
{
    property string txt
    property real txtSize
    color: todayWeather.color
    Text {
        id: todayTeperature
        text: txt
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        color: "white"
        font.pixelSize: txtSize

    }
} // todayTeperatureContaine
