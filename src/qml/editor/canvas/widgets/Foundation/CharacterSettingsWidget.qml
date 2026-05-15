import QtQuick

Rectangle {
    property string bgColor: "#2f3134"
    property string borderColor: "#4a4e53"
    property string fontColor: "#f5f5f5"
    property string labelText: "Text"

    color: bgColor
    border.color: borderColor
    border.width: 1
    radius: 3

    Text {
        anchors.centerIn: parent
        text: labelText
        color: fontColor
        font.pixelSize: 16
        font.bold: true
    }
}
