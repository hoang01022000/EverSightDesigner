import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root

    readonly property var definitions: canvasViewModel.selectedPropertyDefinitions || []

    Layout.fillWidth: true
    visible: definitions.length > 0
    spacing: 8

    InspectorDivider {}
    InspectorSectionHeader { text: canvasViewModel.selectedWidgetType + " Settings" }

    Repeater {
        Layout.fillWidth: true
        model: root.definitions.length

        delegate: RowLayout {
            id: propertyEditor

            required property int index
            readonly property var definition: root.definitions[index] || ({})
            readonly property string propertyKey: definition.key || ""
            readonly property string editorType: definition.editor || "text"
            readonly property var propertyValue: canvasViewModel.selectedPropertyValue(propertyKey)

            Layout.leftMargin: 16
            Layout.rightMargin: 16
            spacing: 12

            Label {
                text: propertyEditor.definition.label || propertyEditor.propertyKey
                color: "#555b62"
                font.pixelSize: 14
                Layout.preferredWidth: 120
                wrapMode: Text.WordWrap
            }

            Loader {
                Layout.fillWidth: true
                sourceComponent: {
                    if (propertyEditor.editorType === "bool")
                        return boolEditor
                    if (propertyEditor.editorType === "choice")
                        return choiceEditor
                    if (propertyEditor.editorType === "color")
                        return colorEditor
                    return textEditor
                }
            }

            Component {
                id: textEditor

                TextField {
                    text: propertyEditor.propertyValue === undefined ? "" : String(propertyEditor.propertyValue)
                    selectByMouse: true
                    inputMethodHints: propertyEditor.editorType === "int" || propertyEditor.editorType === "real"
                                      ? Qt.ImhFormattedNumbersOnly
                                      : Qt.ImhNone
                    onEditingFinished: canvasViewModel.updateSelectedProperty(propertyEditor.propertyKey, text)
                }
            }

            Component {
                id: boolEditor

                RowLayout {
                    Switch {
                        checked: propertyEditor.propertyValue === true || propertyEditor.propertyValue === "true"
                        onToggled: canvasViewModel.updateSelectedProperty(propertyEditor.propertyKey, checked)
                    }
                    Item { Layout.fillWidth: true }
                }
            }

            Component {
                id: choiceEditor

                ComboBox {
                    model: propertyEditor.definition.options || []
                    Component.onCompleted: currentIndex = Math.max(0, indexOfValue(String(propertyEditor.propertyValue)))
                    onActivated: canvasViewModel.updateSelectedProperty(propertyEditor.propertyKey, currentText)
                }
            }

            Component {
                id: colorEditor

                RowLayout {
                    ColorSwatch {
                        swatchColor: colorField.text
                    }
                    TextField {
                        id: colorField
                        Layout.fillWidth: true
                        text: propertyEditor.propertyValue === undefined ? "" : String(propertyEditor.propertyValue)
                        selectByMouse: true
                        onEditingFinished: canvasViewModel.updateSelectedProperty(propertyEditor.propertyKey, text)
                    }
                }
            }
        }
    }
}
