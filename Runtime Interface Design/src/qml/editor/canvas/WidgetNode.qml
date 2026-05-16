import QtQuick
import QtQuick.Controls

Item {
    id: root

    property int widgetId: -1
    property string widgetType: "Widget"
    property string widgetTitle: "Widget"
    property string buttonColor: "#2f2f2f"
    property string borderColorValue: "#272727"
    property string iconColor: "#f6f6f6"
    property string componentSource: "widgets/PlaceholderWidget.qml"
    property bool selected: false
    property real boundsWidth: 1280
    property real boundsHeight: 720
    property real boundsX: 0
    property real boundsY: 0
    property bool showVerticalGuide: false
    property bool showHorizontalGuide: false
    property bool geometryEditing: false
    property bool previewMode: false
    readonly property bool overlayVisible: visible && selected && !previewMode

    signal selectedRequested(int id, bool additive)
    signal geometryCommitted(int id, real newX, real newY, real newWidth, real newHeight)

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: root.selected ? "#1e9bff" : "transparent"
        border.width: root.selected ? 2 : 0
        z: 1
    }

    Loader {
        id: widgetLoader
        anchors.fill: parent
        anchors.margins: root.selected ? 2 : 0
        source: root.componentSource.length > 0
                ? root.componentSource
                : "widgets/PlaceholderWidget.qml"

        onLoaded: {
            if (!item)
                return

            if ("bgColor" in item)
                item.bgColor = Qt.binding(function() { return root.buttonColor })
            if ("borderColor" in item)
                item.borderColor = Qt.binding(function() { return root.borderColorValue })
            else if ("borderColorValue" in item)
                item.borderColorValue = Qt.binding(function() { return root.borderColorValue })
            if ("fontColor" in item)
                item.fontColor = Qt.binding(function() { return root.iconColor })
            else if ("textColor" in item)
                item.textColor = Qt.binding(function() { return root.iconColor })

            if ("labelText" in item)
                item.labelText = Qt.binding(function() { return root.widgetTitle })
            if ("showText" in item)
                item.showText = Qt.binding(function() { return root.widgetTitle })
            else if ("text" in item)
                item.text = Qt.binding(function() { return root.widgetTitle })
        }
    }

    MouseArea {
        anchors.fill: parent
        drag.target: root
        drag.threshold: 8
        preventStealing: true
        enabled: !root.previewMode

        onPressed: function(mouse) {
            root.geometryEditing = true
            root.selectedRequested(root.widgetId, (mouse.modifiers & (Qt.ControlModifier | Qt.ShiftModifier)) !== 0)
        }

        onPositionChanged: {
            const snapThreshold = 6
            const centerX = root.x + root.width / 2
            const centerY = root.y + root.height / 2
            root.showVerticalGuide = Math.abs(centerX - (root.boundsX + root.boundsWidth / 2)) <= snapThreshold
            root.showHorizontalGuide = Math.abs(centerY - (root.boundsY + root.boundsHeight / 2)) <= snapThreshold
            if (root.showVerticalGuide)
                root.x = root.boundsX + root.boundsWidth / 2 - root.width / 2
            if (root.showHorizontalGuide)
                root.y = root.boundsY + root.boundsHeight / 2 - root.height / 2
            root.x = Math.max(root.boundsX, Math.min(root.x, root.boundsX + root.boundsWidth - root.width))
            root.y = Math.max(root.boundsY, Math.min(root.y, root.boundsY + root.boundsHeight - root.height))
        }

        onReleased: {
            root.x = Math.max(root.boundsX, Math.min(root.x, root.boundsX + root.boundsWidth - root.width))
            root.y = Math.max(root.boundsY, Math.min(root.y, root.boundsY + root.boundsHeight - root.height))
            root.showVerticalGuide = false
            root.showHorizontalGuide = false
            root.geometryCommitted(root.widgetId, root.x - root.boundsX, root.y - root.boundsY, root.width, root.height)
            root.geometryEditing = false
        }

        onCanceled: {
            root.showVerticalGuide = false
            root.showHorizontalGuide = false
            root.geometryEditing = false
        }
    }

    Rectangle {
        parent: root.parent
        x: root.boundsX + root.boundsWidth / 2 - width / 2
        y: root.boundsY
        width: 1
        height: root.boundsHeight
        color: "#39a7ff"
        visible: root.visible && root.showVerticalGuide && !root.previewMode
        z: 4000
    }

    Rectangle {
        parent: root.parent
        x: root.boundsX
        y: root.boundsY + root.boundsHeight / 2 - height / 2
        width: root.boundsWidth
        height: 1
        color: "#39a7ff"
        visible: root.visible && root.showHorizontalGuide && !root.previewMode
        z: 4000
    }

    component DistanceLabel: Rectangle {
        property string labelText: ""

        width: Math.max(30, textItem.implicitWidth + 8)
        height: 18
        radius: 2
        color: "#18212a"
        border.color: "#39a7ff"
        visible: root.overlayVisible
        z: 4200

        Text {
            id: textItem
            anchors.centerIn: parent
            text: labelText
            color: "#e7f3ff"
            font.pixelSize: 10
        }
    }

    Rectangle {
        parent: root.parent
        x: root.boundsX
        y: root.y + root.height / 2
        width: Math.max(0, root.x - root.boundsX)
        height: 1
        color: "#39a7ff"
        visible: root.overlayVisible && width > 0
        z: 4100
    }

    DistanceLabel {
        parent: root.parent
        x: Math.max(root.boundsX + 2, root.boundsX + (root.x - root.boundsX) / 2 - width / 2)
        y: Math.max(2, root.y + root.height / 2 - height - 4)
        labelText: Math.round(root.x - root.boundsX).toString()
    }

    Rectangle {
        parent: root.parent
        x: root.x + root.width
        y: root.y + root.height / 2
        width: Math.max(0, root.boundsX + root.boundsWidth - root.x - root.width)
        height: 1
        color: "#39a7ff"
        visible: root.overlayVisible && width > 0
        z: 4100
    }

    DistanceLabel {
        parent: root.parent
        x: Math.min(root.boundsX + root.boundsWidth - width - 2,
                    root.x + root.width + (root.boundsX + root.boundsWidth - root.x - root.width) / 2 - width / 2)
        y: Math.max(2, root.y + root.height / 2 - height - 4)
        labelText: Math.round(root.boundsX + root.boundsWidth - root.x - root.width).toString()
    }

    Rectangle {
        parent: root.parent
        x: root.x + root.width / 2
        y: root.boundsY
        width: 1
        height: Math.max(0, root.y - root.boundsY)
        color: "#39a7ff"
        visible: root.overlayVisible && height > 0
        z: 4100
    }

    DistanceLabel {
        parent: root.parent
        x: Math.max(2, root.x + root.width / 2 + 4)
        y: Math.max(root.boundsY + 2, root.boundsY + (root.y - root.boundsY) / 2 - height / 2)
        labelText: Math.round(root.y - root.boundsY).toString()
    }

    Rectangle {
        parent: root.parent
        x: root.x + root.width / 2
        y: root.y + root.height
        width: 1
        height: Math.max(0, root.boundsY + root.boundsHeight - root.y - root.height)
        color: "#39a7ff"
        visible: root.overlayVisible && height > 0
        z: 4100
    }

    DistanceLabel {
        parent: root.parent
        x: Math.max(2, root.x + root.width / 2 + 4)
        y: Math.min(root.boundsY + root.boundsHeight - height - 2,
                    root.y + root.height + (root.boundsY + root.boundsHeight - root.y - root.height) / 2 - height / 2)
        labelText: Math.round(root.boundsY + root.boundsHeight - root.y - root.height).toString()
    }

    component ResizeHandle: Rectangle {
        property int handlePos: 0

        width: 8
        height: 8
        color: "#ffffff"
        border.color: "#1e9bff"
        border.width: 1
        visible: root.overlayVisible
        z: 1000

        MouseArea {
            anchors.fill: parent
            anchors.margins: -5
            preventStealing: true
            cursorShape: {
                if (handlePos === 0 || handlePos === 4)
                    return Qt.SizeFDiagCursor
                if (handlePos === 2 || handlePos === 6)
                    return Qt.SizeBDiagCursor
                if (handlePos === 1 || handlePos === 5)
                    return Qt.SizeVerCursor
                return Qt.SizeHorCursor
            }

            property real startWidth
            property real startHeight
            property real startX
            property real startY
            property real startMouseX
            property real startMouseY

            onPressed: function(mouse) {
                root.geometryEditing = true
                startWidth = root.width
                startHeight = root.height
                startX = root.x
                startY = root.y
                const startPoint = mapToItem(root.parent, mouse.x, mouse.y)
                startMouseX = startPoint.x
                startMouseY = startPoint.y
                root.selectedRequested(root.widgetId, (mouse.modifiers & (Qt.ControlModifier | Qt.ShiftModifier)) !== 0)
            }

            onPositionChanged: function(mouse) {
                const currentPoint = mapToItem(root.parent, mouse.x, mouse.y)
                const dx = currentPoint.x - startMouseX
                const dy = currentPoint.y - startMouseY

                if (handlePos <= 2) {
                    const newHeight = Math.max(24, startHeight - dy)
                    root.y = startY + (startHeight - newHeight)
                    root.height = newHeight
                }
                if (handlePos >= 4 && handlePos <= 6) {
                    root.height = Math.max(24, startHeight + dy)
                }
                if (handlePos === 0 || handlePos === 7 || handlePos === 6) {
                    const newWidth = Math.max(24, startWidth - dx)
                    root.x = startX + (startWidth - newWidth)
                    root.width = newWidth
                }
                if (handlePos >= 2 && handlePos <= 4) {
                    root.width = Math.max(24, startWidth + dx)
                }
                root.width = Math.min(root.width, root.boundsWidth)
                root.height = Math.min(root.height, root.boundsHeight)
                root.x = Math.max(root.boundsX, Math.min(root.x, root.boundsX + root.boundsWidth - root.width))
                root.y = Math.max(root.boundsY, Math.min(root.y, root.boundsY + root.boundsHeight - root.height))
            }

            onReleased: {
                root.geometryCommitted(root.widgetId, root.x - root.boundsX, root.y - root.boundsY, root.width, root.height)
                root.geometryEditing = false
            }

            onCanceled: root.geometryEditing = false
        }
    }

    ResizeHandle {
        handlePos: 0
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: -width / 2
        anchors.topMargin: -height / 2
    }

    ResizeHandle {
        handlePos: 1
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: -height / 2
    }

    ResizeHandle {
        handlePos: 2
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: -width / 2
        anchors.topMargin: -height / 2
    }

    ResizeHandle {
        handlePos: 3
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: -width / 2
    }

    ResizeHandle {
        handlePos: 4
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: -width / 2
        anchors.bottomMargin: -height / 2
    }

    ResizeHandle {
        handlePos: 5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -height / 2
    }

    ResizeHandle {
        handlePos: 6
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: -width / 2
        anchors.bottomMargin: -height / 2
    }

    ResizeHandle {
        handlePos: 7
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: -width / 2
    }
}
