import QtQuick

Rectangle {
    property string bgColor: "#2d3034"
    property string borderColor: "#4f5358"
    property string fontColor: "#e8ecf0"
    property string labelText: "Sub Window"

    color: bgColor
    border.color: borderColor
    border.width: 1
    radius: 4

    Text {
        anchors.centerIn: parent
        text: labelText
        color: fontColor
        font.pixelSize: 13
        font.bold: true
    }
}
