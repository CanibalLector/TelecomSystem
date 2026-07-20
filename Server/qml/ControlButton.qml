import QtQuick
import QtQuick.Controls
import QtQuick.Shapes 1.5

// Кнопка перключения экранов с анимацией
RoundButton {

    id: btn
    property alias icon_src: icon.source
    property bool activation: false
    implicitHeight: (Screen.width/Screen.pixelDensity) > 100 ? Screen.pixelDensity * 15 : Screen.pixelDensity * 8
    implicitWidth: implicitHeight
    height: implicitHeight
    width: implicitHeight
    icon.color: "transparent"

    onActivationChanged:{
        if(activation)
            up_scale.start();
        else
            downScale.start();
    }

    NumberAnimation on scale {
        id: upScale
        to: 1.4
        duration: 100
    }

    NumberAnimation on scale {
        id: downScale
        to: 1
        duration: 100
    }

    background: Rectangle {
        implicitWidth: 40
        implicitHeight: 40
        radius: parent.radius
        opacity: enabled ? 1 : 0.3
        visible: !parent.flat || parent.down || parent.checked || parent.highlighted
        color: Color.blend(parent.checked || parent.highlighted ? parent.palette.dark : parent.palette.button,
                           parent.palette.mid, parent.down ? 0.5 : 0.0)
        border.color: parent.palette.highlight
        border.width: parent.visualFocus ? 2 : 0

        gradient: RadialGradient {
            GradientStop { position: 0.0; color: "#11BBBBBB" }
            GradientStop { position: 1.0; color: "#AABBBBBB" }
        }
    }

    contentItem:
        Text {
        text: btn.text
        horizontalAlignment: Text.AlignHCenter
        clip: false
        anchors.top: icon.bottom
        anchors.topMargin: -btn.height*0.1
        font.pixelSize: btn.height*0.2
    }

    Image{
        id: icon
        source: icon_src
        height: btn.height*0.7
        width: height
        anchors.top: btn.top
        anchors.horizontalCenter: btn.horizontalCenter
    }


}
