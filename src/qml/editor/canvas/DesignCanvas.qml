import QtQuick
import QtQuick.Controls

Item {
    id: root

    property real zoom: 0.84
    property real designWidth: canvasViewModel.canvasWidth
    property real designHeight: canvasViewModel.canvasHeight
    readonly property real viewportHorizontalPadding: 48
    readonly property real viewportVerticalPadding: 48
    readonly property real workspaceTopBarHeight: 42
    readonly property real workspaceStatusBarHeight: 24
    readonly property real editableHeight: designHeight - workspaceStatusBarHeight
    readonly property bool previewMode: canvasViewModel.previewMode
    property string activeRatioText: ""
    property real activeRatioX: 0
    property real activeRatioY: 0

    Behavior on zoom {
        NumberAnimation {
            duration: 120
            easing.type: Easing.OutCubic
        }
    }

    function cellAt(localX, localY) {
        var cells = canvasViewModel.layoutCells
        var normalizedX = localX / root.designWidth
        var normalizedY = localY / root.editableHeight
        for (var i = cells.length - 1; i >= 0; --i) {
            var cell = cells[i]
            if (normalizedX >= cell.x && normalizedX <= cell.x + cell.width
                    && normalizedY >= cell.y && normalizedY <= cell.y + cell.height)
                return cell
        }
        return null
    }

    Rectangle {
        anchors.fill: parent
        color: "#303437"

        Canvas {
            anchors.fill: parent
            onPaint: {
                var ctx = getContext("2d")
                ctx.fillStyle = "#303437"
                ctx.fillRect(0, 0, width, height)
                ctx.strokeStyle = "#3a3e42"
                ctx.lineWidth = 1
                for (var x = 0; x < width; x += 10) {
                    ctx.beginPath()
                    ctx.moveTo(x, 0)
                    ctx.lineTo(x, height)
                    ctx.stroke()
                }
                for (var y = 0; y < height; y += 10) {
                    ctx.beginPath()
                    ctx.moveTo(0, y)
                    ctx.lineTo(width, y)
                    ctx.stroke()
                }
            }
        }
    }

    Flickable {
        id: canvasFlickable
        anchors.fill: parent
        contentWidth: Math.max(width, stageFrame.width * root.zoom + root.viewportHorizontalPadding)
        contentHeight: Math.max(height, stageFrame.height * root.zoom + root.viewportVerticalPadding)
        clip: true

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            enabled: !root.previewMode
            onClicked: {
                canvasViewModel.clearSelection()
                canvasViewModel.clearLayoutCellSelection()
            }
        }

        Item {
            id: zoomLayer
            x: Math.max(root.viewportHorizontalPadding / 2,
                        (canvasFlickable.width - stageFrame.width * root.zoom) / 2)
            y: Math.max(root.viewportVerticalPadding / 2,
                        (canvasFlickable.height - stageFrame.height * root.zoom) / 2)
            width: stageFrame.width * root.zoom
            height: stageFrame.height * root.zoom
            scale: root.zoom
            transformOrigin: Item.TopLeft

            Rectangle {
                id: stageFrame
                width: root.designWidth
                height: root.designHeight
                color: "#202225"
                border.color: "#68727b"
                border.width: 1

                WorkspaceTopBar {
                    id: workspaceTopBar
                    objectName: "CanvasWorkspaceTopBar"
                    anchors.right: parent.right
                    anchors.top: parent.top
                    width: 132
                    height: root.workspaceTopBarHeight
                    z: 9000
                }

                Item {
                    id: designStage
                    objectName: "DesignCanvasStage"
                    property var layoutCells: canvasViewModel.layoutCells
                    property var layoutResizeHandles: canvasViewModel.layoutResizeHandles
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: workspaceStatusBar.top
                    clip: true

                    function cellById(cellId) {
                        for (var i = 0; i < layoutCells.length; ++i) {
                            if (layoutCells[i].id === cellId)
                                return layoutCells[i]
                        }
                        return null
                    }

                    Canvas {
                        anchors.fill: parent
                        onPaint: {
                            var ctx = getContext("2d")
                            ctx.clearRect(0, 0, width, height)
                            ctx.fillStyle = "#232526"
                            ctx.fillRect(0, 0, width, height)
                        }
                    }

                    Menu {
                        id: cellContextMenu

                        MenuItem {
                            text: "Merge cells"
                            enabled: canvasViewModel.hasSelectedLayoutCell && !root.previewMode
                            onTriggered: canvasViewModel.mergeSelectedLayoutCells()
                        }
                    }

                    TapHandler {
                        acceptedButtons: Qt.RightButton
                        enabled: !root.previewMode
                        onTapped: function(eventPoint) {
                            cellContextMenu.popup(designStage, eventPoint.position.x, eventPoint.position.y)
                        }
                    }

                    Rectangle {
                        anchors.centerIn: parent
                        width: 360
                        height: 118
                        radius: 3
                        color: "#424344"
                        border.color: "#585a5c"
                        visible: canvasViewModel.widgetCount === 0
                        z: 2

                        Column {
                            anchors.centerIn: parent
                            spacing: 8

                            Label {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: "Blank runtime screen"
                                color: "#e0e0e0"
                                font.pixelSize: 18
                                font.bold: true
                            }

                            Label {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: "Drag controls from the palette into a split region"
                                color: "#b8b8b8"
                                font.pixelSize: 13
                            }
                        }
                    }

                    Repeater {
                        model: designStage.layoutCells.length

                        delegate: Item {
                            id: regionContainer

                            required property int index
                            readonly property var cell: designStage.layoutCells[index]
                            property bool hovered: false

                            x: cell.x * designStage.width
                            y: cell.y * designStage.height
                            width: cell.width * designStage.width
                            height: cell.height * designStage.height
                            clip: true
                            z: cell.selected ? 20 : 10

                            Rectangle {
                                anchors.fill: parent
                                color: "transparent"
                                border.color: root.previewMode ? "transparent"
                                                   : cell.selected || dropArea.containsDrag ? "#ff9a2c"
                                                   : (regionContainer.hovered ? "#9aa4ad" : "#68727b")
                                border.width: root.previewMode ? 0
                                             : cell.selected || dropArea.containsDrag ? 2
                                             : (regionContainer.hovered ? 2 : 1)
                                z: 1
                            }

                            HoverHandler {
                                enabled: !root.previewMode
                                onHoveredChanged: regionContainer.hovered = hovered
                            }

                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.LeftButton
                                enabled: !root.previewMode
                                z: 3

                                onClicked: function(mouse) {
                                    canvasViewModel.selectLayoutCell(regionContainer.cell.id,
                                                                     (mouse.modifiers & (Qt.ControlModifier | Qt.ShiftModifier)) !== 0)
                                }
                            }

                            Label {
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.margins: 6
                                z: 6
                                text: "Region " + cell.id
                                color: "#c8d0d8"
                                font.pixelSize: 11
                                visible: false
                            }

                            DropArea {
                                id: dropArea
                                anchors.fill: parent
                                z: 4
                                enabled: !root.previewMode
                                keys: ["text/plain", "application/eversight-widget-type"]

                                onDropped: function(drop) {
                                    var type = ""
                                    if (drop.hasText)
                                        type = drop.text
                                    if (!type || type.length === 0)
                                        type = drop.getDataAsString("text/plain")
                                    if (!type || type.length === 0)
                                        return
                                    canvasViewModel.addWidgetToLayoutCellAt(type,
                                                                            cell.id,
                                                                            Math.max(0, drop.x - 80),
                                                                            Math.max(0, drop.y - 46))
                                }
                            }

                        }
                    }

                    Repeater {
                        model: canvasViewModel

                        delegate: WidgetNode {
                            id: widgetNode

                            readonly property var widgetCell: designStage.cellById(model.widgetParentRegionId)
                            readonly property real regionX: widgetCell ? widgetCell.x * designStage.width : 0
                            readonly property real regionY: widgetCell ? widgetCell.y * designStage.height : 0
                            readonly property real regionWidth: widgetCell ? widgetCell.width * designStage.width : designStage.width
                            readonly property real regionHeight: widgetCell ? widgetCell.height * designStage.height : designStage.height

                            visible: widgetCell !== null
                            widgetId: model.widgetId
                            widgetType: model.widgetType
                            widgetTitle: model.widgetTitle
                            buttonColor: model.widgetButtonColor
                            borderColorValue: model.widgetBorderColor
                            iconColor: model.widgetIconColor
                            componentSource: model.widgetComponentSource
                            selected: model.widgetSelected
                            previewMode: root.previewMode

                            z: 1000 + index

                            boundsX: regionX
                            boundsY: regionY
                            boundsWidth: regionWidth
                            boundsHeight: regionHeight

                            Binding {
                                target: widgetNode
                                property: "x"
                                value: widgetNode.regionX + model.widgetX
                                when: !widgetNode.geometryEditing
                                restoreMode: Binding.RestoreNone
                            }

                            Binding {
                                target: widgetNode
                                property: "y"
                                value: widgetNode.regionY + model.widgetY
                                when: !widgetNode.geometryEditing
                                restoreMode: Binding.RestoreNone
                            }

                            Binding {
                                target: widgetNode
                                property: "width"
                                value: model.widgetWidth
                                when: !widgetNode.geometryEditing
                                restoreMode: Binding.RestoreNone
                            }

                            Binding {
                                target: widgetNode
                                property: "height"
                                value: model.widgetHeight
                                when: !widgetNode.geometryEditing
                                restoreMode: Binding.RestoreNone
                            }

                            onSelectedRequested: function(id, additive) {
                                canvasViewModel.selectWidget(id, additive)
                            }

                            onGeometryCommitted: function(id, newX, newY, newWidth, newHeight) {
                                canvasViewModel.updateWidgetGeometry(id, newX, newY, newWidth, newHeight)
                            }
                        }
                    }

                    Repeater {
                        model: designStage.layoutResizeHandles.length

                        delegate: SplitterHandle {
                            required property int index
                            readonly property var handle: designStage.layoutResizeHandles[index]
                            property real dragRatio: handle.ratio

                            orientation: handle.orientation
                            coordinateItem: designStage
                            parentCellId: handle.parentCellId
                            x: handle.orientation === "vertical"
                               ? handle.x * designStage.width - width / 2
                               : handle.start * designStage.width
                            y: handle.orientation === "vertical"
                               ? handle.start * designStage.height
                               : handle.y * designStage.height - height / 2
                            width: handle.orientation === "vertical"
                                   ? 12
                                   : Math.max(36, handle.length * designStage.width)
                            height: handle.orientation === "vertical"
                                    ? Math.max(36, handle.length * designStage.height)
                                    : 12
                            z: 5000
                            visible: !root.previewMode
                            enabled: !root.previewMode

                            onDragged: function(deltaRatio) {
                                var normalizedDelta = deltaRatio
                                var minFirst = handle.orientation === "vertical"
                                        ? 50 / (handle.parentWidth * designStage.width)
                                        : 50 / (handle.parentHeight * designStage.height)
                                var minSecond = minFirst
                                var nextRatio = Math.max(minFirst, Math.min(1 - minSecond, dragRatio + normalizedDelta))
                                dragRatio = nextRatio
                                canvasViewModel.resizeDivider(handle.parentCellId,
                                                              nextRatio,
                                                              minFirst,
                                                              minSecond)
                                root.activeRatioText = nextRatio.toFixed(2) + "*"
                                root.activeRatioX = handle.orientation === "vertical" ? handle.x * designStage.width : handle.start * designStage.width
                                root.activeRatioY = handle.orientation === "vertical" ? 0 : handle.y * designStage.height
                            }

                            onDragStarted: {
                                dragRatio = handle.ratio
                                canvasViewModel.beginResizeDivider(handle.parentCellId)
                            }

                            onDragFinished: canvasViewModel.endResizeDivider()

                            onMergeRequested: {
                                canvasViewModel.mergeLayoutSiblings(handle.firstCellId,
                                                                    handle.secondCellId)
                            }
                        }
                    }

                    Rectangle {
                        x: root.activeRatioX + 4
                        y: root.activeRatioY + 4
                        width: 46
                        height: 20
                        radius: 2
                        color: "#252a2f"
                        border.color: "#ff9a2c"
                        visible: root.activeRatioText.length > 0
                                 && !root.previewMode
                        z: 7000

                        Text {
                            anchors.centerIn: parent
                            text: root.activeRatioText
                            color: "#f4f6f8"
                            font.pixelSize: 11
                        }
                    }

                    Repeater {
                        model: designStage.layoutResizeHandles.length

                        delegate: Rectangle {
                            required property int index
                            readonly property var handle: designStage.layoutResizeHandles[index]

                            x: handle.orientation === "vertical" ? handle.x * designStage.width - width / 2
                                                                  : handle.x * designStage.width - width / 2
                            y: handle.orientation === "vertical" ? 2
                                                                  : handle.y * designStage.height - height / 2
                            width: 44
                            height: 18
                            radius: 2
                            color: "#202428"
                            border.color: "#5f6870"
                            visible: !root.previewMode
                            z: 6000

                            Text {
                                anchors.centerIn: parent
                                text: Number(handle.ratio).toFixed(2) + "*"
                                color: "#cbd3da"
                                font.pixelSize: 10
                            }

                            TapHandler {
                                acceptedButtons: Qt.LeftButton
                                onDoubleTapped: ratioEditor.openForHandle(handle.parentCellId,
                                                                          Number(handle.ratio).toFixed(2),
                                                                          parent.x,
                                                                          parent.y)
                            }
                        }
                    }

                    Rectangle {
                        id: ratioEditor
                        width: 54
                        height: 22
                        radius: 2
                        color: "#ffffff"
                        border.color: "#ff9a2c"
                        visible: false
                        z: 8000
                        enabled: !root.previewMode

                        property int parentCellId: -1

                        function openForHandle(parentId, value, editorX, editorY) {
                            parentCellId = parentId
                            x = editorX
                            y = editorY
                            ratioInput.text = value
                            visible = true
                            ratioInput.forceActiveFocus()
                            ratioInput.selectAll()
                        }

                        TextInput {
                            id: ratioInput
                            anchors.fill: parent
                            anchors.margins: 3
                            color: "#111820"
                            font.pixelSize: 11
                            horizontalAlignment: TextInput.AlignHCenter
                            verticalAlignment: TextInput.AlignVCenter

                            onAccepted: {
                                canvasViewModel.setExactRatio(ratioEditor.parentCellId, Number(text))
                                ratioEditor.visible = false
                            }

                            onEditingFinished: {
                                if (ratioEditor.visible) {
                                    canvasViewModel.setExactRatio(ratioEditor.parentCellId, Number(text))
                                    ratioEditor.visible = false
                                }
                            }
                        }
                    }

                }

                WorkspaceStatusBar {
                    id: workspaceStatusBar
                    objectName: "CanvasWorkspaceStatusBar"
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    height: root.workspaceStatusBarHeight
                    regionText: canvasViewModel.hasSelectedLayoutCell
                                ? "Active region: " + canvasViewModel.selectedLayoutCellId
                                : "Ready"
                }
            }
        }
    }
}
