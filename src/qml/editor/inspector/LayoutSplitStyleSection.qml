import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root

    Layout.fillWidth: true
    spacing: 10
    visible: canvasViewModel.hasSelectedLayoutCell

    readonly property var basicLayoutOptions: [
        { label: "1", rows: 0, columns: 0 },
        { label: "2H", rows: 2, columns: 1 },
        { label: "2V", rows: 1, columns: 2 },
        { label: "3H", rows: 3, columns: 1 },
        { label: "3V", rows: 1, columns: 3 },
        { label: "4", rows: 2, columns: 2 },
        { label: "6Wide", rows: 2, columns: 3 },
        { label: "6Tall", rows: 3, columns: 2 },
        { label: "9", rows: 3, columns: 3 }
    ]

    InspectorSectionHeader { text: "Layout Split Style" }

    ColumnLayout {
        Layout.fillWidth: true
        Layout.leftMargin: 16
        Layout.rightMargin: 16
        spacing: 10

        Label {
            Layout.fillWidth: true
            text: canvasViewModel.selectedLayoutCellCount > 1
                  ? canvasViewModel.selectedLayoutCellCount + " containers selected"
                  : canvasViewModel.selectedContainerName
            color: "#555b62"
            font.pixelSize: 14
            font.bold: true
        }

        Label {
            text: "Basic Layout"
            color: "#4f555b"
            font.pixelSize: 15
            font.bold: true
        }

        GridLayout {
            Layout.fillWidth: true
            columns: 3
            columnSpacing: 8
            rowSpacing: 8

            Repeater {
                model: root.basicLayoutOptions.length

                delegate: Button {
                    required property int index
                    readonly property var option: root.basicLayoutOptions[index]

                    Layout.fillWidth: true
                    enabled: option.label === "1"
                             ? canvasViewModel.selectedLayoutCellCount === 2
                             : canvasViewModel.selectedLayoutCellCount === 1
                    text: option.label
                    onClicked: {
                        if (option.label === "1")
                            canvasViewModel.mergeSelectedLayoutCells()
                        else
                            canvasViewModel.splitSelectedLayoutCells(option.rows, option.columns)
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            Label {
                text: "Rows"
                color: "#555b62"
                Layout.preferredWidth: 72
            }

            SpinBox {
                id: customRowsBox
                from: 1
                to: 10
                value: 2
                editable: true
                Layout.preferredWidth: 96
            }

            Item { Layout.fillWidth: true }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            Label {
                text: "Columns"
                color: "#555b62"
                Layout.preferredWidth: 72
            }

            SpinBox {
                id: customColumnsBox
                from: 1
                to: 10
                value: 2
                editable: true
                Layout.preferredWidth: 96
            }

            Button {
                text: "Apply"
                Layout.preferredWidth: 72
                enabled: canvasViewModel.selectedLayoutCellCount === 1
                onClicked: canvasViewModel.splitSelectedLayoutCells(customRowsBox.value, customColumnsBox.value)
            }
        }

        Label {
            text: "Split Template"
            color: "#4f555b"
            font.pixelSize: 15
            font.bold: true
        }

        GridLayout {
            Layout.fillWidth: true
            columns: 4
            columnSpacing: 8
            rowSpacing: 8

            Repeater {
                model: 7

                delegate: Button {
                    required property int index

                    Layout.fillWidth: true
                    text: "Template" + (index + 1)
                    onClicked: canvasViewModel.setSplitTemplate(index + 1)
                }
            }
        }
    }

    InspectorDivider {}
}
