import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

RoundButton {
    id: btn
    property alias iconSrc: icon.source
    property bool activation: false
    implicitHeight: (Screen.width/Screen.pixelDensity) > 100 ? Screen.pixelDensity * 15 : Screen.pixelDensity * 8
    implicitWidth: implicitHeight
    height: implicitHeight
    width: implicitHeight
    icon.color: "transparent"

    onActivationChanged:{
        if(activation)
            upScale.start();
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
        id: bgRect
        implicitWidth: 40
        implicitHeight: 40
        radius: parent.radius
        opacity: enabled ? 1 : 0.3
        visible: !parent.flat || parent.down || parent.checked || parent.highlighted

        color: "transparent"

        border.color: parent.palette.highlight
        border.width: parent.visualFocus ? 2 : 0

        Shape {
            anchors.fill: parent
            layer.enabled: true
            layer.samples: 4

            ShapePath {
                strokeColor: "transparent"

                // Рисуем окружность по границам кнопки
                startX: 0
                startY: bgRect.height / 2
                PathAngleArc {
                    centerX: bgRect.width / 2
                    centerY: bgRect.height / 2
                    radiusX: bgRect.width / 2
                    radiusY: bgRect.height / 2
                    startAngle: 0
                    sweepAngle: 360
                }

                // Встроенный радиальный градиент из модуля Shapes
                fillGradient: RadialGradient {
                    centerX: bgRect.width / 2
                    centerY: bgRect.height / 2
                    centerRadius: bgRect.width / 2
                    focalX: centerX
                    focalY: centerY

                    stops: [
                        GradientStop { position: 0.0; color: "#AABBBBBB" },
                        GradientStop { position: 0.7; color: "#44BBBBBB" },
                        GradientStop { position: 1.0; color: "#15BBBBBB" }
                    ]
                }

            }
        }
    }

    contentItem: Text {
        text: btn.text
        horizontalAlignment: Text.AlignHCenter
        clip: false
        anchors.top: icon.bottom
        anchors.topMargin: -btn.height*0.1
        font.pixelSize: btn.height*0.2
    }

    Image {
        id: icon
        source: icon_src
        height: btn.height*0.7
        width: height
        anchors.top: btn.top
        anchors.horizontalCenter: btn.horizontalCenter
    }
}
