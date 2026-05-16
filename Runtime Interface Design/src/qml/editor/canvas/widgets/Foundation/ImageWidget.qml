import QtQuick

Rectangle {
    property string bgColor: "#282c2f"
    property string borderColor: "#474b50"
    property string fontColor: "#dbe0e6"

    color: bgColor
    radius: 4
    border.color: borderColor
    border.width: 1

    Column {
        anchors.centerIn: parent
        spacing: 4

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "[ IMG ]"
            color: fontColor
            font.pixelSize: 13
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "No source"
            color: fontColor
            font.pixelSize: 9
            opacity: 0.6
        }
    }
}
