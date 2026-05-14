import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    signal widgetRequested(string type)

    color: "#f6f7f8"

    Layout.preferredWidth: 250
    Layout.fillHeight: true

    readonly property var sections: [
        { title: "Foundation", category: 0 },
        { title: "Layout", category: 1 },
        { title: "Chart", category: 2 }
    ]

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        TextField {
            id: searchField

            Layout.fillWidth: true
            Layout.leftMargin: 12
            Layout.rightMargin: 12
            Layout.topMargin: 10
            placeholderText: "Search controls"
            color: "#3f454b"
            placeholderTextColor: "#9da3a8"
            selectByMouse: true

            background: Rectangle {
                radius: 2
                color: "#ffffff"
                border.color: searchField.activeFocus ? "#ff8a00" : "#d5d8dc"
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            background: Rectangle {
                color: "transparent"
            }

            Column {
                width: Math.max(1, root.width)
                spacing: 0

                Repeater {
                    model: root.sections

                    delegate: PaletteSection {
                        width: parent.width
                        title: modelData.title
                        category: modelData.category
                        filterText: searchField.text

                        onItemSelected: function(type) {
                            root.widgetRequested(type)
                        }
                    }
                }
            }
        }
    }
}
