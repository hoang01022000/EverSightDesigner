import QtQuick

Rectangle {
    property string bgColor: "#2b2f33"
    property string borderColor: "#4f5358"
    property string fontColor: "#e8ecf0"

    color: bgColor
    border.color: borderColor
    border.width: 1
    radius: 3

    Rectangle {
        anchors.centerIn: parent
        width: Math.min(parent.width, parent.height) * 0.62
        height: width
        radius: width / 2
        color: "#60a5fa"
        border.color: "#f59e0b"
        border.width: Math.max(6, width * 0.16)
    }
}
