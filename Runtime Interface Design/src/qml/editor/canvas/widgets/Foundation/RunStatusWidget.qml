import QtQuick

Rectangle {
    property string bgColor: "#1a3528"
    property string borderColor: "#2e5c45"
    property string fontColor: "#dff0d8"
    property bool isRunning: false

    color: bgColor
    radius: 4
    border.color: borderColor
    border.width: 1

    Row {
        anchors.centerIn: parent
        spacing: 6

        Rectangle {
            width: 10
            height: 10
            radius: 5
            anchors.verticalCenter: parent.verticalCenter
            color: isRunning ? "#27ae60" : "#888888"
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: isRunning ? "Running" : "Stop Run"
            color: fontColor
            font.pixelSize: 11
        }
    }
}
