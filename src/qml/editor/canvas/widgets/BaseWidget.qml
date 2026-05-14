// BaseWidget.qml
import QtQuick

Item {
    id: root
    property bool isSelected: false
    property string widgetTitle: ""
    property color accentColor: "#ff7a00" // Màu cam đặc trưng của bạn

    // Nội dung thực tế của Widget (Button, Image, Chart...) sẽ được ném vào đây
    property alias content: container.children

    Rectangle {
        id: container
        anchors.fill: parent
        color: "transparent"
        clip: true

        // Khung viền chỉ hiện khi được chọn (Design Mode)
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.color: root.accentColor
            border.width: root.isSelected ? 2 : 0
            z: 100 // Đảm bảo luôn nằm trên cùng
        }
    }
}