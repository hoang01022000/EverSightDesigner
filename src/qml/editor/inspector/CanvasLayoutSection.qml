import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root

    Layout.fillWidth: true
    Layout.leftMargin: 16
    Layout.rightMargin: 16
    visible: !canvasViewModel.hasSelection
    spacing: 10

    readonly property var basicLayoutOptions: [
        { label: "1Screen", value: 1 },
        { label: "2H", value: 2 },
        { label: "2V", value: 3 },
        { label: "3H", value: 4 },
        { label: "3V", value: 5 },
        { label: "4Screen", value: 6 },
        { label: "6Wide", value: 7 },
        { label: "6Tall", value: 8 },
        { label: "9Screen", value: 9 }
    ]

    InspectorSectionHeader {
        Layout.leftMargin: 0
        text: "Layout Split Style"
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
                text: option.label
                checkable: true
                checked: canvasViewModel.currentBasicLayout === option.value
                onClicked: canvasViewModel.setBasicLayout(option.value)
            }
        }
    }

    RowLayout {
        Layout.fillWidth: true
        spacing: 8

        Label {
            text: "Custom"
            color: "#555b62"
            Layout.preferredWidth: 70
        }

        SpinBox {
            id: customRowsBox
            from: 1
            to: 10
            value: canvasViewModel.currentCustomRows
            editable: true
            Layout.fillWidth: true
        }

        Label { text: "x"; color: "#555b62" }

        SpinBox {
            id: customColumnsBox
            from: 1
            to: 10
            value: canvasViewModel.currentCustomColumns
            editable: true
            Layout.fillWidth: true
        }

        Button {
            text: "Apply"
            onClicked: canvasViewModel.setCustomLayout(customRowsBox.value, customColumnsBox.value)
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
                checkable: true
                checked: canvasViewModel.currentSplitTemplate === index + 1
                onClicked: canvasViewModel.setSplitTemplate(index + 1)
            }
        }
    }

    RowLayout {
        Layout.fillWidth: true
        spacing: 8

        Button {
            Layout.fillWidth: true
            enabled: canvasViewModel.hasSelectedLayoutCell
            text: "Split 2 H"
            onClicked: canvasViewModel.splitSelectedRegionHorizontal()
        }

        Button {
            Layout.fillWidth: true
            enabled: canvasViewModel.hasSelectedLayoutCell
            text: "Split 2 V"
            onClicked: canvasViewModel.splitSelectedRegionVertical()
        }

        Button {
            Layout.fillWidth: true
            enabled: canvasViewModel.hasSelectedLayoutCell
            text: "Split 1"
            onClicked: canvasViewModel.mergeSelectedLayoutCells()
        }
    }

    InspectorDivider {
        Layout.leftMargin: 0
        Layout.rightMargin: 0
    }
}
