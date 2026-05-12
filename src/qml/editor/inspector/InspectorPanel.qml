import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    color: "#f4f5f6"
    implicitWidth: 318

    function commitGeometry() {
        if (!canvasViewModel.hasSelection)
            return

        canvasViewModel.updateWidgetGeometry(
                    canvasViewModel.selectedWidgetId,
                    Number(xField.text),
                    Number(yField.text),
                    Number(widthField.text),
                    Number(heightField.text))
    }

    function commitData() {
        if (!canvasViewModel.hasSelection)
            return

        canvasViewModel.updateSelectedData(dataSourceBox.currentText, controlTypeBox.currentText)
    }

    function commitAppearance() {
        if (!canvasViewModel.hasSelection)
            return

        canvasViewModel.updateSelectedAppearance(buttonColorField.text,
                                                 borderColorField.text,
                                                 iconColorField.text)
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            implicitHeight: 46
            color: "#f1f1f1"

            Label {
                anchors.left: parent.left
                anchors.leftMargin: 14
                anchors.verticalCenter: parent.verticalCenter
                text: "Base Settings"
                color: "#2f3338"
                font.pixelSize: 16
                font.bold: true
            }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height: 3
                color: "#ff7a00"
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            ColumnLayout {
                width: root.width
                anchors.margins: 15
                spacing: 10

                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: canvasViewModel.hasSelection ? 0 : 130
                    visible: !canvasViewModel.hasSelection
                    color: "transparent"

                    Label {
                        anchors.centerIn: parent
                        text: "Select a control"
                        color: "#7a8086"
                        font.pixelSize: 15
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    visible: canvasViewModel.hasSelection
                    spacing: 10

                    // Identity Section
                    SectionHeader { text: "Identity" }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.leftMargin: 16
                        Layout.rightMargin: 16
                        spacing: 10

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 12

                            Label {
                                text: "Type"
                                color: "#555b62"
                                font.pixelSize: 14
                                Layout.preferredWidth: 80
                            }
                            TextField {
                                Layout.fillWidth: true
                                enabled: false
                                text: canvasViewModel.selectedWidgetType
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 12

                            Label {
                                text: "Title"
                                color: "#555b62"
                                font.pixelSize: 14
                                Layout.preferredWidth: 80
                            }
                            TextField {
                                id: titleField
                                Layout.fillWidth: true
                                text: canvasViewModel.selectedWidgetTitle
                                selectByMouse: true
                                onEditingFinished: canvasViewModel.updateSelectedTitle(text)
                            }
                        }
                    }

                    Divider {}

                    // Data Source Section
                    SectionHeader { text: "Data Source" }

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
                                Component.onCompleted: currentIndex = Math.max(0, indexOfValue(canvasViewModel.selectedDataSource))
                                onActivated: root.commitData()
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
                                Component.onCompleted: currentIndex = Math.max(0, indexOfValue(canvasViewModel.selectedControlType))
                                onActivated: root.commitData()
                            }
                        }
                    }

                    Divider {}

                    // Layout Section
                    SectionHeader { text: "Layout" }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.leftMargin: 16
                        Layout.rightMargin: 16
                        spacing: 10

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 12

                            Label {
                                text: "Auto Fill"
                                color: "#555b62"
                                font.pixelSize: 14
                                Layout.preferredWidth: 80
                            }
                            Switch {
                                checked: canvasViewModel.selectedAutoFill
                                onToggled: canvasViewModel.updateSelectedAutoFill(checked)
                            }
                            Item { Layout.fillWidth: true }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            Label {
                                text: "X"
                                color: "#555b62"
                                font.pixelSize: 14
                                Layout.preferredWidth: 40
                            }
                            TextField {
                                id: xField
                                Layout.fillWidth: true
                                text: Math.round(canvasViewModel.selectedWidgetX)
                                inputMethodHints: Qt.ImhFormattedNumbersOnly
                                onEditingFinished: root.commitGeometry()
                            }

                            Label {
                                text: "Y"
                                color: "#555b62"
                                font.pixelSize: 14
                                Layout.preferredWidth: 40
                            }
                            TextField {
                                id: yField
                                Layout.fillWidth: true
                                text: Math.round(canvasViewModel.selectedWidgetY)
                                inputMethodHints: Qt.ImhFormattedNumbersOnly
                                onEditingFinished: root.commitGeometry()
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            Label {
                                text: "W"
                                color: "#555b62"
                                font.pixelSize: 14
                                Layout.preferredWidth: 40
                            }
                            TextField {
                                id: widthField
                                Layout.fillWidth: true
                                text: Math.round(canvasViewModel.selectedWidgetWidth)
                                inputMethodHints: Qt.ImhFormattedNumbersOnly
                                onEditingFinished: root.commitGeometry()
                            }

                            Label {
                                text: "H"
                                color: "#555b62"
                                font.pixelSize: 14
                                Layout.preferredWidth: 40
                            }
                            TextField {
                                id: heightField
                                Layout.fillWidth: true
                                text: Math.round(canvasViewModel.selectedWidgetHeight)
                                inputMethodHints: Qt.ImhFormattedNumbersOnly
                                onEditingFinished: root.commitGeometry()
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 7

                            Label {
                                text: "Align"
                                color: "#555b62"
                                font.pixelSize: 14
                                Layout.preferredWidth: 80
                            }
                            Button {
                                text: "L"
                                Layout.fillWidth: true
                                onClicked: canvasViewModel.alignSelected("left", 1280, 720)
                            }
                            Button {
                                text: "C"
                                Layout.fillWidth: true
                                onClicked: canvasViewModel.alignSelected("hcenter", 1280, 720)
                            }
                            Button {
                                text: "R"
                                Layout.fillWidth: true
                                onClicked: canvasViewModel.alignSelected("right", 1280, 720)
                            }
                            Button {
                                text: "T"
                                Layout.fillWidth: true
                                onClicked: canvasViewModel.alignSelected("top", 1280, 720)
                            }
                            Button {
                                text: "M"
                                Layout.fillWidth: true
                                onClicked: canvasViewModel.alignSelected("vcenter", 1280, 720)
                            }
                            Button {
                                text: "B"
                                Layout.fillWidth: true
                                onClicked: canvasViewModel.alignSelected("bottom", 1280, 720)
                            }
                        }
                    }

                    Divider {}

                    // Appearance Section
                    SectionHeader { text: "Appearance" }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.leftMargin: 16
                        Layout.rightMargin: 16
                        spacing: 10

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 12

                            Label {
                                text: "Button Color"
                                color: "#555b62"
                                font.pixelSize: 14
                                Layout.preferredWidth: 80
                            }
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

                            Label {
                                text: "Border Color"
                                color: "#555b62"
                                font.pixelSize: 14
                                Layout.preferredWidth: 80
                            }
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

                            Label {
                                text: "Icon Color"
                                color: "#555b62"
                                font.pixelSize: 14
                                Layout.preferredWidth: 80
                            }
                            ColorSwatch { swatchColor: iconColorField.text }
                            TextField {
                                id: iconColorField
                                Layout.fillWidth: true
                                text: canvasViewModel.selectedIconColor
                                onEditingFinished: root.commitAppearance()
                            }
                        }
                    }

                    Button {
                        Layout.fillWidth: true
                        Layout.leftMargin: 16
                        Layout.rightMargin: 16
                        Layout.topMargin: 16
                        text: "Delete Control"
                        onClicked: canvasViewModel.removeSelectedWidget()
                    }
                }
            }
        }
    }

    Connections {
        target: canvasViewModel

        function onSelectedWidgetChanged() {
            if (dataSourceBox) {
                dataSourceBox.currentIndex = Math.max(0, dataSourceBox.indexOfValue(canvasViewModel.selectedDataSource))
            }

            if (controlTypeBox) {
                controlTypeBox.currentIndex = Math.max(0, controlTypeBox.indexOfValue(canvasViewModel.selectedControlType))
            }
        }
    }

    component SectionHeader: Label {
        Layout.fillWidth: true
        Layout.leftMargin: 16
        Layout.topMargin: 14
        Layout.bottomMargin: 8
        color: "#4f555b"
        font.pixelSize: 18
        font.bold: true
    }

    component Divider: Rectangle {
        Layout.fillWidth: true
        Layout.leftMargin: 16
        Layout.rightMargin: 16
        implicitHeight: 1
        color: "#d8dadd"
    }

    component ColorSwatch: Rectangle {
        required property string swatchColor
        Layout.preferredWidth: 32
        Layout.preferredHeight: 26
        color: swatchColor
        border.color: "#b8bcc1"
    }
}