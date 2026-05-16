import QtQuick

Item {
    id: root

    property string orientation: "vertical"
    property Item coordinateItem
    property int parentCellId: -1
    readonly property bool vertical: orientation === "vertical"
    readonly property bool active: dragHandler.active
    readonly property bool hovered: hoverHandler.hovered

    signal dragged(real deltaRatio)
    signal dragStarted()
    signal dragFinished()
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

    TapHandler {
        acceptedButtons: Qt.LeftButton
        onDoubleTapped: root.mergeRequested()
    }

    DragHandler {
        id: dragHandler
        target: null
        acceptedButtons: Qt.LeftButton

        property real lastTranslation: 0

        onActiveChanged: {
            if (active) {
                lastTranslation = root.vertical ? translation.x : translation.y
                root.dragStarted()
            } else {
                root.dragFinished()
            }
        }

        onTranslationChanged: {
            if (!active)
                return

            var currentTranslation = root.vertical ? translation.x : translation.y
            var deltaPixels = currentTranslation - lastTranslation
            lastTranslation = currentTranslation
            var divisor = root.vertical && root.coordinateItem ? root.coordinateItem.width
                        : (!root.vertical && root.coordinateItem ? root.coordinateItem.height : 1)
            if (divisor > 0)
                root.dragged(deltaPixels / divisor)
        }
    }
}
