import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root

    Layout.fillWidth: true
    spacing: 10

    readonly property bool hasOneContainer: canvasViewModel.selectedLayoutCellCount === 1
    readonly property bool hasTwoContainers: canvasViewModel.selectedLayoutCellCount === 2

    InspectorSectionHeader { text: "LayoutSplitStyle" }

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
            Layout.fillWidth: true
            text: Math.round(canvasViewModel.selectedContainerWidth) + " x "
                  + Math.round(canvasViewModel.selectedContainerHeight)
            color: "#7a8086"
            font.pixelSize: 12
        }

        GridLayout {
            Layout.fillWidth: true
            columns: 2
            columnSpacing: 8
            rowSpacing: 8

            Button {
                Layout.fillWidth: true
                enabled: root.hasOneContainer
                text: "Split Horizontal"
                onClicked: canvasViewModel.splitSelectedRegionHorizontal()
            }

            Button {
                Layout.fillWidth: true
                enabled: root.hasOneContainer
                text: "Split Vertical"
                onClicked: canvasViewModel.splitSelectedRegionVertical()
            }

            Button {
                Layout.fillWidth: true
                enabled: root.hasTwoContainers
                text: "Merge Horizontal"
                onClicked: canvasViewModel.mergeSelectedContainersHorizontal()
            }

            Button {
                Layout.fillWidth: true
                enabled: root.hasTwoContainers
                text: "Merge Vertical"
                onClicked: canvasViewModel.mergeSelectedContainersVertical()
            }
        }
    }
}
