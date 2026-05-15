import QtQuick

Rectangle {
    property string bgColor: "#1e2124"
    property string borderColor: "#3a3e42"
    property string fontColor: "#e8ecf0"
    property string labelText: "Log"

    color: bgColor
    border.color: borderColor
    border.width: 1
    radius: 3

    Column {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 6

        Text {
            text: labelText
            color: fontColor
            font.pixelSize: 13
            font.bold: true
        }

        Repeater {
            model: 4
            Rectangle {
                width: parent.width
                height: 1
                color: "#3a3e42"
            }
        }
    }
}
