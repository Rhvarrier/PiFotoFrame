import QtQuick.Controls 2.5
import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

Rectangle
{
    property color widgetColor

//    property real todayLabelHeight : parent.height*1/28
//    property real todayDateHeight: parent.height*1/28
//    property real todayWeatherIconHeight: parent.height*4/28
//    property real todayWeatherDescriptionHeight: parent.height*1/28
//    property real todayMaxMinHeight: parent.height*2/28
//    property real todayTempHeight : parent.height*3/28
//    property real blankBoxHeight : parent.height*2/28

//    property real tomorrowWeatherMinMaxHeight : parent.height*4/28
//    property real tomorrowWeatherIconHeight: parent.height*4/28
//    property real tomorrowWeatherDescriptionHeight: parent.height*1/28
//    property real tomorrowLabelHeight : parent.height*1/28
//    property real tomorrowDateHeight : parent.height*1/28

    property real todayLabelHeight : parent.height*1/24
    property real todayDateHeight: parent.height*1/24
    property real todayWeatherIconHeight: parent.height*4/24
    property real todayWeatherDescriptionHeight: parent.height*1/24
    property real todayMaxMinHeight: parent.height*2/24
    property real todayTempHeight : parent.height*3/24
    property real blankBoxHeight : parent.height*2/24

    property real tomorrowWeatherMinMaxHeight : parent.height*3/24
    property real tomorrowWeatherIconHeight: parent.height*4/24
    property real tomorrowWeatherDescriptionHeight: parent.height*1/24
    property real tomorrowLabelHeight : parent.height*1/24
    property real tomorrowDateHeight : parent.height*1/24

    color: widgetColor
    property date currentDate: new Date()

    Timer {
        interval: 10000; running: true; repeat: true
        onTriggered: currentDate = new Date()
    }

    function addTrailingZero(num)
    {
        if (num < 9)
        {
            return "0"+num
        }
        else
        {
            return num
        }
    }

    function getDateStr(date)
    {
        var weekday = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thusday", "Fiday", "Saturday"]
        return weekday[date.getUTCDay()] + " " + addTrailingZero(date.getUTCDate()) + "/" + addTrailingZero(date.getUTCMonth())
    }
    function getNextDay(day) {
        var tomorrow = new Date();
        tomorrow.setDate(day.getDate()+1)
        return tomorrow
    }
    Connections
    {
        target: Weather
        function onTodayWeatherIconUpdated(icon_url)
        {
            todayWeatherIcon.sourceImg = "qrc:///weather_icons/weather_icons/" + icon_url
        }
        function onTodayWeatherUpdated()
        {
            todayWeatherMinMax.maxTemp = Weather.todayMaxTemp
            todayWeatherMinMax.minTemp = Weather.todayMinTemp
            todayTeperature.txt = Weather.todayTemp
            todayWeatherDescription.txt = Weather.todayDescription
        }
        function onTomorrowWeatherIconUpdated(icon_url)
        {
            tomorrowWeatherIcon.sourceImg = "qrc:///weather_icons/weather_icons/" + icon_url
        }
        function onTomorrowWeatherUpdated()
        {
            tomorrowWeatherMinMax.minTemp = Weather.tomorrowMinTemp
            tomorrowWeatherMinMax.maxTemp = Weather.tomorrowMaxTemp
            tomorrowWeatherDescription.txt = Weather.tomorrowDescription
        }
    }

    ColumnLayout
    {
        id: globalLayout
        spacing: 0
        anchors.fill: parent
        Rectangle
        {
            id: todayWeather
            color: widgetColor
            Layout.preferredHeight: todayTempHeight + todayMaxMinHeight + todayWeatherIconHeight + todayWeatherDescriptionHeight + todayLabelHeight + todayDateHeight
            Layout.preferredWidth : parent.width
            Layout.alignment: Qt.AlignTop
            ColumnLayout
            {
                anchors.fill: parent
                spacing: 0
                TextContainer
                {
                    id: todayLabel
                    Layout.preferredHeight: todayLabelHeight
                    Layout.preferredWidth : parent.width
                    color: widgetColor
                    txt: "Today"
                    txtSize : todayDateHeight/1.5
                }

//                TextContainer
//                {
//                    id: todayDate
//                    Layout.preferredHeight: todayDateHeight
//                    Layout.preferredWidth : parent.width
//                    color: widgetColor
//                    txt: getDateStr(currentDate)
//                    txtSize : todayDateHeight/1.5

//                }

                WeatherIcon
                {
                    id : todayWeatherIcon
                    color: widgetColor
                    Layout.preferredHeight: todayWeatherIconHeight
                    Layout.preferredWidth : parent.width
                    sourceImg : "qrc:///weather_icons/weather_icons/Clear.png"
                }

                TextContainer
                {
                    id: todayWeatherDescription
                    Layout.preferredHeight: todayWeatherDescriptionHeight
                    Layout.preferredWidth : parent.width
                    color: widgetColor
                    txt: qsTr("-")
                    txtSize : todayWeatherDescriptionHeight/1.5

                }

                TempMaxMin
                {
                    id: todayWeatherMinMax
                    color: widgetColor
                    Layout.preferredHeight: todayWeatherIconHeight
                    Layout.preferredWidth : parent.width
                    maxTemp : qsTr("-")
                    minTemp : qsTr("-")
                }

                TextContainer
                {
                    id : todayTeperature
                    Layout.preferredHeight: todayTempHeight
                    Layout.preferredWidth : parent.width
                    color: widgetColor
                    Layout.alignment: Qt.AlignRight
                    txt: qsTr("-")
                    txtSize: height/1.5
                } // todayTeperature
            }
        } // Today Weather

        Rectangle
        {
            id : tomorrowWeather
            color: widgetColor
            Layout.preferredHeight: tomorrowWeatherMinMaxHeight + tomorrowWeatherIconHeight + tomorrowWeatherDescriptionHeight + tomorrowLabelHeight + tomorrowDateHeight
            Layout.preferredWidth : parent.width
            Layout.alignment: Qt.AlignBottom


            ColumnLayout
            {
                spacing : 0
                anchors.fill: parent
                TempMaxMin
                {
                    id: tomorrowWeatherMinMax
                    color: widgetColor
                    Layout.preferredHeight: tomorrowWeatherMinMaxHeight
                    Layout.preferredWidth : parent.width
                    maxTemp : qsTr("-")
                    minTemp : qsTr("-")
                }

                WeatherIcon
                {
                    id : tomorrowWeatherIcon
                    color: widgetColor
                    Layout.preferredHeight: tomorrowWeatherIconHeight
                    Layout.preferredWidth : parent.width
                    sourceImg : "qrc:///weather_icons/weather_icons/Clear.png"
                }
                TextContainer
                {
                    id: tomorrowWeatherDescription
                    Layout.preferredHeight: tomorrowWeatherDescriptionHeight
                    Layout.preferredWidth : parent.width
                    color: widgetColor
                    txt: qsTr("-")
                    txtSize : tomorrowWeatherDescriptionHeight/1.5

                }

                TextContainer
                {
                    id: tomorrowLabel
                    Layout.preferredHeight: tomorrowLabelHeight
                    Layout.preferredWidth : parent.width
                    color: widgetColor
                    txt: "Tomorrow"
                    txtSize : tomorrowLabelHeight/1.5
                }
            }
        } // tomorrowWeather
    } // globalLayout
} //WeatherWidget Item

