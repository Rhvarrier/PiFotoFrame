import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

ApplicationWindow {
    visible: true
    width: Screen.width
    height: Screen.height
    title: qsTr("PiFotoFrame")
    flags: Qt.Window | Qt.FramelessWindowHint
    Connections
    {
        target: Photo
        function onUpdateImage(img_path)
        {
            if (img_path === "")
            {
                mainImage.source = "qrc:///test_img.jpg"
            }

            mainImage.source = "file:" + img_path
        }
    }
//    RowLayout
//    {
//        spacing: 0
        Rectangle{
            Image {
                id: mainImage
                source: "qrc:///test_img.jpg"
                sourceSize.width: Screen.width
                sourceSize.height: Screen.height
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
            }
            color: "grey"
            anchors.right: parent.right
            width: parent.width*7/8
            height: parent.height
        }

        WeatherWidget
        {
            widgetColor: "grey"
            width: parent.width/8
            height: parent.height
             anchors.left: parent.left
        }
//    }

}
