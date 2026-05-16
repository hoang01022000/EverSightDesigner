import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    required property string swatchColor
    property bool popupEnabled: true

    signal colorPicked(string color)

    function displayColor(value) {
        var text = String(value || "").trim()
        if (/^#[0-9a-fA-F]{3}$/.test(text) || /^#[0-9a-fA-F]{6}$/.test(text))
            return text
        if (text === "transparent")
            return text
        return "#ffffff"
    }

    Layout.preferredWidth: 32
    Layout.preferredHeight: 26
    color: displayColor(swatchColor)
    border.color: "#b8bcc1"

    MouseArea {
        anchors.fill: parent
        enabled: root.popupEnabled
        cursorShape: Qt.PointingHandCursor
        onClicked: colorPicker.open()
    }

    ColorPickerPopup {
        id: colorPicker
        parent: Overlay.overlay
        x: Math.max(12, parent.width - width - 12)
        y: 58
        selectedColor: root.swatchColor
        onColorAccepted: function(color) {
            root.colorPicked(color)
        }
    }
}
