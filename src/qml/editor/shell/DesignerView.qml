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
            onBringToFrontRequested: canvasViewModel.bringSelectedToFront()
            onBringForwardRequested: canvasViewModel.moveSelectedForward()
            onSendToBackRequested: canvasViewModel.sendSelectedToBack()
            onSendBackwardRequested: canvasViewModel.moveSelectedBackward()

            // Alignment
            onAlignLeftRequested: canvasViewModel.alignSelected("left", designCanvas.designWidth, designCanvas.designHeight)
            onAlignCenterRequested: canvasViewModel.alignSelected("hcenter", designCanvas.designWidth, designCanvas.designHeight)
            onAlignRightRequested: canvasViewModel.alignSelected("right", designCanvas.designWidth, designCanvas.designHeight)
            onAlignTopRequested: canvasViewModel.alignSelected("top", designCanvas.designWidth, designCanvas.designHeight)
            onAlignMiddleRequested: canvasViewModel.alignSelected("vcenter", designCanvas.designWidth, designCanvas.designHeight)
            onAlignBottomRequested: canvasViewModel.alignSelected("bottom", designCanvas.designWidth, designCanvas.designHeight)

            // Distribution
            onDistributeHorizontalRequested: canvasViewModel.distributeSelectedHorizontal()
            onDistributeVerticalRequested: canvasViewModel.distributeSelectedVertical()

            // Edit
            onUndoRequested: canvasViewModel.undo()
            onRedoRequested: canvasViewModel.redo()
            onDeleteRequested: canvasViewModel.removeSelectedWidget()
            onDuplicateRequested: canvasViewModel.duplicateSelectedWidget()

            // View
            onZoomChanged: function(value) { root.zoomPercent = value }
            onFitToWindowRequested: root.zoomPercent = canvasViewModel.fittedZoomPercent(
                                        designCanvas.width,
                                        designCanvas.height,
                                        designCanvas.designWidth,
                                        designCanvas.designHeight,
                                        designCanvas.viewportHorizontalPadding,
                                        designCanvas.viewportVerticalPadding)

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
            function onSelectedWidgetChanged() {
                topBar.updateActionStates(canvasViewModel.selectedCount,
                                          canvasViewModel.canUndo,
                                          canvasViewModel.canRedo)
            }
            function onWidgetCountChanged() {
                topBar.updateActionStates(canvasViewModel.selectedCount,
                                          canvasViewModel.canUndo,
                                          canvasViewModel.canRedo)
            }
            function onUndoRedoChanged() {
                topBar.updateActionStates(canvasViewModel.selectedCount,
                                          canvasViewModel.canUndo,
                                          canvasViewModel.canRedo)
            }
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
