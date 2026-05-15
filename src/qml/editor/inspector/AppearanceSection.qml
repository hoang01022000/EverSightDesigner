import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root

    Layout.fillWidth: true
    spacing: 10

    function commitAppearance() {
        if (!canvasViewModel.hasSelection)
            return

        canvasViewModel.updateSelectedAppearance(buttonColorField.text,
                                                 borderColorField.text,
                                                 iconColorField.text)
    }

    InspectorSectionHeader { text: "Appearance" }

    ColumnLayout {
        Layout.fillWidth: true
        Layout.leftMargin: 16
        Layout.rightMargin: 16
        spacing: 10

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Label { text: "Background"; color: "#555b62"; font.pixelSize: 14; Layout.preferredWidth: 80 }
            ColorSwatch { swatchColor: buttonColorField.text }
            TextField {
                id: buttonColorField
                Layout.fillWidth: true
                text: canvasViewModel.selectedButtonColor
                onEditingFinished: root.commitAppearance()
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Label { text: "Border Color"; color: "#555b62"; font.pixelSize: 14; Layout.preferredWidth: 80 }
            ColorSwatch { swatchColor: borderColorField.text }
            TextField {
                id: borderColorField
                Layout.fillWidth: true
                text: canvasViewModel.selectedBorderColor
                onEditingFinished: root.commitAppearance()
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Label { text: "Icon Color"; color: "#555b62"; font.pixelSize: 14; Layout.preferredWidth: 80 }
            ColorSwatch { swatchColor: iconColorField.text }
            TextField {
                id: iconColorField
                Layout.fillWidth: true
                text: canvasViewModel.selectedIconColor
                onEditingFinished: root.commitAppearance()
            }
        }
    }
}
