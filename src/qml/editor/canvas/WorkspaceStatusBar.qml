import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    property string regionText: "Ready"

    color: "#151719"
    border.color: "#2d3338"

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 10

        Label {
            text: root.regionText
            color: "#9fa8b1"
            font.pixelSize: 11
            Layout.fillWidth: true
        }

        Label {
            text: "Industrial editor"
            color: "#7d8790"
            font.pixelSize: 11
        }
    }
}
