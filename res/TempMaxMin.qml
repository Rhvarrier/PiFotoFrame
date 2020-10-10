import QtQuick 2.0
import QtQuick.Layouts 1.3

Rectangle
{

    property string maxTemp
    property string minTemp

    RowLayout
    {
        anchors.fill: parent
        spacing: 15
        property color color: parent.color
        Rectangle
        {
            id : maxTemperatureContainer
            color: parent.color
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height/2
            Layout.alignment: Qt.AlignLeft
            Text {
                id: maxTemperatureLabel
                text: qsTr("MAX")
                color: "white"
                font.pixelSize: parent.width/6
            }
            Text {
                id: maxTemparature
                text: maxTemp
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                color: "white"
                font.pixelSize: parent.width/3
            }
        }
        Rectangle
        {
            id : minTemperatureContainer
            color: parent.color
            Layout.alignment: Qt.AlignRight
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height/2
            Text {
                id: minTemperatureLabel
                text: qsTr("MIN")
                color: "white"
                font.pixelSize: parent.width/6
            }
            Text {
                id: minTemperature
                text: minTemp
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                color: "white"
                font.pixelSize: parent.width/3
            }
        }
    }
}
