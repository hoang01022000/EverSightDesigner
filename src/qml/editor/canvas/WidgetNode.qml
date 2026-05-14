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

            item.bgColor = Qt.binding(function() { return root.buttonColor })
            item.borderColor = Qt.binding(function() { return root.borderColorValue })
            item.fontColor = Qt.binding(function() { return root.iconColor })

            if (item.hasOwnProperty("labelText"))
                item.labelText = Qt.binding(function() { return root.widgetTitle })
            if (item.hasOwnProperty("showText"))
                item.showText = Qt.binding(function() { return root.widgetTitle })
        }
    }

    MouseArea {
        anchors.fill: parent
        drag.target: root
        drag.threshold: 8

        onPressed: function(mouse) {
            root.z = 999
            root.selectedRequested(root.widgetId, (mouse.modifiers & (Qt.ControlModifier | Qt.ShiftModifier)) !== 0)
        }

        onReleased: {
            root.z = 1
            root.geometryCommitted(root.widgetId, root.x, root.y, root.width, root.height)
        }
    }

    component ResizeHandle: Rectangle {
        property int handlePos: 0

        width: 8
        height: 8
        color: "#ffffff"
        border.color: "#1e9bff"
        border.width: 1
        visible: root.selected
        z: 1000

        MouseArea {
            anchors.fill: parent
            anchors.margins: -5
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
                startWidth = root.width
                startHeight = root.height
                startX = root.x
                startY = root.y
                startMouseX = mouse.x
                startMouseY = mouse.y
                root.selectedRequested(root.widgetId, (mouse.modifiers & (Qt.ControlModifier | Qt.ShiftModifier)) !== 0)
            }

            onPositionChanged: function(mouse) {
                const dx = mouse.x - startMouseX
                const dy = mouse.y - startMouseY

                if (handlePos <= 2) {
                    const newHeight = Math.max(20, startHeight - dy)
                    root.y = startY + (startHeight - newHeight)
                    root.height = newHeight
                }
                if (handlePos >= 4 && handlePos <= 6) {
                    root.height = Math.max(20, startHeight + dy)
                }
                if (handlePos === 0 || handlePos === 7 || handlePos === 6) {
                    const newWidth = Math.max(20, startWidth - dx)
                    root.x = startX + (startWidth - newWidth)
                    root.width = newWidth
                }
                if (handlePos >= 2 && handlePos <= 4) {
                    root.width = Math.max(20, startWidth + dx)
                }
            }

            onReleased: root.geometryCommitted(root.widgetId, root.x, root.y, root.width, root.height)
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
