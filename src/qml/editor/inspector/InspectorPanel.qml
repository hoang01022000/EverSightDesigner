import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    color: "#f4f5f6"
    implicitWidth: 318

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
                text: canvasViewModel.inspectorMode === "widget"
                      ? "BaseSettings"
                      : "LayoutSplitStyle"
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
            id: inspectorScroll
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            contentWidth: availableWidth
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded

            ColumnLayout {
                width: inspectorScroll.availableWidth
                spacing: 10

                LayoutSplitStyleSection {
                    visible: canvasViewModel.inspectorMode === "container"
                }

                BaseSettingsSection {
                    visible: canvasViewModel.inspectorMode === "widget"
                }
            }
        }
    }
}
