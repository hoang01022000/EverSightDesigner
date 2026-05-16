import QtQuick
import QtQuick.Controls
import QtQuick.Window
import EverSightDesigner

ApplicationWindow {
    visible: true
    visibility: Window.Maximized
    minimumWidth: 1100
    minimumHeight: 720
    flags: Qt.FramelessWindowHint | Qt.Window
    title: "Run-time Interface Design"

    DesignerView {
        anchors.fill: parent
    }
}
