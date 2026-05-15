import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root

    Layout.fillWidth: true
    spacing: 10

    function syncFields() {
        dataSourceBox.currentIndex = Math.max(0, dataSourceBox.indexOfValue(canvasViewModel.selectedDataSource))
        controlTypeBox.currentIndex = Math.max(0, controlTypeBox.indexOfValue(canvasViewModel.selectedControlType))
    }

    InspectorSectionHeader { text: "Data Source" }

    ColumnLayout {
        Layout.fillWidth: true
        Layout.leftMargin: 16
        Layout.rightMargin: 16
        spacing: 10

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Label {
                text: "Data Source"
                color: "#555b62"
                font.pixelSize: 14
                Layout.preferredWidth: 80
            }

            ComboBox {
                id: dataSourceBox
                Layout.fillWidth: true
                model: ["AllProcess", "Camera01", "PLC Tags", "Recipe", "InspectionResult"]
                Component.onCompleted: root.syncFields()
                onActivated: canvasViewModel.updateSelectedData(dataSourceBox.currentText, controlTypeBox.currentText)
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Label {
                text: "Control Type"
                color: "#555b62"
                font.pixelSize: 14
                Layout.preferredWidth: 80
            }

            ComboBox {
                id: controlTypeBox
                Layout.fillWidth: true
                model: ["Both Display", "Display Only", "Input Only", "Command"]
                Component.onCompleted: root.syncFields()
                onActivated: canvasViewModel.updateSelectedData(dataSourceBox.currentText, controlTypeBox.currentText)
            }
        }
    }

    Connections {
        target: canvasViewModel
        function onSelectedWidgetChanged() {
            root.syncFields()
        }
    }
}
