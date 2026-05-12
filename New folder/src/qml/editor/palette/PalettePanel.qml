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
        {
            title: "Foundation",
            items: [
                { name: "Image", type: "CameraView", icon: "IMG", description: "Image display" },
                { name: "Multiple I...", type: "DataTable", icon: "N", description: "Multi image" },
                { name: "Run Cont...", type: "RunControl", icon: "PLAY", description: "Run control" },
                { name: "Run Status", type: "Indicator", icon: "!", description: "Run status" },
                { name: "OK&NG", type: "Indicator", icon: "OK", description: "OK / NG" },
                { name: "Textbox", type: "TextInput", icon: "T", description: "Text input" },
                { name: "Button", type: "Button", icon: "BTN", description: "Command" },
                { name: "Switch C...", type: "Toggle", icon: "SW", description: "Switch" },
                { name: "Params S...", type: "ParameterSet", icon: "SET", description: "Parameters" },
                { name: "Variable...", type: "NumericReadout", icon: "var", description: "Variable" },
                { name: "Character...", type: "Label", icon: "A", description: "Text label" },
                { name: "Traffic Li...", type: "TrafficLight", icon: "L", description: "Traffic light" },
                { name: "Conditio...", type: "Condition", icon: "IF", description: "Condition" }
            ]
        },
        {
            title: "Layout",
            items: [
                { name: "Group Box", type: "GroupBox", icon: "BOX", description: "Group" },
                { name: "Child Inte...", type: "ChildInterface", icon: "CH", description: "Child screen" },
                { name: "Tab Cont...", type: "TabContainer", icon: "TAB", description: "Tabs" }
            ]
        },
        {
            title: "Chart",
            items: [
                { name: "Table", type: "DataTable", icon: "TBL", description: "Table" },
                { name: "Line Diag...", type: "TrendChart", icon: "LIN", description: "Line chart" },
                { name: "Multi Lin...", type: "MultiTrend", icon: "ML", description: "Multi line" }
            ]
        }
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
                        items: modelData.items
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
