import QtQuick

Rectangle {
    property string bgColor: "#212529"
    property string borderColor: "#4e5b68"
    property bool isActive: true

    color: bgColor
    radius: 4
    border.color: borderColor
    border.width: 1

    Rectangle {
        width: Math.min(parent.width, parent.height) * 0.5
        height: width
        radius: width / 2
        anchors.centerIn: parent
        color: isActive ? "#27ae60" : "#e74c3c"
    }
}
