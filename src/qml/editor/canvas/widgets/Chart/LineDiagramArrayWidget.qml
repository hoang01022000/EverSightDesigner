import QtQuick

Rectangle {
    property string bgColor: "#2b2f33"
    property string borderColor: "#4f5358"
    property string fontColor: "#e8ecf0"
    property string labelText: "Line Diagram"

    color: bgColor
    border.color: borderColor
    border.width: 1
    radius: 3

    Canvas {
        anchors.fill: parent
        anchors.margins: 12
        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)
            ctx.strokeStyle = "#60a5fa"
            ctx.lineWidth = 2
            ctx.beginPath()
            for (var i = 0; i < 8; ++i) {
                var x = i * width / 7
                var y = height * (0.65 - 0.28 * Math.sin(i * 0.9))
                if (i === 0) ctx.moveTo(x, y)
                else ctx.lineTo(x, y)
            }
            ctx.stroke()
        }
    }
}
