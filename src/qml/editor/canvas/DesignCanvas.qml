import QtQuick
import QtQuick.Controls

Item {
    id: root

    property real zoom: 0.84
    readonly property real designWidth: 1280
    readonly property real designHeight: 720
    readonly property real viewportHorizontalPadding: 48
    readonly property real viewportVerticalPadding: 48
    readonly property real workspaceTopBarHeight: 42
    readonly property real workspaceStatusBarHeight: 24
    readonly property real editableHeight: designHeight - workspaceTopBarHeight - workspaceStatusBarHeight

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
                border.color: "#ff7a00"
                border.width: 2

                WorkspaceTopBar {
                    id: workspaceTopBar
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    height: root.workspaceTopBarHeight
                }

                Item {
                    id: editableArea
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: workspaceTopBar.bottom
                    anchors.bottom: workspaceStatusBar.top
                    clip: true

                    Canvas {
                        anchors.fill: parent
                        onPaint: {
                            var ctx = getContext("2d")
                            ctx.clearRect(0, 0, width, height)
                            ctx.fillStyle = "#232526"
                            ctx.fillRect(0, 0, width, height)
                            ctx.strokeStyle = "#363b40"
                            ctx.lineWidth = 1
                            for (var x = 0; x < width; x += 20) {
                                ctx.beginPath()
                                ctx.moveTo(x, 0)
                                ctx.lineTo(x, height)
                                ctx.stroke()
                            }
                            for (var y = 0; y < height; y += 20) {
                                ctx.beginPath()
                                ctx.moveTo(0, y)
                                ctx.lineTo(width, y)
                                ctx.stroke()
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: function(mouse) {
                            var targetCell = root.cellAt(mouse.x, mouse.y)
                            if (targetCell)
                                canvasViewModel.selectLayoutCell(targetCell.id,
                                                                 (mouse.modifiers & (Qt.ControlModifier | Qt.ShiftModifier)) !== 0)
                            else {
                                canvasViewModel.clearSelection()
                                canvasViewModel.clearLayoutCellSelection()
                            }
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
                        model: canvasViewModel.layoutCells.length

                        delegate: Item {
                            id: regionContainer

                            required property int index
                            readonly property var cell: canvasViewModel.layoutCells[index]
                            property bool hovered: false

                            x: cell.x * editableArea.width
                            y: cell.y * editableArea.height
                            width: cell.width * editableArea.width
                            height: cell.height * editableArea.height
                            clip: true
                            z: cell.selected ? 20 : 10

                            Rectangle {
                                anchors.fill: parent
                                color: "transparent"
                                border.color: cell.selected || dropArea.containsDrag ? "#ff9a2c"
                                                   : (regionContainer.hovered ? "#7b858e" : "#4d555c")
                                border.width: cell.selected || dropArea.containsDrag ? 2
                                             : (regionContainer.hovered ? 1 : 1)
                            }

                            HoverHandler {
                                onHoveredChanged: regionContainer.hovered = hovered
                            }

                            Label {
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.margins: 6
                                text: "Region " + cell.id
                                color: "#c8d0d8"
                                font.pixelSize: 11
                                visible: parent.width > 92 && parent.height > 38
                            }

                            DropArea {
                                id: dropArea
                                anchors.fill: parent
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

                            Repeater {
                                model: canvasViewModel

                                delegate: WidgetNode {
                                    visible: model.widgetParentRegionId === regionContainer.cell.id
                                    widgetId: model.widgetId
                                    widgetType: model.widgetType
                                    widgetTitle: model.widgetTitle
                                    buttonColor: model.widgetButtonColor
                                    borderColorValue: model.widgetBorderColor
                                    iconColor: model.widgetIconColor
                                    componentSource: model.widgetComponentSource
                                    selected: model.widgetSelected

                                    x: model.widgetX
                                    y: model.widgetY
                                    width: model.widgetWidth
                                    height: model.widgetHeight
                                    z: model.widgetSelected ? 100 : 30

                                    boundsWidth: regionContainer.width
                                    boundsHeight: regionContainer.height

                                    onSelectedRequested: function(id, additive) {
                                        canvasViewModel.selectWidget(id, additive)
                                    }

                                    onGeometryCommitted: function(id, newX, newY, newWidth, newHeight) {
                                        canvasViewModel.updateWidgetGeometry(id, newX, newY, newWidth, newHeight)
                                    }
                                }
                            }
                        }
                    }

                    Repeater {
                        model: canvasViewModel.layoutResizeHandles.length

                        delegate: SplitterHandle {
                            required property int index
                            readonly property var handle: canvasViewModel.layoutResizeHandles[index]

                            orientation: handle.orientation
                            coordinateItem: editableArea
                            x: handle.orientation === "vertical"
                               ? handle.x * editableArea.width - width / 2
                               : handle.start * editableArea.width
                            y: handle.orientation === "vertical"
                               ? handle.start * editableArea.height
                               : handle.y * editableArea.height - height / 2
                            width: handle.orientation === "vertical"
                                   ? 12
                                   : Math.max(36, handle.length * editableArea.width)
                            height: handle.orientation === "vertical"
                                    ? Math.max(36, handle.length * editableArea.height)
                                    : 12
                            z: 5000

                            onDragged: function(deltaRatio) {
                                canvasViewModel.resizeLayoutCells(String(handle.firstCellId),
                                                                  String(handle.secondCellId),
                                                                  handle.orientation,
                                                                  deltaRatio)
                            }

                            onMergeRequested: {
                                canvasViewModel.mergeLayoutSiblings(handle.firstCellId,
                                                                    handle.secondCellId)
                            }
                        }
                    }
                }

                WorkspaceStatusBar {
                    id: workspaceStatusBar
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
