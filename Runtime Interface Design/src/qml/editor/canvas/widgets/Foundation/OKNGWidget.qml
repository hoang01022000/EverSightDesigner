import QtQuick

Rectangle {
    property string bgColor: "#1d3e24"
    property string fontColor: "#ffffff"
    property string okContent: "OK"
    property string ngContent: "NG"
    property bool isOK: true

    color: isOK ? "#1d7a3a" : "#c0392b"
    radius: 4

    Text {
        anchors.centerIn: parent
        text: isOK ? okContent : ngContent
        color: "#ffffff"
        font.pixelSize: 22
        font.bold: true
    }
}
