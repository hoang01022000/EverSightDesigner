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
            Layout.fillWidth: true
            zoomPercent: root.zoomPercent

            onClearRequested: canvasViewModel.clear()
            onDeleteRequested: canvasViewModel.removeSelectedWidget()
            onDuplicateRequested: canvasViewModel.duplicateSelectedWidget()
            onForwardRequested: canvasViewModel.moveSelectedForward()
            onBackwardRequested: canvasViewModel.moveSelectedBackward()
            onZoomChanged: function(value) {
                root.zoomPercent = value
            }
            onSaveRequested: {
                statusText.text = canvasViewModel.saveToFile("runtime_layout.json")
                        ? "Saved runtime_layout.json"
                        : "Save failed"
            }
            onLoadRequested: {
                statusText.text = canvasViewModel.loadFromFile("runtime_layout.json")
                        ? "Loaded runtime_layout.json"
                        : "No saved layout found"
            }
            onPreviewRequested: statusText.text = "Preview mode is ready"
            onExportRequested: statusText.text = canvasViewModel.saveToFile("runtime_export.json")
                    ? "Exported runtime_export.json"
                    : "Export failed"
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

            DesignCanvas {
                id: designCanvas
                Layout.fillWidth: true
                Layout.fillHeight: true
                zoom: root.zoomPercent / 100
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
