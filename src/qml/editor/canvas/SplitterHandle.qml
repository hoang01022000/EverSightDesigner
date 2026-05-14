import QtQuick

Item {
    id: root

    property string orientation: "vertical"
    property Item coordinateItem
    readonly property bool vertical: orientation === "vertical"
    readonly property bool active: dragArea.pressed
    readonly property bool hovered: hoverHandler.hovered

    signal dragged(real deltaRatio)
    signal mergeRequested()

    implicitWidth: vertical ? 12 : 80
    implicitHeight: vertical ? 80 : 12

    Rectangle {
        anchors.centerIn: parent
        width: root.vertical ? (root.active || root.hovered ? 4 : 2) : parent.width
        height: root.vertical ? parent.height : (root.active || root.hovered ? 4 : 2)
        radius: 1
        color: root.active ? "#ff8a1c" : (root.hovered ? "#aeb6bf" : "#69727a")
    }

    Rectangle {
        anchors.centerIn: parent
        width: root.vertical ? 10 : 28
        height: root.vertical ? 28 : 10
        radius: 2
        color: root.active ? "#ff8a1c" : (root.hovered ? "#2f3438" : "transparent")
        border.color: root.active ? "#ffb15f" : (root.hovered ? "#8e98a1" : "transparent")
        border.width: root.active || root.hovered ? 1 : 0
    }

    HoverHandler {
        id: hoverHandler
        cursorShape: root.vertical ? Qt.SizeHorCursor : Qt.SizeVerCursor
    }

    MouseArea {
        id: dragArea
        anchors.fill: parent
        anchors.margins: -6
        acceptedButtons: Qt.LeftButton
        cursorShape: root.vertical ? Qt.SizeHorCursor : Qt.SizeVerCursor
        preventStealing: true

        property real lastPosition: 0

        onPressed: function(mouse) {
            var point = root.coordinateItem ? root.mapToItem(root.coordinateItem, mouse.x, mouse.y)
                                            : Qt.point(mouse.x, mouse.y)
            lastPosition = root.vertical ? point.x : point.y
        }

        onPositionChanged: function(mouse) {
            if (!pressed)
                return

            var point = root.coordinateItem ? root.mapToItem(root.coordinateItem, mouse.x, mouse.y)
                                            : Qt.point(mouse.x, mouse.y)
            var currentPosition = root.vertical ? point.x : point.y
            var deltaPixels = currentPosition - lastPosition
            lastPosition = currentPosition

            var divisor = root.vertical && root.coordinateItem ? root.coordinateItem.width
                        : (!root.vertical && root.coordinateItem ? root.coordinateItem.height : 1)
            if (divisor > 0)
                root.dragged(deltaPixels / divisor)
        }

        onDoubleClicked: root.mergeRequested()
    }
}
