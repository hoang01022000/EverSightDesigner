import QtQuick
import QtQuick.Controls

Item {
    id: root

    property string widgetName: "Image"
    property string widgetType: "CameraView"
    property string description: "Runtime control"
    property string iconSource: "qrc:/qt/qml/EverSightDesigner/src/qml/editor/palette/assets/icons/Image.png"

    signal itemClicked(string type)

    implicitHeight: 76

    Rectangle {
        anchors.fill: parent
        radius: 2
        color: mouseArea.containsMouse ? "#ffffff" : "transparent"
        border.color: mouseArea.containsMouse ? "#ff8a00" : "transparent"
    }

    Rectangle {
        id: iconBox
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter
        width: 38
        height: 28
        radius: 4
        color: "transparent"

        Image {
            anchors.fill: parent
            source: root.iconSource
            fillMode: Image.PreserveAspectFit
            smooth: true
        }
    }

    Label {
        anchors.top: iconBox.bottom
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.right: parent.right
        text: root.widgetName
        color: "#50565c"
        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideRight
        font.pixelSize: 13
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        drag.target: dragProxy
        drag.threshold: 8

        onPressed: {
            dragProxy.x = 0
            dragProxy.y = 0
        }

        onReleased: {
            dragProxy.x = 0
            dragProxy.y = 0
        }

        onClicked: {
            if (!drag.active)
                root.itemClicked(root.widgetType)
        }
    }

    Item {
        id: dragProxy
        width: 1
        height: 1
        visible: false
        Drag.active: mouseArea.drag.active
        Drag.hotSpot.x: root.width / 2
        Drag.hotSpot.y: root.height / 2
        Drag.dragType: Drag.Automatic
        Drag.mimeData: {
            "text/plain": root.widgetType,
            "application/eversight-widget-type": root.widgetType
        }
    }
}
