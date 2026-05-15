import QtQuick

Rectangle {
    property string bgColor: "#2d3136"
    property string borderColor: "#5b5f65"
    property string fontColor: "#e8ecf0"
    property string labelText: "Group"

    color: bgColor
    border.color: borderColor
    border.width: 1
    radius: 4

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 28
        color: "#252a30"

        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
            text: labelText
            color: fontColor
            font.pixelSize: 13
            font.bold: true
        }
    }
}
