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
        id: header
        anchors.top: parent.top
        height: 90
        width:  parent.width
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#AA8FF4EF" }
            GradientStop { position: 1.0; color: "#FFFFFFFF" }
        }

        Flow {
            id: flow
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            ControlButton {
                iconSrc: "../images/list-96.png"
                text: "clients"
                onClicked: screenList.currentIndex = 0
            }

            ControlButton {
                iconSrc: "../images/console-96.png"
                text: "events"
                onClicked: screenList.currentIndex = 1
            }

            ControlButton {
                iconSrc: "../images/area-chart-96.png"
                text: "data"
                onClicked: screenList.currentIndex = 2
            }

            ControlButton {
                text: "control"
                iconSrc: "../images/structure-96.png"
                onClicked: screenList.currentIndex = 3
            }

            ControlButton {
                text: "settings"
                iconSrc: "../images/settings-96.png"
                onClicked: screenList.currentIndex = 4
            }

        }
    }

    ObjectModel {
        id: itemModel
        ClientsView{width: screenList.width; height: screenList.height;}
        EventsView{width: screenList.width; height: screenList.height;}
        DataView{width: screenList.width; height: screenList.height;}
        ControlView{width: screenList.width; height: screenList.height;}
        SettingsView{width: screenList.width; height: screenList.height;}
    }

    ListView {
        id: screenList
        focus: true
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        highlightMoveDuration: 250
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.StopAtBounds
        currentIndex: 0

        property int lastIndex: 1

        onCurrentIndexChanged: {
            flow.children[screenList.lastIndex].activation =  false
            flow.children[screenList.currentIndex].activation = true
            screenList.lastIndex = screenList.currentIndex
        }

        model: itemModel
    }
}
