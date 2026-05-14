import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    anchors.fill: parent

    ColumnLayout {
        anchors.fill: parent

        // Tab bar
        RowLayout {
            width: parent.width
            height: 36
            spacing: 6

            Repeater {
                model: canvasViewModel.tabCount
                delegate: Button {
                    text: "Tab " + (index + 1)
                    onClicked: canvasViewModel.setActiveTab(index)
                }
            }

            Button { text: "+"; onClicked: canvasViewModel.addTab() }
        }

        // Main area with optional fixed bars
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Top fixed
            Rectangle {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: canvasViewModel.topFixedVisible ? 48 : 0
                color: "#f0f0f0"
                visible: canvasViewModel.topFixedVisible
                z: 10
                Label { anchors.centerIn: parent; text: "Top Fixed" }
            }

            // Left fixed
            Rectangle {
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: canvasViewModel.leftFixedVisible ? 160 : 0
                color: "#f6f6f6"
                visible: canvasViewModel.leftFixedVisible
                z: 10
                Label { anchors.centerIn: parent; text: "Left Fixed" }
            }

            // Canvas center area
            Rectangle {
                anchors.left: canvasViewModel.leftFixedVisible ? parent.left : parent.left
                anchors.right: canvasViewModel.rightFixedVisible ? parent.right : parent.right
                anchors.top: canvasViewModel.topFixedVisible ? parent.top : parent.top
                anchors.bottom: canvasViewModel.bottomFixedVisible ? parent.bottom : parent.bottom
                color: "#ffffff"
                border.color: "#dddddd"

                // simple grid when showGrid true
                Canvas {
                    anchors.fill: parent
                    onPaint: {
                        var ctx = getContext("2d")
                        ctx.reset()
                        if (!canvasViewModel.tabCount) return
                        ctx.fillStyle = parent.color
                        ctx.fillRect(0,0,width,height)
                        // basic grid lines placeholder
                        ctx.strokeStyle = "#eeeeee"
                        ctx.lineWidth = 1
                        var cols = 4
                        var rows = 3
                        for (var i=1;i<cols;i++) {
                            var x = width * i / cols
                            ctx.beginPath(); ctx.moveTo(x,0); ctx.lineTo(x,height); ctx.stroke()
                        }
                        for (var j=1;j<rows;j++) {
                            var y = height * j / rows
                            ctx.beginPath(); ctx.moveTo(0,y); ctx.lineTo(width,y); ctx.stroke()
                        }
                    }
                }
            }

            // Right fixed
            Rectangle {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: canvasViewModel.rightFixedVisible ? 160 : 0
                color: "#f6f6f6"
                visible: canvasViewModel.rightFixedVisible
                z: 10
                Label { anchors.centerIn: parent; text: "Right Fixed" }
            }

            // Bottom fixed
            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height: canvasViewModel.bottomFixedVisible ? 48 : 0
                color: "#f0f0f0"
                visible: canvasViewModel.bottomFixedVisible
                z: 10
                Label { anchors.centerIn: parent; text: "Bottom Fixed" }
            }
        }
    }
}
