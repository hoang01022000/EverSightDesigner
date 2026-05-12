import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Rectangle {
    id: root

    property int zoomPercent: 84

    signal previewRequested()
    signal exportRequested()
    signal saveRequested()
    signal loadRequested()
    signal clearRequested()
    signal duplicateRequested()
    signal deleteRequested()
    signal forwardRequested()
    signal backwardRequested()
    signal zoomChanged(int value)

    color: "#f4f4f4"
    implicitHeight: 78

    Rectangle {
        id: titleBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 34
        color: "#242424"

        Label {
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            text: "Run-time Interface Design"
            color: "#ffffff"
            font.pixelSize: 15
            font.bold: true
        }

        MouseArea {
            anchors.left: parent.left
            anchors.right: windowButtons.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            acceptedButtons: Qt.LeftButton
            onPressed: {
                if (Window.window)
                    Window.window.startSystemMove()
            }
            onDoubleClicked: {
                if (!Window.window)
                    return

                if (Window.window.visibility === Window.Maximized)
                    Window.window.showNormal()
                else
                    Window.window.showMaximized()
            }
        }

        Row {
            id: windowButtons
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            spacing: 0

            WindowButton {
                width: 46
                height: parent.height
                label: "-"
                tooltip: "Minimize"
                onClicked: Window.window.showMinimized()
            }

            WindowButton {
                width: 46
                height: parent.height
                label: Window.window && Window.window.visibility === Window.Maximized ? "[]" : "[ ]"
                tooltip: "Maximize"
                onClicked: {
                    if (Window.window.visibility === Window.Maximized)
                        Window.window.showNormal()
                    else
                        Window.window.showMaximized()
                }
            }

            WindowButton {
                width: 46
                height: parent.height
                label: "X"
                tooltip: "Close"
                danger: true
                onClicked: Window.window.close()
            }
        }
    }

    RowLayout {
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        spacing: 10

        ToolButton { text: "Open"; ToolTip.visible: hovered; ToolTip.text: "Load layout"; onClicked: root.loadRequested() }
        ToolButton { text: "Save"; ToolTip.visible: hovered; ToolTip.text: "Save layout"; onClicked: root.saveRequested() }

        ToolSeparator {}

        ToolButton { text: "Copy"; ToolTip.visible: hovered; ToolTip.text: "Duplicate selected control"; onClicked: root.duplicateRequested() }
        ToolButton { text: "Up"; ToolTip.visible: hovered; ToolTip.text: "Bring forward"; onClicked: root.forwardRequested() }
        ToolButton { text: "Down"; ToolTip.visible: hovered; ToolTip.text: "Send backward"; onClicked: root.backwardRequested() }

        ToolSeparator {}

        ToolButton { text: "Undo"; enabled: false; ToolTip.visible: hovered; ToolTip.text: "Undo" }
        ToolButton { text: "Redo"; enabled: false; ToolTip.visible: hovered; ToolTip.text: "Redo" }

        ToolSeparator {}

        ToolButton { text: "Delete"; ToolTip.visible: hovered; ToolTip.text: "Delete selected control"; onClicked: root.deleteRequested() }
        ToolButton { text: "Clear"; ToolTip.visible: hovered; ToolTip.text: "Clear canvas"; onClicked: root.clearRequested() }

        ToolSeparator {}

        Slider {
            Layout.preferredWidth: 150
            from: 40
            to: 160
            stepSize: 1
            value: root.zoomPercent
            onMoved: root.zoomChanged(Math.round(value))
        }

        Label {
            text: root.zoomPercent + "%"
            color: "#707780"
            font.pixelSize: 14
        }

        ToolSeparator {}

        ToolButton { text: "Settings"; ToolTip.visible: hovered; ToolTip.text: "Screen settings" }

        Item {
            Layout.fillWidth: true
        }

        Button {
            text: "Preview"
            onClicked: root.previewRequested()
        }

        Button {
            text: "Export"
            onClicked: root.exportRequested()
        }
    }

    component WindowButton: Rectangle {
        id: buttonRoot

        property string label: ""
        property string tooltip: ""
        property bool danger: false

        signal clicked()

        color: mouseArea.containsMouse ? (danger ? "#c93636" : "#3a3a3a") : "transparent"

        Label {
            anchors.centerIn: parent
            text: buttonRoot.label
            color: "#f0f0f0"
            font.pixelSize: 14
            font.bold: true
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: buttonRoot.clicked()
        }

        ToolTip.visible: mouseArea.containsMouse
        ToolTip.text: buttonRoot.tooltip
    }
}
