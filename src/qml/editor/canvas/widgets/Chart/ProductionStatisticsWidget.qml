import QtQuick

Rectangle {
    property string bgColor: "#2b2f33"
    property string borderColor: "#4f5358"
    property string fontColor: "#e8ecf0"
    property string labelText: "Statistics"

    color: bgColor
    border.color: borderColor
    border.width: 1
    radius: 3

    Row {
        anchors.centerIn: parent
        spacing: 10

        Repeater {
            model: [0.45, 0.75, 0.35, 0.60]
            Rectangle {
                width: 22
                height: parent.parent.height * modelData
                color: "#60a5fa"
                anchors.bottom: parent.bottom
            }
        }
    }
}
