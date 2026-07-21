import QtQuick

Rectangle {

    id: background
    gradient: Gradient {
        GradientStop { position: 0.0; color: "white"}
        GradientStop { position: 1.0; color: "#45218BF1" }
    }

    Text {
        id: label
        text: "Control"
        font.italic: true
        font.bold: true
        font.pixelSize: 30
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 5
    }
}
