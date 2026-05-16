import QtQuick

Rectangle {
    property string bgColor: "#1f3d60"
    property string borderColor: "#2d5a8a"
    property string fontColor: "#f3f9ff"
    property string controlType: "Both Display"

    color: bgColor
    radius: 4
    border.color: borderColor
    border.width: 1

    Row {
        anchors.centerIn: parent
        spacing: 6

        Rectangle {
            visible: controlType !== "Continuously"
            width: 36
            height: 22
            radius: 3
            color: "#2d6a4f"

            Text {
                anchors.centerIn: parent
                text: "▶"
                color: "#ffffff"
                font.pixelSize: 12
            }
        }

        Rectangle {
            visible: controlType !== "Once"
            width: 36
            height: 22
            radius: 3
            color: "#1a4a7a"

            Text {
                anchors.centerIn: parent
                text: "⏹"
                color: "#ffffff"
                font.pixelSize: 12
            }
        }
    }
}
