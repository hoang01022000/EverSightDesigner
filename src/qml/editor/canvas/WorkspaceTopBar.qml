import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Rectangle {
    id: root

    color: "#17191c"
    border.color: "#2d3338"

    RowLayout {
        anchors.fill: parent
        anchors.margins: 2
        spacing: 2

        ToolButton {
            text: "-"
            Layout.fillWidth: true
            onClicked: Window.window.showMinimized()
        }

        ToolButton {
            text: "[]"
            Layout.fillWidth: true
            onClicked: Window.window.visibility === Window.Maximized
                       ? Window.window.showNormal()
                       : Window.window.showMaximized()
        }

        ToolButton {
            text: "X"
            Layout.fillWidth: true
            onClicked: Window.window.close()
        }
    }
}
