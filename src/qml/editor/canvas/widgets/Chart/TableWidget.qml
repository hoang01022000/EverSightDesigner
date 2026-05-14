import QtQuick

Rectangle {
    property string bgColor: "#32363a"
    property string borderColor: "#cbd5e1"
    property string fontColor: "#e8ecf0"
    property int rows: 3
    property int columns: 3

    color: bgColor
    radius: 4
    border.color: borderColor
    border.width: 1

    Grid {
        anchors.centerIn: parent
        rows: parent.rows
        columns: parent.columns
        spacing: 2

        Repeater {
            model: parent.parent.rows * parent.parent.columns

            Rectangle {
                width: Math.max(16, (parent.parent.width - 16) / parent.parent.columns)
                height: 14
                color: "transparent"
                border.color: parent.parent.borderColor
                border.width: 1
                radius: 1
            }
        }
    }
}
