import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root

    Layout.fillWidth: true
    spacing: 10
    visible: canvasViewModel.selectedAppearanceFields.length > 0

    InspectorSectionHeader { text: "Appearance" }

    ColumnLayout {
        Layout.fillWidth: true
        Layout.leftMargin: 16
        Layout.rightMargin: 16
        spacing: 10

        Repeater {
            model: canvasViewModel.selectedAppearanceFields

            delegate: RowLayout {
                required property var modelData

                Layout.fillWidth: true
                spacing: 12

                Label {
                    text: modelData.label
                    color: "#555b62"
                    font.pixelSize: 14
                    Layout.preferredWidth: 92
                    elide: Text.ElideRight
                }

                ColorSwatch {
                    swatchColor: valueField.text
                    onColorPicked: function(color) {
                        valueField.text = color
                        canvasViewModel.updateSelectedAppearanceField(modelData.key, color)
                    }
                }

                TextField {
                    id: valueField
                    Layout.fillWidth: true
                    text: modelData.value
                    selectByMouse: true
                    onEditingFinished: canvasViewModel.updateSelectedAppearanceField(modelData.key, text)
                }
            }
        }
    }
}
