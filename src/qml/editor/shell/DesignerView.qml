import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import EverSightDesigner

Item {
    id: root
    property int zoomPercent: 84

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TopBar {
            id: topBar
            Layout.fillWidth: true
            zoomPercent: root.zoomPercent

            // File
            onOpenRequested: {
                statusText.text = canvasViewModel.loadFromFile("runtime_layout.json")
                        ? "Loaded runtime_layout.json"
                        : "No saved layout found"
            }
            onSaveRequested: {
                statusText.text = canvasViewModel.saveToFile("runtime_layout.json")
                        ? "Saved runtime_layout.json"
                        : "Save failed"
            }
            onTemplateManagerRequested: statusText.text = "Template manager requested"
            onSaveTemplateRequested: statusText.text = "Save template requested"

            // Layer
            onBringToFrontRequested: statusText.text = "Bring to front requested"
            onBringForwardRequested: canvasViewModel.moveSelectedForward()
            onSendToBackRequested: statusText.text = "Send to back requested"
            onSendBackwardRequested: canvasViewModel.moveSelectedBackward()

            // Alignment
            onAlignLeftRequested: canvasViewModel.alignSelected("left", designCanvas.width, designCanvas.height)
            onAlignCenterRequested: canvasViewModel.alignSelected("hcenter", designCanvas.width, designCanvas.height)
            onAlignRightRequested: canvasViewModel.alignSelected("right", designCanvas.width, designCanvas.height)
            onAlignTopRequested: canvasViewModel.alignSelected("top", designCanvas.width, designCanvas.height)
            onAlignMiddleRequested: canvasViewModel.alignSelected("vcenter", designCanvas.width, designCanvas.height)
            onAlignBottomRequested: canvasViewModel.alignSelected("bottom", designCanvas.width, designCanvas.height)

            // Distribution
            onDistributeHorizontalRequested: statusText.text = "Distribute horizontally requested"
            onDistributeVerticalRequested: statusText.text = "Distribute vertically requested"

            // Edit
            onUndoRequested: statusText.text = "Undo requested"
            onRedoRequested: statusText.text = "Redo requested"
            onDeleteRequested: canvasViewModel.removeSelectedWidget()
            onDuplicateRequested: canvasViewModel.duplicateSelectedWidget()

            // View
            onZoomChanged: function(value) { root.zoomPercent = value }
            onFitToWindowRequested: statusText.text = "Fit to window requested"

            // Fixed area
            onToggleTopRequested: fixedBarViewModel.setTopVisible(!fixedBarViewModel.topVisible)
            onToggleBottomRequested: fixedBarViewModel.setBottomVisible(!fixedBarViewModel.bottomVisible)
            onToggleLeftRequested: fixedBarViewModel.setLeftVisible(!fixedBarViewModel.leftVisible)
            onToggleRightRequested: fixedBarViewModel.setRightVisible(!fixedBarViewModel.rightVisible)

            // Runtime
            onPreviewRequested: statusText.text = "Preview mode is ready"
            onExportRequested: statusText.text = canvasViewModel.saveToFile("runtime_export.json")
                    ? "Exported runtime_export.json"
                    : "Export failed"
        }

        // Keep toolbar enabled state in sync with selection
        Connections {
            target: canvasViewModel
            onSelectedWidgetChanged: topBar.updateActionStates(canvasViewModel.hasSelection ? 1 : 0)
            onWidgetCountChanged: topBar.updateActionStates(canvasViewModel.hasSelection ? 1 : 0)
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            PalettePanel {

                onWidgetRequested: function(type) {
                    canvasViewModel.addWidget(type)
                    statusText.text = type + " added"
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 0

                Rectangle { // Top fixed bar area
                    id: topFixedBar
                    color: "#0f1724"
                    Layout.fillWidth: true
                    implicitHeight: fixedBarViewModel.topVisible ? fixedBarViewModel.topHeight : 0
                    height: implicitHeight
                    visible: fixedBarViewModel.topVisible

                    Label {
                        anchors.centerIn: parent
                        text: "Top Fixed Bar"
                        color: "#cbd5e1"
                        font.pixelSize: 14
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 0

                    Rectangle { // Left fixed bar
                        id: leftFixedBar
                        color: "#0b1220"
                        Layout.preferredWidth: fixedBarViewModel.leftVisible ? fixedBarViewModel.leftWidth : 0
                        width: Layout.preferredWidth
                        visible: fixedBarViewModel.leftVisible
                    }

                    DesignCanvas {
                        id: designCanvas
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        zoom: root.zoomPercent / 100
                    }

                    Rectangle { // Right fixed bar
                        id: rightFixedBar
                        color: "#0b1220"
                        Layout.preferredWidth: fixedBarViewModel.rightVisible ? fixedBarViewModel.rightWidth : 0
                        width: Layout.preferredWidth
                        visible: fixedBarViewModel.rightVisible
                    }
                }

                Rectangle { // Bottom fixed bar
                    id: bottomFixedBar
                    color: "#0f1724"
                    Layout.fillWidth: true
                    implicitHeight: fixedBarViewModel.bottomVisible ? fixedBarViewModel.bottomHeight : 0
                    height: implicitHeight
                    visible: fixedBarViewModel.bottomVisible

                    Row {
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 12

                        Label {
                            text: "Device: Connected"
                            color: "#9ca3af"
                        }

                        Label {
                            text: "Status: OK"
                            color: "#9ca3af"
                        }
                    }
                }
            }

            InspectorPanel {
                Layout.fillHeight: true
            }
        }

        Rectangle {
            Layout.fillWidth: true
            implicitHeight: 28
            color: "#171923"

            Label {
                id: statusText
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 12
                text: "Ready"
                color: "#9ca3af"
                font.pixelSize: 12
            }
        }
    }
}
