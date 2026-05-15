import QtQuick
import QtQuick.Controls

Column {
    id: root

    signal itemSelected(string type)

    property string title: "Section"
    // category number (0:Foundation,1:Layout,2:Chart,3:Other)
    property int category: 0
    property string filterText: ""
    spacing: 0
    width: parent ? parent.width : 250
    visible: filteredItems.length > 0

    readonly property string normalizedFilter: filterText.toLowerCase()
    readonly property var filteredItems: {
        var items = []
        for (var i = 0; i < paletteModel.count; ++i) {
            var it = paletteModel.get(i)
            if (it.category !== root.category) continue
            if (normalizedFilter.length === 0) {
                items.push(it)
                continue
            }
            var name = it.displayName ? it.displayName.toLowerCase() : ""
            var type = it.typeId ? it.typeId.toLowerCase() : ""
            var desc = it.description ? it.description.toLowerCase() : ""
            if (name.indexOf(normalizedFilter) >= 0 || type.indexOf(normalizedFilter) >= 0 || desc.indexOf(normalizedFilter) >= 0)
                items.push(it)
        }
        return items
    }

    Rectangle {
        width: root.width
        height: 42
        color: "#f6f7f8"

        Label {
            anchors.left: parent.left
            anchors.leftMargin: 14
            anchors.verticalCenter: parent.verticalCenter
            text: root.title
            color: "#2f3338"
            font.bold: true
            font.pixelSize: 18
        }

        Label {
            anchors.right: parent.right
            anchors.rightMargin: 18
            anchors.verticalCenter: parent.verticalCenter
            text: "⌄"
            color: "#687078"
            font.pixelSize: 18
        }
    }

    Grid {
        width: root.width
        columns: 3
        columnSpacing: 3
        rowSpacing: 10
        leftPadding: 6
        rightPadding: 6
        bottomPadding: 16

        Repeater {
            model: root.filteredItems

            delegate: PaletteItem {
                width: Math.floor((root.width - 18) / 3)

                widgetName: modelData.displayName
                widgetType: modelData.typeId
                description: modelData.description
                iconSource: modelData.iconSource ? modelData.iconSource : "assets/icons/Image.png"

                onItemClicked: function(type) {
                    root.itemSelected(type)
                }
            }
        }
    }

    Rectangle {
        width: root.width - 28
        x: 14
        height: 1
        color: "#dcdfe2"
    }
}
