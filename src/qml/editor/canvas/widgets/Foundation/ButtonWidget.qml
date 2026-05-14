// ButtonWidget.qml
import QtQuick

Rectangle {
    // Các thuộc tính này sẽ được map từ model
    property string bgColor: "#2f2f2f"
    property string borderColorValue: "#272727"
    property string textColor: "#f6f6f6"
    property string text: "Button"

    color: bgColor
    border.color: borderColorValue
    border.width: 1
    radius: 4

    Text {
        anchors.centerIn: parent
        text: parent.text
        color: parent.textColor
        font.pixelSize: 12
    }
}