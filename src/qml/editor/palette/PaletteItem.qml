import QtQuick
import QtQuick.Controls

Item {
    id: root

    property string widgetName: "Image"
    property string widgetType: "CameraView"
    property string description: "Runtime control"
    property string iconSource: "assets/icons/Image.png"

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

        // ✅ hotSpot cố định ở tâm item — chuẩn hơn cho palette drag
        Drag.hotSpot.x: root.width / 2
        Drag.hotSpot.y: root.height / 2

        // ✅ Dùng mimeData để truyền widgetType thay vì Drag.text
        Drag.mimeData: {
            "text/plain": root.widgetType
        }
        Drag.active: false
        Drag.dragType: Drag.Automatic

        onPressed: {
            Drag.active = true
        }
        onReleased: {
            Drag.active = false
        }
        onClicked: {
            if (!Drag.active)
                root.itemClicked(root.widgetType)
        }
    }
}
