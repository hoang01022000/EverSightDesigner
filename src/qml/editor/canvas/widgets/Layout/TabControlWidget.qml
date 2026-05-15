import QtQuick

Rectangle {
    property string bgColor: "#2b2f33"
    property string borderColor: "#54595f"
    property string fontColor: "#e8ecf0"
    property string labelText: "Tab Control"

    color: bgColor
    border.color: borderColor
    border.width: 1
    radius: 4

    Row {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 6
        spacing: 4

        Repeater {
            model: 2
            Rectangle {
                width: 72
                height: 24
                color: index === 0 ? "#3a4148" : "#24292f"
                border.color: borderColor

                Text {
                    anchors.centerIn: parent
                    text: index === 0 ? "Tab 1" : "Tab 2"
                    color: fontColor
                    font.pixelSize: 11
                }
            }
        }
    }

    Text {
        anchors.centerIn: parent
        text: labelText
        color: "#aeb7c0"
        font.pixelSize: 13
    }
}
