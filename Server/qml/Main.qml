import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    width:  Screen.devicePixelRatio > 2 ? Screen.width : 600
    height: Screen.devicePixelRatio > 2 ? Screen.height : 1024
    visible: true
    title: qsTr("Telecom control system")

    // Фоновая подложка всего окна
    background:  Image {
        anchors.fill: parent
        source: "../images/bg.png"
        fillMode: Image.PreserveAspectCrop
    }

    // Заглавие с кноапками выбора экранов
    Rectangle {
        anchors.top: parent.top
        height: 90
        width:  parent.width
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#AA8FF4EF" }
            GradientStop { position: 1.0; color: "#FFFFFFFF" }
        }

        Flow{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            ControlButton {
                icon_src: "../images/list-96.png"
                text: "clients"
                onClicked: screen_list.currentIndex = 0
            }

            ControlButton {
                icon_src: "../images/console-96.png"
                text: "events"
                onClicked: screen_list.currentIndex = 1
            }

            ControlButton {
                icon_src: "../images/area-chart-96.png"
                text: "data"
                onClicked: screen_list.currentIndex = 2
            }

            ControlButton {
                text: "control"
                icon_src: "../images/structure-96.png"
                onClicked: screen_list.currentIndex = 3
            }

            ControlButton {
                text: "settings"
                icon_src: "../images/settings-96.png"
                onClicked: screen_list.currentIndex = 4
            }

        }
    }
}
