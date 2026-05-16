import QtQuick

Rectangle {
    property string bgColor: "#2c3239"
    property string borderColor: "#515960"
    property string fontColor: "#e8ecf0"
    property string labelText: "Param Array"

    color: bgColor
    border.color: borderColor
    border.width: 1
    radius: 3

    Column {
        anchors.centerIn: parent
        spacing: 6
        Repeater {
            model: 3
            Rectangle {
                width: 160
                height: 24
                color: "#20262d"
                border.color: borderColor
            }
        }
    }
}
