import QtQuick
import QtQuick.Controls

Item {
    id: root

    property real zoom: 0.84
    readonly property real designWidth: 1280
    readonly property real designHeight: 720
    readonly property real viewportHorizontalPadding: 48
    readonly property real viewportVerticalPadding: 48

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

                        canvasViewModel.addWidgetAt(type, position.x / root.zoom, position.y / root.zoom)
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: canvasViewModel.clearSelection()
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
                    model: canvasViewModel

                    delegate: WidgetNode {
                        widgetId: model.widgetId
                        widgetType: model.widgetType
                        widgetTitle: model.widgetTitle
                        buttonColor: model.widgetButtonColor
                        borderColorValue: model.widgetBorderColor
                        iconColor: model.widgetIconColor
                        selected: model.widgetSelected

                        x: model.widgetX
                        y: model.widgetY
                        width: model.widgetWidth
                        height: model.widgetHeight

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
    }
}
