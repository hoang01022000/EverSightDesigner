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
        anchors.leftMargin: 14
        anchors.rightMargin: 8
        spacing: 8

        Label {
            text: "Run-time Interface Design"
            color: "#f1f4f7"
            font.pixelSize: 18
            font.bold: true
            Layout.fillWidth: true
        }

        ToolButton {
            text: "Run"
            onClicked: console.log("Run requested")
        }

        ToolButton {
            text: "Refresh"
            onClicked: console.log("Refresh requested")
        }

        ToolButton {
            text: "Save Scheme"
            onClicked: canvasViewModel.saveToFile("runtime_layout.json")
        }

        ToolButton {
            text: "-"
            onClicked: Window.window.showMinimized()
        }

        ToolButton {
            text: "□"
            onClicked: Window.window.visibility === Window.Maximized
                       ? Window.window.showNormal()
                       : Window.window.showMaximized()
        }

        ToolButton {
            text: "×"
            onClicked: Window.window.close()
        }
    }
}
