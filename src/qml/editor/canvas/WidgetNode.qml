import QtQuick
import QtQuick.Controls

Item {
    id: root

    // --- Properties kết nối với Model/Logic ---
    property int widgetId: -1
    property string widgetType: "Widget"
    property string widgetTitle: "Widget"
    property string buttonColor: "#2f2f2f"
    property string borderColorValue: "#272727"
    property string iconColor: "#f6f6f6"
    property bool selected: false

    // --- Signals ---
    signal selectedRequested(int id)
    signal geometryCommitted(int id, real newX, real newY, real newWidth, real newHeight)

    // --- Logic tự động tìm Widget theo thư mục ---
    function getWidgetSource(type) {
        // Các typeId đầy đủ theo Widget Configuration Reference
        const foundationList = ["Image", "MultipleImages", "RunControl", "RunStatus", "OKNG", "Textbox", "Button", "SwitchControl", "ParamsSettings", "VariableSettings", "CharacterSettings", "TrafficLight", "ConditionalLight"];
        const layoutList = ["GroupBox", "ChildInterface", "TabControl"];
        const chartList = ["Table", "LineDiagramArray", "MultiLineDiagram", "ProductionStatistics", "PieControl", "ParamSettingsArray", "LabelArray"];

        var subFolder = "Foundation";
        var fileName = type;

        if (layoutList.indexOf(type) !== -1) {
            subFolder = "Layout";
        } else if (chartList.indexOf(type) !== -1) {
            subFolder = "Chart";
        } else if (foundationList.indexOf(type) === -1) {
            return "widgets/PlaceholderWidget.qml";
        }

        // Những ánh xạ tên file đặc thù (nếu file QML vật lý khác tên typeId)
        if (type === "MultipleImages") fileName = "Image";
        if (type === "SwitchControl") fileName = "Button";
        if (type === "DataTable") fileName = "Table";

        var candidate = "widgets/" + subFolder + "/" + fileName + "Widget.qml";

        // Nếu file QML cụ thể chưa tồn tại, trả về PlaceholderWidget.qml
        try {
            var comp = Qt.createComponent(candidate);
            if (comp.status === Component.Ready) return candidate;
            return "widgets/PlaceholderWidget.qml";
        } catch (e) {
            return "widgets/PlaceholderWidget.qml";
        }
    }

    // --- Khung hiển thị khi được chọn ---
    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: root.selected ? "#1e9bff" : "transparent"
        border.width: root.selected ? 2 : 0
        z: 1
    }

    // --- Nội dung Widget hiển thị ---
    Loader {
        id: widgetLoader
        anchors.fill: parent
        anchors.margins: root.selected ? 2 : 0
        source: getWidgetSource(root.widgetType)

        onLoaded: {
            if (!item) return
            // Ràng buộc dữ liệu từ Node xuống Widget con (Appearance settings)
            item.bgColor = Qt.binding(() => root.buttonColor)
            item.borderColor = Qt.binding(() => root.borderColorValue)
            item.fontColor = Qt.binding(() => root.iconColor)

            // Kiểm tra thuộc tính tồn tại trước khi bind để tránh lỗi log
            if (item.hasOwnProperty("labelText")) item.labelText = Qt.binding(() => root.widgetTitle)
            if (item.hasOwnProperty("showText")) item.showText = Qt.binding(() => root.widgetTitle)
        }
    }

    // --- MouseArea chính để Di chuyển (Drag) ---
    MouseArea {
        anchors.fill: parent
        drag.target: root
        drag.threshold: 8

        onPressed: {
            root.z = 999 // Đưa lên trên cùng khi đang thao tác
            root.selectedRequested(root.widgetId)
        }
        onReleased: {
            root.z = 1
            root.geometryCommitted(root.widgetId, root.x, root.y, root.width, root.height)
        }
    }

    // --- RE-SIZE HANDLES (Gồm 8 điểm điều hướng) ---
    // Component con dùng chung cho các góc và cạnh
    component ResizeHandle: Rectangle {
        property int handlePos: 0 // 0:TL, 1:T, 2:TR, 3:R, 4:BR, 5:B, 6:BL, 7:L
        width: 10; height: 10
        color: "#ffffff"
        border.color: "#1e9bff"
        border.width: 1
        visible: root.selected
        z: 1000

        MouseArea {
            anchors.fill: parent
            anchors.margins: -5 // Tăng diện tích nhận diện chuột
            cursorShape: {
                if (handlePos === 0 || handlePos === 4) return Qt.SizeFDiagCursor
                if (handlePos === 2 || handlePos === 6) return Qt.SizeBDiagCursor
                if (handlePos === 1 || handlePos === 5) return Qt.SizeVerCursor
                return Qt.SizeHorCursor
            }

            property real sW; property real sH
            property real sX; property real sY
            property real sMX; property real sMY

            onPressed: (mouse) => {
                sW = root.width; sH = root.height
                sX = root.x; sY = root.y
                sMX = mouse.x; sMY = mouse.y
                root.selectedRequested(root.widgetId)
            }

            onPositionChanged: (mouse) => {
                let dx = mouse.x - sMX
                let dy = mouse.y - sMY

                // Logic Resize chuẩn xác cho cả 8 hướng
                if (handlePos <= 2) { // Top side
                    let newH = Math.max(20, sH - dy)
                    root.y = sY + (sH - newH)
                    root.height = newH
                }
                if (handlePos >= 4 && handlePos <= 6) { // Bottom side
                    root.height = Math.max(20, sH + dy)
                }
                if (handlePos === 0 || handlePos === 7 || handlePos === 6) { // Left side
                    let newW = Math.max(20, sW - dx)
                    root.x = sX + (sW - newW)
                    root.width = newW
                }
                if (handlePos >= 2 && handlePos <= 4) { // Right side
                    root.width = Math.max(20, sW + dx)
                }
            }
            onReleased: root.geometryCommitted(root.widgetId, root.x, root.y, root.width, root.height)
        }
    }

    // Đặt 8 điểm điều khiển vào các góc và trung điểm cạnh
    ResizeHandle { handlePos: 0; anchors.centerIn: parent.topLeft }
    ResizeHandle { handlePos: 1; anchors.centerIn: parent.top }
    ResizeHandle { handlePos: 2; anchors.centerIn: parent.topRight }
    ResizeHandle { handlePos: 3; anchors.centerIn: parent.right }
    ResizeHandle { handlePos: 4; anchors.centerIn: parent.bottomRight }
    ResizeHandle { handlePos: 5; anchors.centerIn: parent.bottom }
    ResizeHandle { handlePos: 6; anchors.centerIn: parent.bottomLeft }
    ResizeHandle { handlePos: 7; anchors.centerIn: parent.left }
}