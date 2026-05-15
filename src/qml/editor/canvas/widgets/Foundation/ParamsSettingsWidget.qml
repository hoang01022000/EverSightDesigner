import QtQuick

Rectangle {
    property string bgColor: "#2c3239"
    property string borderColor: "#515960"
    property string fontColor: "#e8ecf0"
    property string labelText: "Parameter"

    color: bgColor
    border.color: borderColor
    border.width: 1
    radius: 4

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 8

        Text {
            text: labelText
            color: fontColor
            font.pixelSize: 14
            font.bold: true
            elide: Text.ElideRight
            width: parent.width
        }

        Rectangle {
            width: parent.width
            height: Math.max(28, parent.parent.height - 46)
            radius: 3
            color: "#20262d"
            border.color: "#65707a"

            Text {
                anchors.centerIn: parent
                text: "Value"
                color: "#aeb7c0"
                font.pixelSize: 12
            }
        }
    }
}
