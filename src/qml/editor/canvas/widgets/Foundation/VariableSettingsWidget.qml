import QtQuick

Rectangle {
    property string bgColor: "#253141"
    property string borderColor: "#4a5468"
    property string fontColor: "#d8e3f0"
    property string labelText: "Variable"

    color: bgColor
    border.color: borderColor
    border.width: 1
    radius: 4

    Text {
        anchors.centerIn: parent
        text: labelText
        color: fontColor
        font.pixelSize: 14
        font.bold: true
    }
}
