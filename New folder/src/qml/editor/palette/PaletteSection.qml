import QtQuick
import QtQuick.Controls

Column {
    id: root

    signal itemSelected(string type)

    property string title: "Section"
    property var items: []
    property string filterText: ""

    spacing: 0
    width: parent ? parent.width : 250
    visible: visibleItemCount > 0

    readonly property string normalizedFilter: filterText.toLowerCase()
    readonly property int visibleItemCount: {
        if (normalizedFilter.length === 0)
            return items.length

        var count = 0
        for (var i = 0; i < items.length; ++i) {
            var item = items[i]
            if (item.name.toLowerCase().indexOf(normalizedFilter) >= 0
                    || item.type.toLowerCase().indexOf(normalizedFilter) >= 0
                    || item.description.toLowerCase().indexOf(normalizedFilter) >= 0) {
                ++count
            }
        }

        return count
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
            model: root.items

            delegate: PaletteItem {
                readonly property bool matchesFilter: root.normalizedFilter.length === 0
                                                  || modelData.name.toLowerCase().indexOf(root.normalizedFilter) >= 0
                                                  || modelData.type.toLowerCase().indexOf(root.normalizedFilter) >= 0
                                                  || modelData.description.toLowerCase().indexOf(root.normalizedFilter) >= 0

                width: Math.floor((root.width - 18) / 3)
                visible: matchesFilter
                height: matchesFilter ? implicitHeight : 0

                widgetName: modelData.name
                widgetType: modelData.type
                description: modelData.description
                iconText: modelData.icon

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
