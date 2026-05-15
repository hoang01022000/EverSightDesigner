import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    Layout.fillWidth: true
    spacing: 10

    InspectorSectionHeader { text: "Identity" }

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
                Layout.fillWidth: true
                text: canvasViewModel.selectedWidgetTitle
                selectByMouse: true
                onEditingFinished: canvasViewModel.updateSelectedTitle(text)
            }
        }
    }
}
