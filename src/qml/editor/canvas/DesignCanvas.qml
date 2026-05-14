import QtQuick
import QtQuick.Controls

Item {
    id: root

    property real zoom: 0.84
    readonly property real designWidth: 1280
    readonly property real designHeight: 720
    readonly property real viewportHorizontalPadding: 48
    readonly property real viewportVerticalPadding: 48

    function cellAt(stageX, stageY) {
        var cells = canvasViewModel.layoutCells
        var normalizedX = stageX / root.designWidth
        var normalizedY = stageY / root.designHeight
        for (var i = cells.length - 1; i >= 0; --i) {
            var cell = cells[i]
            if (normalizedX >= cell.x && normalizedX <= cell.x + cell.width
                    && normalizedY >= cell.y && normalizedY <= cell.y + cell.height) {
                return cell
            }
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
                color: "#3b3c3d"
                border.color: "#ff7a00"
                border.width: 2

                Canvas {
                    anchors.fill: parent
                    anchors.margins: 1

                    onPaint: {
                        var ctx = getContext("2d")
                        ctx.clearRect(0, 0, width, height)
                        ctx.fillStyle = "#232526"
                        ctx.fillRect(0, 0, width, height)
                        ctx.strokeStyle = "#454748"
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

                Rectangle {
                    id: topFixedArea
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    height: fixedBarViewModel.topVisible ? fixedBarViewModel.topHeight : 0
                    visible: fixedBarViewModel.topVisible
                    color: "#1b1b1b"
                    border.color: "#242424"
                    z: 2
                }

                Rectangle {
                    id: bottomFixedArea
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    height: fixedBarViewModel.bottomVisible ? fixedBarViewModel.bottomHeight : 0
                    visible: fixedBarViewModel.bottomVisible
                    color: "#1b1b1b"
                    border.color: "#242424"
                    z: 2
                }

                Rectangle {
                    id: leftFixedArea
                    anchors.left: parent.left
                    anchors.top: topFixedArea.bottom
                    anchors.bottom: bottomFixedArea.top
                    width: fixedBarViewModel.leftVisible ? fixedBarViewModel.leftWidth : 0
                    visible: fixedBarViewModel.leftVisible
                    color: "#202020"
                    border.color: "#2b2b2b"
                    z: 2
                }

                Rectangle {
                    id: rightFixedArea
                    anchors.right: parent.right
                    anchors.top: topFixedArea.bottom
                    anchors.bottom: bottomFixedArea.top
                    width: fixedBarViewModel.rightVisible ? fixedBarViewModel.rightWidth : 0
                    visible: fixedBarViewModel.rightVisible
                    color: "#202020"
                    border.color: "#2b2b2b"
                    z: 2
                }

                /* Sample widgets removed to leave a blank design surface.
                   Keep the grid background and the DropArea/MouseArea/Repeater
                   so users can add components via the palette. */

                DropArea {
                    anchors.fill: parent
                    onDropped: {
                        var type = drag.mimeData.text || drag.text
                        if (!type || type.length === 0)
                            return

                        var targetCell = root.cellAt(position.x / root.zoom, position.y / root.zoom)
                        if (targetCell)
                            canvasViewModel.addWidgetToLayoutCell(type, targetCell.id)
                        else
                            canvasViewModel.addWidgetAt(type, position.x / root.zoom, position.y / root.zoom)
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: function(mouse) {
                        var targetCell = root.cellAt(mouse.x, mouse.y)
                        if (targetCell)
                            canvasViewModel.selectLayoutCell(targetCell.id,
                                                             (mouse.modifiers & (Qt.ControlModifier | Qt.ShiftModifier)) !== 0)
                        else
                            canvasViewModel.clearSelection()
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
                            text: "Add controls from the palette to design this screen"
                            color: "#b8b8b8"
                            font.pixelSize: 13
                        }
                    }
                }

                Repeater {
                    model: canvasViewModel.layoutCells.length

                    delegate: Rectangle {
                        readonly property var cell: canvasViewModel.layoutCells[index]

                        x: cell.x * root.designWidth
                        y: cell.y * root.designHeight
                        width: cell.width * root.designWidth
                        height: cell.height * root.designHeight
                        color: cell.selected ? "#243c4c55" : "transparent"
                        border.color: cell.selected ? "#1e9bff" : "#ff7a00"
                        border.width: cell.selected ? 2 : 1
                        z: 0.5

                        Label {
                            anchors.left: parent.left
                            anchors.top: parent.top
                            anchors.margins: 6
                            text: "Cell " + cell.id
                            color: "#d8dee3"
                            font.pixelSize: 11
                            visible: parent.width > 80 && parent.height > 36
                        }
                    }
                }

                Repeater {
                    model: canvasViewModel

                    delegate: WidgetNode {
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
                        z: model.widgetSelected ? 100 : 10

                        onSelectedRequested: function(id, additive) {
                            canvasViewModel.selectWidget(id, additive)
                        }

                        onGeometryCommitted: function(id, newX, newY, newWidth, newHeight) {
                            canvasViewModel.updateWidgetGeometry(id, newX, newY, newWidth, newHeight)
                        }
                    }
                }

                Repeater {
                    model: canvasViewModel.layoutResizeHandles.length

                    delegate: Item {
                        readonly property var handle: canvasViewModel.layoutResizeHandles[index]

                        x: handle.x * root.designWidth - width / 2
                        y: handle.y * root.designHeight - height / 2
                        width: 18
                        height: 18
                        z: 2000

                        Canvas {
                            anchors.fill: parent
                            rotation: handle.orientation === "vertical" ? 90 : 0
                            onPaint: {
                                var ctx = getContext("2d")
                                ctx.clearRect(0, 0, width, height)
                                ctx.fillStyle = "#ff7a00"
                                ctx.beginPath()
                                ctx.moveTo(width / 2, 2)
                                ctx.lineTo(width - 2, height - 2)
                                ctx.lineTo(2, height - 2)
                                ctx.closePath()
                                ctx.fill()
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            anchors.margins: -5
                            cursorShape: handle.orientation === "vertical" ? Qt.SizeHorCursor : Qt.SizeVerCursor

                            property real lastX: 0
                            property real lastY: 0

                            onPressed: function(mouse) {
                                lastX = mouse.x
                                lastY = mouse.y
                            }

                            onPositionChanged: function(mouse) {
                                var delta = handle.orientation === "vertical"
                                        ? (mouse.x - lastX) / root.designWidth
                                        : (mouse.y - lastY) / root.designHeight
                                lastX = mouse.x
                                lastY = mouse.y
                                canvasViewModel.resizeLayoutCells(String(handle.firstCellId),
                                                                  String(handle.secondCellId),
                                                                  handle.orientation,
                                                                  delta)
                            }
                        }
                    }
                }
            }
        }
    }
}
