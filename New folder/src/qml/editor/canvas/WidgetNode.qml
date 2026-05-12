import QtQuick

Rectangle {
    id: root

    property int widgetId: -1
    property string widgetType: "Widget"
    property string widgetTitle: "Widget"
    property string buttonColor: "#2f2f2f"
    property string borderColorValue: "#272727"
    property string iconColor: "#f6f6f6"
    property bool selected: false

    signal selectedRequested(int id)
    signal geometryCommitted(int id, real newX, real newY, real newWidth, real newHeight)

    width: 104
    height: 50
    radius: 0
    color: root.buttonColor
    border.color: root.selected ? "#1e9bff" : root.borderColorValue
    border.width: root.selected ? 2 : 1

    function displayText() {
        if (root.widgetType === "RunControl")
            return "RUN"
        if (root.widgetType === "Indicator")
            return "OK"
        if (root.widgetType === "TrafficLight")
            return "●  ●"
        if (root.widgetType === "TextInput")
            return root.widgetTitle
        if (root.widgetType === "TrendChart")
            return "LINE"
        if (root.widgetType === "DataTable")
            return "TABLE"
        if (root.widgetType === "CameraView")
            return "IMG"
        if (root.widgetType === "Button")
            return "BTN"
        if (root.widgetType === "Toggle")
            return "SW"
        if (root.widgetType === "Label")
            return root.widgetTitle
        if (root.widgetType === "GroupBox")
            return root.widgetTitle
        return root.widgetTitle
    }

    Text {
        anchors.centerIn: parent
        text: root.displayText()
        color: root.iconColor
        font.pixelSize: Math.max(11, Math.min(26, root.height * 0.42))
        font.bold: true
        elide: Text.ElideRight
        width: parent.width - 12
        horizontalAlignment: Text.AlignHCenter
    }

    Repeater {
        model: root.selected ? 8 : 0

        delegate: Rectangle {
            width: 7
            height: 7
            radius: 1
            color: "#6bb8ff"
            border.color: "#ffffff"

            x: index % 3 === 0 ? -4 : (index % 3 === 1 ? root.width / 2 - 3 : root.width - 3)
            y: index < 3 ? -4 : (index < 5 ? root.height / 2 - 3 : root.height - 3)
        }
    }

    MouseArea {
        anchors.fill: parent
        drag.target: root

        onPressed: {
            root.z = 999
            root.selectedRequested(root.widgetId)
        }

        onClicked: root.selectedRequested(root.widgetId)

        onReleased: {
            root.geometryCommitted(root.widgetId, root.x, root.y, root.width, root.height)
        }
    }

    Rectangle {
        id: resizeHandleTopLeft
        width: 14
        height: 14
        visible: root.selected
        color: "#ff8a00"
        anchors.left: parent.left
        anchors.top: parent.top
        z: 10

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeFDiagCursor

            property real startWidth: 0
            property real startHeight: 0
            property real startX: 0
            property real startY: 0
            property real startMouseX: 0
            property real startMouseY: 0

            onPressed: function(mouse) {
                root.selectedRequested(root.widgetId)
                startWidth = root.width
                startHeight = root.height
                startX = root.x
                startY = root.y
                startMouseX = mouse.x
                startMouseY = mouse.y
            }

            onPositionChanged: function(mouse) {
                if (!pressed)
                    return

                var dx = mouse.x - startMouseX
                var dy = mouse.y - startMouseY
                var newWidth = Math.max(32, startWidth - dx)
                var newHeight = Math.max(24, startHeight - dy)
                root.width = newWidth
                root.height = newHeight
                root.x = startX + (startWidth - newWidth)
                root.y = startY + (startHeight - newHeight)
            }

            onReleased: {
                root.geometryCommitted(root.widgetId, root.x, root.y, root.width, root.height)
            }
        }
    }

    Rectangle {
        id: resizeHandleTop
        width: 14
        height: 14
        visible: root.selected
        color: "#ff8a00"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        z: 10

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeVerCursor

            property real startHeight: 0
            property real startY: 0
            property real startMouseY: 0

            onPressed: function(mouse) {
                root.selectedRequested(root.widgetId)
                startHeight = root.height
                startY = root.y
                startMouseY = mouse.y
            }

            onPositionChanged: function(mouse) {
                if (!pressed)
                    return

                var dy = mouse.y - startMouseY
                var newHeight = Math.max(24, startHeight - dy)
                root.height = newHeight
                root.y = startY + (startHeight - newHeight)
            }

            onReleased: {
                root.geometryCommitted(root.widgetId, root.x, root.y, root.width, root.height)
            }
        }
    }

    Rectangle {
        id: resizeHandleTopRight
        width: 14
        height: 14
        visible: root.selected
        color: "#ff8a00"
        anchors.right: parent.right
        anchors.top: parent.top
        z: 10

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeBDiagCursor

            property real startWidth: 0
            property real startHeight: 0
            property real startY: 0
            property real startMouseX: 0
            property real startMouseY: 0

            onPressed: function(mouse) {
                root.selectedRequested(root.widgetId)
                startWidth = root.width
                startHeight = root.height
                startY = root.y
                startMouseX = mouse.x
                startMouseY = mouse.y
            }

            onPositionChanged: function(mouse) {
                if (!pressed)
                    return

                var dx = mouse.x - startMouseX
                var dy = mouse.y - startMouseY
                root.width = Math.max(32, startWidth + dx)
                var newHeight = Math.max(24, startHeight - dy)
                root.height = newHeight
                root.y = startY + (startHeight - newHeight)
            }

            onReleased: {
                root.geometryCommitted(root.widgetId, root.x, root.y, root.width, root.height)
            }
        }
    }

    Rectangle {
        id: resizeHandleRight
        width: 14
        height: 14
        visible: root.selected
        color: "#ff8a00"
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        z: 10

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeHorCursor

            property real startWidth: 0
            property real startMouseX: 0

            onPressed: function(mouse) {
                root.selectedRequested(root.widgetId)
                startWidth = root.width
                startMouseX = mouse.x
            }

            onPositionChanged: function(mouse) {
                if (!pressed)
                    return

                root.width = Math.max(32, startWidth + mouse.x - startMouseX)
            }

            onReleased: {
                root.geometryCommitted(root.widgetId, root.x, root.y, root.width, root.height)
            }
        }
    }

    Rectangle {
        id: resizeHandleBottomRight
        width: 14
        height: 14
        visible: root.selected
        color: "#ff8a00"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        z: 10

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeFDiagCursor

            property real startWidth: 0
            property real startHeight: 0
            property real startMouseX: 0
            property real startMouseY: 0

            onPressed: function(mouse) {
                root.selectedRequested(root.widgetId)
                startWidth = root.width
                startHeight = root.height
                startMouseX = mouse.x
                startMouseY = mouse.y
            }

            onPositionChanged: function(mouse) {
                if (!pressed)
                    return

                root.width = Math.max(32, startWidth + mouse.x - startMouseX)
                root.height = Math.max(24, startHeight + mouse.y - startMouseY)
            }

            onReleased: {
                root.geometryCommitted(root.widgetId, root.x, root.y, root.width, root.height)
            }
        }
    }

    Rectangle {
        id: resizeHandleBottom
        width: 14
        height: 14
        visible: root.selected
        color: "#ff8a00"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        z: 10

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeVerCursor

            property real startHeight: 0
            property real startMouseY: 0

            onPressed: function(mouse) {
                root.selectedRequested(root.widgetId)
                startHeight = root.height
                startMouseY = mouse.y
            }

            onPositionChanged: function(mouse) {
                if (!pressed)
                    return

                root.height = Math.max(24, startHeight + mouse.y - startMouseY)
            }

            onReleased: {
                root.geometryCommitted(root.widgetId, root.x, root.y, root.width, root.height)
            }
        }
    }

    Rectangle {
        id: resizeHandleBottomLeft
        width: 14
        height: 14
        visible: root.selected
        color: "#ff8a00"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        z: 10

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeBDiagCursor

            property real startWidth: 0
            property real startHeight: 0
            property real startX: 0
            property real startMouseX: 0
            property real startMouseY: 0

            onPressed: function(mouse) {
                root.selectedRequested(root.widgetId)
                startWidth = root.width
                startHeight = root.height
                startX = root.x
                startMouseX = mouse.x
                startMouseY = mouse.y
            }

            onPositionChanged: function(mouse) {
                if (!pressed)
                    return

                var dx = mouse.x - startMouseX
                var dy = mouse.y - startMouseY
                var newWidth = Math.max(32, startWidth - dx)
                root.width = newWidth
                root.height = Math.max(24, startHeight + dy)
                root.x = startX + (startWidth - newWidth)
            }

            onReleased: {
                root.geometryCommitted(root.widgetId, root.x, root.y, root.width, root.height)
            }
        }
    }

    Rectangle {
        id: resizeHandleLeft
        width: 14
        height: 14
        visible: root.selected
        color: "#ff8a00"
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        z: 10

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeHorCursor

            property real startWidth: 0
            property real startX: 0
            property real startMouseX: 0

            onPressed: function(mouse) {
                root.selectedRequested(root.widgetId)
                startWidth = root.width
                startX = root.x
                startMouseX = mouse.x
            }

            onPositionChanged: function(mouse) {
                if (!pressed)
                    return

                var dx = mouse.x - startMouseX
                var newWidth = Math.max(32, startWidth - dx)
                root.width = newWidth
                root.x = startX + (startWidth - newWidth)
            }

            onReleased: {
                root.geometryCommitted(root.widgetId, root.x, root.y, root.width, root.height)
            }
        }
    }
}
