import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root

    Layout.fillWidth: true
    spacing: 10

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

    InspectorSectionHeader { text: "Layout" }

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

            Label { text: "X"; color: "#555b62"; font.pixelSize: 14; Layout.preferredWidth: 40 }
            TextField {
                id: xField
                Layout.fillWidth: true
                text: Math.round(canvasViewModel.selectedWidgetX)
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                onEditingFinished: root.commitGeometry()
            }

            Label { text: "Y"; color: "#555b62"; font.pixelSize: 14; Layout.preferredWidth: 40 }
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

            Label { text: "W"; color: "#555b62"; font.pixelSize: 14; Layout.preferredWidth: 40 }
            TextField {
                id: widthField
                Layout.fillWidth: true
                text: Math.round(canvasViewModel.selectedWidgetWidth)
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                onEditingFinished: root.commitGeometry()
            }

            Label { text: "H"; color: "#555b62"; font.pixelSize: 14; Layout.preferredWidth: 40 }
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

            Button { text: "L"; Layout.fillWidth: true; onClicked: canvasViewModel.alignSelected("left", canvasViewModel.canvasWidth, canvasViewModel.canvasHeight) }
            Button { text: "C"; Layout.fillWidth: true; onClicked: canvasViewModel.alignSelected("hcenter", canvasViewModel.canvasWidth, canvasViewModel.canvasHeight) }
            Button { text: "R"; Layout.fillWidth: true; onClicked: canvasViewModel.alignSelected("right", canvasViewModel.canvasWidth, canvasViewModel.canvasHeight) }
            Button { text: "T"; Layout.fillWidth: true; onClicked: canvasViewModel.alignSelected("top", canvasViewModel.canvasWidth, canvasViewModel.canvasHeight) }
            Button { text: "M"; Layout.fillWidth: true; onClicked: canvasViewModel.alignSelected("vcenter", canvasViewModel.canvasWidth, canvasViewModel.canvasHeight) }
            Button { text: "B"; Layout.fillWidth: true; onClicked: canvasViewModel.alignSelected("bottom", canvasViewModel.canvasWidth, canvasViewModel.canvasHeight) }
        }
    }
}
