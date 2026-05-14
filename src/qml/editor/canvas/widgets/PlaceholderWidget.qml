import QtQuick

Rectangle {
    property string typeName: "Widget"
    property string bgColor: "#2f2f2f"
    property string borderColor: "#272727"
    property string fontColor: "#f6f6f6"

    color: bgColor
    radius: 4
    border.color: borderColor
    border.width: 1

    Text {
        anchors.centerIn: parent
        text: typeName
        color: fontColor
        font.pixelSize: 11
    }
}
