import QtQuick
import QtQuick.Controls

Item {
    id: root

    signal exitRequested()

    readonly property real designWidth: Math.max(1, canvasViewModel.canvasWidth)
    readonly property real designHeight: Math.max(1, canvasViewModel.canvasHeight)
    readonly property real workspaceStatusBarHeight: 24
    readonly property real editableHeight: Math.max(1, designHeight - workspaceStatusBarHeight)
    readonly property real topFixedHeight: canvasViewModel.topFixedVisible ? fixedBarViewModel.topHeight : 0
    readonly property real bottomFixedHeight: canvasViewModel.bottomFixedVisible ? fixedBarViewModel.bottomHeight : 0
    readonly property real leftFixedWidth: canvasViewModel.leftFixedVisible ? fixedBarViewModel.leftWidth : 0
    readonly property real rightFixedWidth: canvasViewModel.rightFixedVisible ? fixedBarViewModel.rightWidth : 0
    readonly property real runtimeWidth: designWidth + leftFixedWidth + rightFixedWidth
    readonly property real runtimeHeight: designHeight + topFixedHeight + bottomFixedHeight
    readonly property real sceneScale: Math.max(0.01, Math.min(width / runtimeWidth, height / runtimeHeight))

    focus: visible

    onVisibleChanged: {
        if (visible)
            forceActiveFocus()
    }

    Keys.onEscapePressed: root.exitRequested()

    Rectangle {
        anchors.fill: parent
        color: "#101214"
    }

    Item {
        id: runtimeRoot

        x: Math.round((root.width - width * scale) / 2)
        y: Math.round((root.height - height * scale) / 2)
        width: root.runtimeWidth
        height: root.runtimeHeight
        scale: root.sceneScale
        transformOrigin: Item.TopLeft

        Rectangle {
            anchors.fill: parent
            color: "#15191c"
        }

        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.color: "#2a9d8f"
            border.width: 2
            z: 9000
        }

        Item {
            id: topFixed
            x: root.leftFixedWidth
            y: 0
            width: root.designWidth
            height: root.topFixedHeight
            visible: height > 0
            clip: true

            Rectangle { anchors.fill: parent; color: "#1d2427" }
            RuntimeWidgetLayer { anchors.fill: parent; model: []; cellLookupItem: null }
        }

        Item {
            id: bottomFixed
            x: root.leftFixedWidth
            y: root.topFixedHeight + root.designHeight
            width: root.designWidth
            height: root.bottomFixedHeight
            visible: height > 0
            clip: true

            Rectangle { anchors.fill: parent; color: "#1d2427" }
            RuntimeWidgetLayer { anchors.fill: parent; model: []; cellLookupItem: null }
        }

        Item {
            id: leftFixed
            x: 0
            y: root.topFixedHeight
            width: root.leftFixedWidth
            height: root.designHeight
            visible: width > 0
            clip: true

            Rectangle { anchors.fill: parent; color: "#1b2225" }
            RuntimeWidgetLayer { anchors.fill: parent; model: []; cellLookupItem: null }
        }

        Item {
            id: rightFixed
            x: root.leftFixedWidth + root.designWidth
            y: root.topFixedHeight
            width: root.rightFixedWidth
            height: root.designHeight
            visible: width > 0
            clip: true

            Rectangle { anchors.fill: parent; color: "#1b2225" }
            RuntimeWidgetLayer { anchors.fill: parent; model: []; cellLookupItem: null }
        }

        Item {
            id: runtimeStage

            property var layoutCells: canvasViewModel.layoutCells

            x: root.leftFixedWidth
            y: root.topFixedHeight
            width: root.designWidth
            height: root.editableHeight
            clip: true

            function cellById(cellId) {
                for (var i = 0; i < layoutCells.length; ++i) {
                    if (layoutCells[i].id === cellId)
                        return layoutCells[i]
                }
                return null
            }

            Rectangle {
                anchors.fill: parent
                color: canvasViewModel.currentBackgroundColor.length > 0
                       ? canvasViewModel.currentBackgroundColor
                       : "#202225"
            }

            RuntimeWidgetLayer {
                anchors.fill: parent
                model: canvasViewModel
                cellLookupItem: runtimeStage
            }
        }

        WorkspaceTopBar {
            objectName: "RuntimePreviewTopBar"
            x: root.leftFixedWidth + root.designWidth - width
            y: root.topFixedHeight
            width: 132
            height: 42
            z: 8000
        }

        WorkspaceStatusBar {
            objectName: "RuntimePreviewStatusBar"
            x: root.leftFixedWidth
            y: root.topFixedHeight + root.editableHeight
            width: root.designWidth
            height: root.workspaceStatusBarHeight
            regionText: "Ready"
            z: 8000
        }

        Rectangle {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 10
            anchors.topMargin: 10
            width: previewText.implicitWidth + 16
            height: 24
            radius: 3
            color: "#2a9d8f"
            z: 9200

            Text {
                id: previewText
                anchors.centerIn: parent
                text: "PREVIEW"
                color: "#ffffff"
                font.pixelSize: 11
                font.bold: true
            }
        }
    }

    component RuntimeWidgetLayer: Item {
        id: layer

        property var model
        property var cellLookupItem

        Repeater {
            model: layer.model

            delegate: Item {
                id: widgetPreview

                readonly property var widgetCell: layer.cellLookupItem ? layer.cellLookupItem.cellById(model.widgetParentRegionId) : null
                readonly property real regionX: widgetCell ? widgetCell.x * layer.width : 0
                readonly property real regionY: widgetCell ? widgetCell.y * layer.height : 0
                readonly property string componentSource: model.widgetComponentSource
                readonly property string buttonColor: model.widgetButtonColor
                readonly property string borderColorValue: model.widgetBorderColor
                readonly property string iconColor: model.widgetIconColor
                readonly property string title: model.widgetTitle

                visible: layer.cellLookupItem === null || widgetCell !== null
                x: regionX + model.widgetX
                y: regionY + model.widgetY
                width: model.widgetWidth
                height: model.widgetHeight
                z: 1000 + index

                Loader {
                    anchors.fill: parent
                    source: widgetPreview.componentSource.length > 0
                            ? widgetPreview.componentSource
                            : "widgets/PlaceholderWidget.qml"

                    onLoaded: {
                        if (!item)
                            return

                        if ("enabled" in item)
                            item.enabled = false
                        if ("bgColor" in item)
                            item.bgColor = Qt.binding(function() { return widgetPreview.buttonColor })
                        if ("borderColor" in item)
                            item.borderColor = Qt.binding(function() { return widgetPreview.borderColorValue })
                        else if ("borderColorValue" in item)
                            item.borderColorValue = Qt.binding(function() { return widgetPreview.borderColorValue })
                        if ("fontColor" in item)
                            item.fontColor = Qt.binding(function() { return widgetPreview.iconColor })
                        else if ("textColor" in item)
                            item.textColor = Qt.binding(function() { return widgetPreview.iconColor })

                        if ("labelText" in item)
                            item.labelText = Qt.binding(function() { return widgetPreview.title })
                        if ("showText" in item)
                            item.showText = Qt.binding(function() { return widgetPreview.title })
                        else if ("text" in item)
                            item.text = Qt.binding(function() { return widgetPreview.title })
                    }
                }
            }
        }
    }

    Button {
        id: exitButton

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 14
        text: "Exit Preview"
        focusPolicy: Qt.NoFocus
        opacity: hovered ? 1.0 : 0.72
        onClicked: root.exitRequested()

        background: Rectangle {
            radius: 4
            color: "#1b2024"
            border.color: "#3b444c"
        }

        contentItem: Text {
            text: exitButton.text
            color: "#f4f6f8"
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
