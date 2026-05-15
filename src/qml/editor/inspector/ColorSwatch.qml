import QtQuick
import QtQuick.Layouts

Rectangle {
    required property string swatchColor

    Layout.preferredWidth: 32
    Layout.preferredHeight: 26
    color: swatchColor
    border.color: "#b8bcc1"
}
