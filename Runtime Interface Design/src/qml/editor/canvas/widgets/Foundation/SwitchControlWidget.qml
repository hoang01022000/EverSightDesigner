import QtQuick

Rectangle {
    property string bgColor: "#3d5a7f"
    property string borderColor: "#5b7d9b"
    property string fontColor: "#f0f4f8"
    property string labelText: "Switch"
    property bool checked: false

    color: bgColor
    border.color: borderColor
    border.width: 1
    radius: 6

    Rectangle {
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        width: Math.min(parent.width * 0.34, 48)
        height: Math.min(parent.height * 0.48, 24)
        radius: height / 2
        color: checked ? "#35b779" : "#56616d"

        Rectangle {
            width: parent.height - 6
            height: width
            radius: width / 2
            anchors.verticalCenter: parent.verticalCenter
            x: checked ? parent.width - width - 3 : 3
            color: "#ffffff"
        }
    }

    Text {
        anchors.centerIn: parent
        text: labelText
        color: fontColor
        font.pixelSize: 13
        font.bold: true
    }
}
