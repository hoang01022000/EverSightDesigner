import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Rectangle {
    id: root

    property int zoomPercent: 84
    property string documentTitle: "Untitled"
    property bool previewMode: false

    signal openRequested()
    signal saveRequested()
    signal saveAsRequested()
    signal templateManagerRequested()
    signal saveTemplateRequested()
    signal canvasSizeRequested()

    signal bringToFrontRequested()
    signal bringForwardRequested()
    signal sendToBackRequested()
    signal sendBackwardRequested()

    signal alignLeftRequested()
    signal alignCenterRequested()
    signal alignRightRequested()
    signal alignTopRequested()
    signal alignMiddleRequested()
    signal alignBottomRequested()

    signal distributeHorizontalRequested()
    signal distributeVerticalRequested()

    signal undoRequested()
    signal redoRequested()
    signal deleteRequested()

    signal zoomChanged(int value)
    signal fitToWindowRequested()

    signal toggleTopRequested()
    signal toggleBottomRequested()
    signal toggleLeftRequested()
    signal toggleRightRequested()

    signal previewRequested()
    signal exportRequested()
    signal clearRequested()
    signal duplicateRequested()
    signal forwardRequested()
    signal backwardRequested()

    color: "#f7f7f7"
    implicitHeight: 64

    Action { id: actOpen; text: "Open"; shortcut: "Ctrl+O"; onTriggered: root.openRequested() }
    Action { id: actSave; text: "Save"; shortcut: "Ctrl+S"; onTriggered: root.saveRequested() }
    Action { id: actSaveAs; text: "Save As"; shortcut: "Ctrl+Shift+S"; onTriggered: root.saveAsRequested() }
    Action { id: actTemplateManager; text: "Template Manager"; onTriggered: root.templateManagerRequested() }
    Action { id: actSaveTemplate; text: "Save Template"; onTriggered: root.saveTemplateRequested() }
    Action { id: actCanvasSize; text: "Canvas Size"; onTriggered: root.canvasSizeRequested() }

    Action { id: actBringToFront; text: "Bring to Front"; enabled: false; onTriggered: root.bringToFrontRequested() }
    Action { id: actBringForward; text: "Bring Forward"; enabled: false; onTriggered: root.bringForwardRequested() }
    Action { id: actSendToBack; text: "Send to Back"; enabled: false; onTriggered: root.sendToBackRequested() }
    Action { id: actSendBackward; text: "Send Backward"; enabled: false; onTriggered: root.sendBackwardRequested() }

    Action { id: actAlignLeft; text: "Align Left"; enabled: false; onTriggered: root.alignLeftRequested() }
    Action { id: actAlignCenter; text: "Align Center"; enabled: false; onTriggered: root.alignCenterRequested() }
    Action { id: actAlignRight; text: "Align Right"; enabled: false; onTriggered: root.alignRightRequested() }
    Action { id: actAlignTop; text: "Align Top"; enabled: false; onTriggered: root.alignTopRequested() }
    Action { id: actAlignMiddle; text: "Align Middle"; enabled: false; onTriggered: root.alignMiddleRequested() }
    Action { id: actAlignBottom; text: "Align Bottom"; enabled: false; onTriggered: root.alignBottomRequested() }

    Action { id: actDistributeH; text: "Distribute Horizontally"; enabled: false; onTriggered: root.distributeHorizontalRequested() }
    Action { id: actDistributeV; text: "Distribute Vertically"; enabled: false; onTriggered: root.distributeVerticalRequested() }

    Action { id: actUndo; text: "Undo"; shortcut: "Ctrl+Z"; enabled: false; onTriggered: root.undoRequested() }
    Action { id: actRedo; text: "Redo"; shortcut: "Ctrl+Y"; enabled: false; onTriggered: root.redoRequested() }
    Action { id: actDelete; text: "Delete"; shortcut: "Del"; enabled: false; onTriggered: root.deleteRequested() }
    Action { id: actFitToWindow; text: "Fit to Window"; onTriggered: root.fitToWindowRequested() }

    Action { id: actToggleTop; text: "Toggle Top Fixed Area"; onTriggered: root.toggleTopRequested() }
    Action { id: actToggleBottom; text: "Toggle Bottom Fixed Area"; onTriggered: root.toggleBottomRequested() }
    Action { id: actToggleLeft; text: "Toggle Left Fixed Area"; onTriggered: root.toggleLeftRequested() }
    Action { id: actToggleRight; text: "Toggle Right Fixed Area"; onTriggered: root.toggleRightRequested() }

    Action { id: actPreview; text: root.previewMode ? "Edit" : "Preview"; onTriggered: root.previewRequested() }
    Action { id: actExport; text: "Export"; onTriggered: root.exportRequested() }

    Rectangle {
        id: titleBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 28
        color: "#242424"

        Label {
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            text: "Run-time Interface Design - " + root.documentTitle
            color: "#ffffff"
            font.pixelSize: 13
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

            WindowButton { width: 42; height: parent.height; label: "-"; tooltip: "Minimize"; onClicked: Window.window.showMinimized() }
            WindowButton {
                width: 42
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
            WindowButton { width: 42; height: parent.height; label: "X"; tooltip: "Close"; danger: true; onClicked: Window.window.close() }
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleBar.bottom
        anchors.bottom: parent.bottom
        color: "#f7f7f7"
        border.color: "#d2d2d2"
        border.width: 1

        Flickable {
            anchors.left: parent.left
            anchors.right: runtimeButtons.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 8
            anchors.rightMargin: 12
            contentWidth: toolbarRow.implicitWidth
            contentHeight: height
            flickableDirection: Flickable.HorizontalFlick
            boundsBehavior: Flickable.StopAtBounds
            clip: true

            Row {
                id: toolbarRow
                height: parent.height
                spacing: 5

                IconButton { action: actOpen; iconSource: topBarViewModel.openIcon; fallbackText: "O" }
                IconButton { action: actSave; iconSource: topBarViewModel.saveIcon; fallbackText: "S" }
                IconButton { action: actSaveAs; fallbackText: "SA"; glyph: "\u21e7" }
                IconButton { action: actTemplateManager; iconSource: topBarViewModel.templateIcon; fallbackText: "TM"; glyph: "\u25a3" }
                IconButton { action: actSaveTemplate; iconSource: topBarViewModel.saveTemplateIcon; fallbackText: "ST"; glyph: "\u229e" }
                IconButton { action: actCanvasSize; fallbackText: "CS"; glyph: "\u25a1" }

                ToolbarDivider {}

                IconButton { action: actBringToFront; iconSource: topBarViewModel.bringToFrontIcon; fallbackText: "FF" }
                IconButton { action: actBringForward; iconSource: topBarViewModel.bringForwardIcon; fallbackText: "F"; glyph: "\u25c6" }
                IconButton { action: actSendToBack; iconSource: topBarViewModel.sendToBackIcon; fallbackText: "BB"; glyph: "\u25c7" }
                IconButton { action: actSendBackward; iconSource: topBarViewModel.sendBackwardIcon; fallbackText: "B"; glyph: "\u25c8" }

                ToolbarDivider {}

                IconButton { action: actAlignLeft; iconSource: topBarViewModel.alignLeftIcon; fallbackText: "L"; glyph: "\u22a2" }
                IconButton { action: actAlignCenter; iconSource: topBarViewModel.alignCenterIcon; fallbackText: "C"; glyph: "\u2261" }
                IconButton { action: actAlignRight; iconSource: topBarViewModel.alignRightIcon; fallbackText: "R"; glyph: "\u22a3" }
                IconButton { action: actAlignTop; iconSource: topBarViewModel.alignTopIcon; fallbackText: "T"; glyph: "\u22a4" }
                IconButton { action: actAlignMiddle; iconSource: topBarViewModel.alignMiddleIcon; fallbackText: "M"; glyph: "\u2501" }
                IconButton { action: actAlignBottom; iconSource: topBarViewModel.alignBottomIcon; fallbackText: "D"; glyph: "\u22a5" }

                ToolbarDivider {}

                IconButton { action: actDistributeH; iconSource: topBarViewModel.distributeHIcon; fallbackText: "DH"; glyph: "\u2194" }
                IconButton { action: actDistributeV; iconSource: topBarViewModel.distributeVIcon; fallbackText: "DV"; glyph: "\u2195" }

                ToolbarDivider {}

                IconButton { action: actUndo; iconSource: topBarViewModel.undoIcon; fallbackText: "U" }
                IconButton { action: actRedo; iconSource: topBarViewModel.redoIcon; fallbackText: "R" }
                IconButton { action: actDelete; iconSource: topBarViewModel.deleteIcon; fallbackText: "Del" }
                IconButton { action: actFitToWindow; fallbackText: "Fit"; glyph: "\u25a3" }

                ToolbarDivider {}

                Slider {
                    id: zoomSlider
                    anchors.verticalCenter: parent.verticalCenter
                    width: 118
                    height: 28
                    from: 40
                    to: 160
                    stepSize: 1
                    value: root.zoomPercent
                    onMoved: root.zoomChanged(Math.round(value))
                }

                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    width: 44
                    text: root.zoomPercent + "%"
                    color: "#6f7479"
                    font.pixelSize: 12
                }

                ToolbarDivider {}

                IconButton { action: actToggleTop; iconSource: topBarViewModel.toggleTopIcon; fallbackText: "Top"; glyph: "\u25ac" }
                IconButton { action: actToggleBottom; iconSource: topBarViewModel.toggleBottomIcon; fallbackText: "Bot"; glyph: "\u25ad" }
                IconButton { action: actToggleLeft; iconSource: topBarViewModel.toggleLeftIcon; fallbackText: "Left"; glyph: "\u25e7" }
                IconButton { action: actToggleRight; iconSource: topBarViewModel.toggleRightIcon; fallbackText: "Right"; glyph: "\u25e8" }

            }
        }

        Row {
            id: runtimeButtons
            anchors.right: parent.right
            anchors.rightMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            spacing: 6

            TextButton { action: actPreview }
            TextButton { action: actExport }
        }
    }

    function updateActionStates(selectedCount, canUndo, canRedo) {
        var hasSelection = selectedCount > 0
        var multipleSelection = selectedCount > 1

        actBringToFront.enabled = hasSelection
        actBringForward.enabled = hasSelection
        actSendToBack.enabled = hasSelection
        actSendBackward.enabled = hasSelection
        actDelete.enabled = hasSelection

        actAlignLeft.enabled = hasSelection
        actAlignCenter.enabled = hasSelection
        actAlignRight.enabled = hasSelection
        actAlignTop.enabled = hasSelection
        actAlignMiddle.enabled = hasSelection
        actAlignBottom.enabled = hasSelection

        actDistributeH.enabled = multipleSelection
        actDistributeV.enabled = multipleSelection
        actUndo.enabled = canUndo
        actRedo.enabled = canRedo
    }

    component IconButton: ToolButton {
        id: button

        property string iconSource: ""
        property string fallbackText: ""
        property string glyph: ""

        width: 28
        height: 28
        anchors.verticalCenter: parent ? parent.verticalCenter : undefined
        padding: 0
        hoverEnabled: true

        background: Rectangle {
            radius: 2
            color: button.down ? "#d9dde1" : button.hovered ? "#eef1f4" : "transparent"
            border.color: button.hovered ? "#b8bec5" : "transparent"
        }

        contentItem: Item {
            opacity: button.enabled ? 1.0 : 0.35

            Image {
                id: iconImage
                anchors.centerIn: parent
                width: 20
                height: 20
                source: button.iconSource
                fillMode: Image.PreserveAspectFit
                smooth: true
                visible: button.iconSource.length > 0 && status === Image.Ready
            }

            Label {
                anchors.centerIn: parent
                text: button.glyph.length > 0 ? button.glyph : button.fallbackText
                color: "#6e747a"
                font.pixelSize: button.glyph.length > 0 ? 18 : 10
                font.bold: button.glyph.length === 0
                visible: button.iconSource.length === 0 || iconImage.status !== Image.Ready
            }
        }

        ToolTip.visible: hovered
        ToolTip.text: action ? action.text : ""
    }

    component TextButton: Button {
        id: button

        height: 28
        width: 72
        anchors.verticalCenter: parent ? parent.verticalCenter : undefined
        padding: 0

        background: Rectangle {
            radius: 2
            color: button.down ? "#d9dde1" : button.hovered ? "#eef1f4" : "#ffffff"
            border.color: "#cdd2d6"
        }

        contentItem: Label {
            text: button.action ? button.action.text : ""
            color: "#4d5359"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 12
        }
    }

    component ToolbarDivider: Rectangle {
        width: 1
        height: 24
        anchors.verticalCenter: parent ? parent.verticalCenter : undefined
        color: "#d0d4d8"
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
            font.pixelSize: 12
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
