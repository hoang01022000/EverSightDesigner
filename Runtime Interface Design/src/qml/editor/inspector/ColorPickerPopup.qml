import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: root

    property string selectedColor: "#ffffff"
    property string previewColor: "#ffffff"
    property bool syncing: false
    property int editorMode: 0

    signal colorAccepted(string color)

    modal: true
    focus: true
    padding: 12
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    function normalizeHex(value) {
        var text = String(value || "").trim()
        if (text.charAt(0) !== "#")
            text = "#" + text

        if (/^#[0-9a-fA-F]{3}$/.test(text)) {
            return "#" + text.charAt(1) + text.charAt(1)
                    + text.charAt(2) + text.charAt(2)
                    + text.charAt(3) + text.charAt(3)
        }

        if (/^#[0-9a-fA-F]{6}$/.test(text))
            return text.toLowerCase()

        return "#ffffff"
    }

    function toHex(value) {
        var hex = Math.max(0, Math.min(255, Math.round(value))).toString(16)
        return hex.length === 1 ? "0" + hex : hex
    }

    function hsvToHex(h, s, v) {
        s = s / 100
        v = v / 100

        var c = v * s
        var x = c * (1 - Math.abs((h / 60) % 2 - 1))
        var m = v - c
        var r = 0
        var g = 0
        var b = 0

        if (h < 60) {
            r = c; g = x
        } else if (h < 120) {
            r = x; g = c
        } else if (h < 180) {
            g = c; b = x
        } else if (h < 240) {
            g = x; b = c
        } else if (h < 300) {
            r = x; b = c
        } else {
            r = c; b = x
        }

        return "#" + toHex((r + m) * 255) + toHex((g + m) * 255) + toHex((b + m) * 255)
    }

    function rgbToHex(r, g, b) {
        return "#" + toHex(r) + toHex(g) + toHex(b)
    }

    function hexToRgb(hex) {
        var colorText = normalizeHex(hex)
        return {
            r: parseInt(colorText.substr(1, 2), 16),
            g: parseInt(colorText.substr(3, 2), 16),
            b: parseInt(colorText.substr(5, 2), 16)
        }
    }

    function hexToHsv(hex) {
        var colorText = normalizeHex(hex)
        var r = parseInt(colorText.substr(1, 2), 16) / 255
        var g = parseInt(colorText.substr(3, 2), 16) / 255
        var b = parseInt(colorText.substr(5, 2), 16) / 255
        var maxValue = Math.max(r, g, b)
        var minValue = Math.min(r, g, b)
        var delta = maxValue - minValue
        var h = 0

        if (delta !== 0) {
            if (maxValue === r)
                h = 60 * (((g - b) / delta) % 6)
            else if (maxValue === g)
                h = 60 * ((b - r) / delta + 2)
            else
                h = 60 * ((r - g) / delta + 4)
        }

        if (h < 0)
            h += 360

        return {
            h: h,
            s: maxValue === 0 ? 0 : delta / maxValue * 100,
            v: maxValue * 100
        }
    }

    function setColor(value) {
        var colorText = normalizeHex(value)
        var hsv = hexToHsv(colorText)
        var rgb = hexToRgb(colorText)

        syncing = true
        hueSlider.value = hsv.h
        hueBox.value = Math.round(hsv.h)
        saturationSlider.value = hsv.s
        saturationBox.value = Math.round(hsv.s)
        valueSlider.value = hsv.v
        valueBox.value = Math.round(hsv.v)
        redSlider.value = rgb.r
        redBox.value = rgb.r
        greenSlider.value = rgb.g
        greenBox.value = rgb.g
        blueSlider.value = rgb.b
        blueBox.value = rgb.b
        hexField.text = colorText
        previewColor = colorText
        syncing = false
    }

    function updateFromHsv() {
        if (syncing)
            return

        var colorText = hsvToHex(hueSlider.value, saturationSlider.value, valueSlider.value)
        setColor(colorText)
    }

    function updateFromRgb() {
        if (syncing)
            return

        var colorText = rgbToHex(redSlider.value, greenSlider.value, blueSlider.value)
        setColor(colorText)
    }

    onOpened: setColor(selectedColor)

    background: Rectangle {
        color: "#ffffff"
        radius: 6
        border.color: "#c9cdd2"
    }

    contentItem: ColumnLayout {
        width: 268
        spacing: 10

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 38
            radius: 4
            color: root.previewColor
            border.color: "#b8bcc1"
        }

        ComboBox {
            Layout.fillWidth: true
            model: [ "HSV", "RGB" ]
            currentIndex: root.editorMode
            onActivated: root.editorMode = currentIndex
        }

        GridLayout {
            Layout.fillWidth: true
            columns: 2
            columnSpacing: 10
            rowSpacing: 8
            visible: root.editorMode === 0

            Label { text: "Hue"; color: "#555b62"; font.pixelSize: 13 }
            RowLayout {
                Layout.fillWidth: true
                Slider {
                    id: hueSlider
                    Layout.fillWidth: true
                    from: 0
                    to: 359
                    onValueChanged: {
                        if (!root.syncing)
                            hueBox.value = Math.round(value)
                        root.updateFromHsv()
                    }
                }
                SpinBox {
                    id: hueBox
                    from: 0
                    to: 359
                    editable: true
                    Layout.preferredWidth: 78
                    onValueModified: {
                        hueSlider.value = value
                        root.updateFromHsv()
                    }
                }
            }

            Label { text: "Saturation"; color: "#555b62"; font.pixelSize: 13 }
            RowLayout {
                Layout.fillWidth: true
                Slider {
                    id: saturationSlider
                    Layout.fillWidth: true
                    from: 0
                    to: 100
                    onValueChanged: {
                        if (!root.syncing)
                            saturationBox.value = Math.round(value)
                        root.updateFromHsv()
                    }
                }
                SpinBox {
                    id: saturationBox
                    from: 0
                    to: 100
                    editable: true
                    Layout.preferredWidth: 78
                    onValueModified: {
                        saturationSlider.value = value
                        root.updateFromHsv()
                    }
                }
            }

            Label { text: "Value"; color: "#555b62"; font.pixelSize: 13 }
            RowLayout {
                Layout.fillWidth: true
                Slider {
                    id: valueSlider
                    Layout.fillWidth: true
                    from: 0
                    to: 100
                    onValueChanged: {
                        if (!root.syncing)
                            valueBox.value = Math.round(value)
                        root.updateFromHsv()
                    }
                }
                SpinBox {
                    id: valueBox
                    from: 0
                    to: 100
                    editable: true
                    Layout.preferredWidth: 78
                    onValueModified: {
                        valueSlider.value = value
                        root.updateFromHsv()
                    }
                }
            }
        }

        GridLayout {
            Layout.fillWidth: true
            columns: 2
            columnSpacing: 10
            rowSpacing: 8
            visible: root.editorMode === 1

            Label { text: "Red"; color: "#555b62"; font.pixelSize: 13 }
            RowLayout {
                Layout.fillWidth: true
                Slider {
                    id: redSlider
                    Layout.fillWidth: true
                    from: 0
                    to: 255
                    onValueChanged: {
                        if (!root.syncing)
                            redBox.value = Math.round(value)
                        root.updateFromRgb()
                    }
                }
                SpinBox {
                    id: redBox
                    from: 0
                    to: 255
                    editable: true
                    Layout.preferredWidth: 78
                    onValueModified: {
                        redSlider.value = value
                        root.updateFromRgb()
                    }
                }
            }

            Label { text: "Green"; color: "#555b62"; font.pixelSize: 13 }
            RowLayout {
                Layout.fillWidth: true
                Slider {
                    id: greenSlider
                    Layout.fillWidth: true
                    from: 0
                    to: 255
                    onValueChanged: {
                        if (!root.syncing)
                            greenBox.value = Math.round(value)
                        root.updateFromRgb()
                    }
                }
                SpinBox {
                    id: greenBox
                    from: 0
                    to: 255
                    editable: true
                    Layout.preferredWidth: 78
                    onValueModified: {
                        greenSlider.value = value
                        root.updateFromRgb()
                    }
                }
            }

            Label { text: "Blue"; color: "#555b62"; font.pixelSize: 13 }
            RowLayout {
                Layout.fillWidth: true
                Slider {
                    id: blueSlider
                    Layout.fillWidth: true
                    from: 0
                    to: 255
                    onValueChanged: {
                        if (!root.syncing)
                            blueBox.value = Math.round(value)
                        root.updateFromRgb()
                    }
                }
                SpinBox {
                    id: blueBox
                    from: 0
                    to: 255
                    editable: true
                    Layout.preferredWidth: 78
                    onValueModified: {
                        blueSlider.value = value
                        root.updateFromRgb()
                    }
                }
            }
        }

        GridLayout {
            Layout.fillWidth: true
            columns: 2
            columnSpacing: 10
            rowSpacing: 8

            Label { text: "Hex"; color: "#555b62"; font.pixelSize: 13 }
            TextField {
                id: hexField
                Layout.fillWidth: true
                selectByMouse: true
                validator: RegularExpressionValidator {
                    regularExpression: /^#?[0-9a-fA-F]{0,6}$/
                }
                onEditingFinished: root.setColor(text)
            }
        }

        RowLayout {
            Layout.fillWidth: true

            Item { Layout.fillWidth: true }

            Button {
                text: "Cancel"
                onClicked: root.close()
            }

            Button {
                text: "Apply"
                onClicked: {
                    root.colorAccepted(root.previewColor)
                    root.close()
                }
            }
        }
    }
}
