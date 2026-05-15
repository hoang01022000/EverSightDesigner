import QtQuick

Rectangle {
    property string bgColor: "#3b3222"
    property string borderColor: "#705d35"
    property string fontColor: "#f5e9c8"
    property string labelText: "Conditional"

    color: bgColor
    border.color: borderColor
    border.width: 1
    radius: 4

    Rectangle {
        width: Math.min(parent.width, parent.height) * 0.36
        height: width
        radius: width / 2
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.verticalCenter: parent.verticalCenter
        color: "#d9a441"
        border.color: "#f3cf80"
    }

    Text {
        anchors.centerIn: parent
        text: labelText
        color: fontColor
        font.pixelSize: 12
        font.bold: true
    }
}
