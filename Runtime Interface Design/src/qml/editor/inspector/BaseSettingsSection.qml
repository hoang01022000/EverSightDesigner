import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    Layout.fillWidth: true
    spacing: 10

    InspectorSectionHeader { text: "BaseSettings" }

    IdentitySection {}
    InspectorDivider {}
    DataSourceSection {}
    InspectorDivider { visible: canvasViewModel.selectedWidgetHasDataSource }
    LayoutSection {}
    InspectorDivider {}
    AppearanceSection {}
    DynamicPropertySection {}

    Button {
        Layout.fillWidth: true
        Layout.leftMargin: 16
        Layout.rightMargin: 16
        Layout.topMargin: 16
        text: "Delete Control"
        onClicked: canvasViewModel.removeSelectedWidget()
    }
}
