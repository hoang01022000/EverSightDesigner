import QtQuick

Rectangle {
    property string bgColor: "#1e2124"
    property string borderColor: "#3a3e42"
    property string fontColor: "#f5f5f5"
    property string labelText: "HH:mm:ss"

    color: bgColor
    border.color: borderColor
    border.width: 1
    radius: 3

    Text {
        anchors.centerIn: parent
        text: labelText
        color: fontColor
        font.pixelSize: 18
        font.family: "Consolas"
    }
}
