import QtQuick

Rectangle {
    property string bgColor: "#2f3438"
    property string borderColor: "#52585f"
    property string fontColor: "#f5f5f5"
    property string labelText: "Label"

    color: bgColor
    radius: 4
    border.color: borderColor
    border.width: 1

    Text {
        anchors.centerIn: parent
        text: labelText
        color: fontColor
        font.pixelSize: 12
        elide: Text.ElideRight
        width: parent.width - 8
        horizontalAlignment: Text.AlignHCenter
    }
}
