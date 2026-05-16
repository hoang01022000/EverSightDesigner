import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import EverSightDesigner

Item {
    id: root
    property int zoomPercent: 100
    property bool fitToWindowMode: true
    property string currentFilePath: ""
    property string currentFileName: currentFilePath.length > 0 ? fileNameFromPath(currentFilePath) : "Untitled"

    function filePathFromUrl(fileUrl) {
        if (!fileUrl)
            return ""
        return fileUrl.toLocalFile ? fileUrl.toLocalFile() : String(fileUrl).replace("file:///", "")
    }

    function fileNameFromPath(path) {
        var normalized = String(path).replace(/\\/g, "/")
        var index = normalized.lastIndexOf("/")
        return index >= 0 ? normalized.slice(index + 1) : normalized
    }

    function ensureJsonPath(path) {
        return path.toLowerCase().endsWith(".json") ? path : path + ".json"
    }

    function fitCanvasToWindow() {
        if (!designCanvas || designCanvas.width <= 0 || designCanvas.height <= 0)
            return
        root.zoomPercent = canvasViewModel.fittedZoomPercent(
                    designCanvas.width,
                    designCanvas.height,
                    designCanvas.designWidth,
                    designCanvas.designHeight,
                    designCanvas.viewportHorizontalPadding,
                    designCanvas.viewportVerticalPadding)
    }

    Component.onCompleted: Qt.callLater(root.fitCanvasToWindow)

    ColumnLayout {
        id: editorShell

        anchors.fill: parent
        spacing: 0
        visible: !canvasViewModel.previewMode
        enabled: !canvasViewModel.previewMode

        TopBar {
            id: topBar
            Layout.fillWidth: true
            zoomPercent: root.zoomPercent
            documentTitle: root.currentFileName
            previewMode: canvasViewModel.previewMode

            // File
            onOpenRequested: openFileDialog.open()
            onSaveRequested: {
                if (root.currentFilePath.length > 0) {
                    statusText.text = canvasViewModel.saveToFile(root.currentFilePath)
                            ? "Saved " + root.currentFileName
                            : "Save failed"
                } else {
                    saveFileDialog.open()
                }
            }
            onSaveAsRequested: saveFileDialog.open()
            onTemplateManagerRequested: statusText.text = "Template manager requested"
            onSaveTemplateRequested: statusText.text = "Save template requested"
            onCanvasSizeRequested: canvasSizeDialog.open()

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
            onZoomChanged: function(value) {
                root.fitToWindowMode = false
                root.zoomPercent = value
            }
            onFitToWindowRequested: {
                root.fitToWindowMode = true
                root.fitCanvasToWindow()
            }

            // Fixed area
            onToggleTopRequested: {
                var visible = !canvasViewModel.topFixedVisible
                canvasViewModel.setFixedBar("top", visible)
                fixedBarViewModel.setTopVisible(visible)
            }
            onToggleBottomRequested: {
                var visible = !canvasViewModel.bottomFixedVisible
                canvasViewModel.setFixedBar("bottom", visible)
                fixedBarViewModel.setBottomVisible(visible)
            }
            onToggleLeftRequested: {
                var visible = !canvasViewModel.leftFixedVisible
                canvasViewModel.setFixedBar("left", visible)
                fixedBarViewModel.setLeftVisible(visible)
            }
            onToggleRightRequested: {
                var visible = !canvasViewModel.rightFixedVisible
                canvasViewModel.setFixedBar("right", visible)
                fixedBarViewModel.setRightVisible(visible)
            }

            // Runtime
            onPreviewRequested: {
                canvasViewModel.enterPreview()
                statusText.text = "Preview mode"
            }
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
            function onCanvasSizeChanged() {
                if (root.fitToWindowMode)
                    Qt.callLater(root.fitCanvasToWindow)
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
                onWidthChanged: if (root.fitToWindowMode) Qt.callLater(root.fitCanvasToWindow)
                onHeightChanged: if (root.fitToWindowMode) Qt.callLater(root.fitCanvasToWindow)
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

    RuntimePreviewSurface {
        anchors.fill: parent
        visible: canvasViewModel.previewMode
        enabled: canvasViewModel.previewMode

        onExitRequested: {
            canvasViewModel.exitPreview()
            statusText.text = "Editor mode"
        }
    }

    FileDialog {
        id: openFileDialog
        title: "Open Layout"
        fileMode: FileDialog.OpenFile
        nameFilters: ["EverSight layout (*.json)"]

        onAccepted: {
            var path = root.filePathFromUrl(selectedFile)
            if (canvasViewModel.loadFromFile(path)) {
                root.currentFilePath = path
                statusText.text = "Loaded " + root.currentFileName
                if (root.fitToWindowMode)
                    Qt.callLater(root.fitCanvasToWindow)
            } else {
                statusText.text = "Open failed"
            }
        }
    }

    FileDialog {
        id: saveFileDialog
        title: "Save Layout"
        fileMode: FileDialog.SaveFile
        nameFilters: ["EverSight layout (*.json)"]
        currentFile: root.currentFilePath.length > 0 ? Qt.resolvedUrl(root.currentFilePath) : Qt.resolvedUrl("untitled.json")

        onAccepted: {
            var path = root.ensureJsonPath(root.filePathFromUrl(selectedFile))
            if (canvasViewModel.saveToFile(path)) {
                root.currentFilePath = path
                statusText.text = "Saved " + root.currentFileName
            } else {
                statusText.text = "Save failed"
            }
        }
    }

    Dialog {
        id: canvasSizeDialog
        title: "Canvas Size"
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel

        x: Math.round((root.width - width) / 2)
        y: 96
        width: 280

        contentItem: GridLayout {
            columns: 2
            rowSpacing: 8
            columnSpacing: 8

            Label { text: "Width" }
            TextField {
                id: canvasWidthInput
                text: Math.round(canvasViewModel.canvasWidth)
                validator: DoubleValidator { bottom: 320 }
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                Layout.fillWidth: true
            }

            Label { text: "Height" }
            TextField {
                id: canvasHeightInput
                text: Math.round(canvasViewModel.canvasHeight)
                validator: DoubleValidator { bottom: 240 }
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                Layout.fillWidth: true
            }
        }

        onOpened: {
            canvasWidthInput.text = Math.round(canvasViewModel.canvasWidth)
            canvasHeightInput.text = Math.round(canvasViewModel.canvasHeight)
            canvasWidthInput.forceActiveFocus()
            canvasWidthInput.selectAll()
        }

        onAccepted: {
            canvasViewModel.setCanvasSize(Number(canvasWidthInput.text), Number(canvasHeightInput.text))
            if (root.fitToWindowMode)
                Qt.callLater(root.fitCanvasToWindow)
        }
    }
}
