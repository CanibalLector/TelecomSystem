import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: controlView

    Rectangle {
        anchors.fill: parent

        gradient: Gradient {
            GradientStop { position: 0.0; color: "white"}
            GradientStop { position: 1.0; color: colorBgControlView }
        }

        Text {
            id: label
            text: "Control"
            font.italic: true
            font.bold: true
            font.pixelSize: 30
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 15
        }

        ColumnLayout {
            anchors.top: label.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 20
            spacing: 20

            // Блок управления сокетом сервера
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 80
                color: "transparent"
                radius: 8

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 20

                    // Индикатор статуса
                    Item {
                        id: statusIndicator
                        Layout.preferredWidth: 16
                        Layout.preferredHeight: 16

                        // Внешний пульсирующий эффект
                        Rectangle {
                            anchors.centerIn: parent
                            width: 24
                            height: 24
                            radius: 12
                            color: serverController.isServerRunning ? "#2ecc71" : "transparent"
                            opacity: 0.0

                            SequentialAnimation on opacity {
                                loops: Animation.Infinite
                                running: serverController.isServerRunning
                                NumberAnimation { from: 0.5; to: 0.0; duration: 1200; easing.type: Easing.OutQuad }
                                PauseAnimation { duration: 300 }
                            }

                            SequentialAnimation on scale {
                                loops: Animation.Infinite
                                running: serverController.isServerRunning
                                NumberAnimation { from: 0.8; to: 1.8; duration: 1200; easing.type: Easing.OutQuad }
                                PauseAnimation { duration: 300 }
                            }
                        }

                        // Центральная  точка
                        Rectangle {
                            anchors.fill: parent
                            radius: 8
                            color: serverController.isServerRunning ? colorSaveActivGreen : colorAlarmStopRed
                            antialiasing: true

                            // Пульсирующий эффект
                            SequentialAnimation on scale {
                                loops: Animation.Infinite
                                running: serverController.isServerRunning
                                NumberAnimation { to: 1.1; duration: 800; easing.type: Easing.InOutSine }
                                NumberAnimation { to: 1.0; duration: 800; easing.type: Easing.InOutSine }
                            }
                        }
                    }

                    Label {
                        text: "Порт Сервера:"
                        font.italic: true
                        font.bold: true
                        color: "black"
                        font.pixelSize: 16
                    }

                    TextField {
                        id: portInput
                        text: "12345"
                        font.italic: true
                        font.bold: true
                        placeholderText: "12345"
                        selectByMouse: true
                        horizontalAlignment: Text.AlignHCenter
                        validator: IntValidator { bottom: 1024; top: 65535 }
                        enabled: !serverController.isServerRunning

                        background: Rectangle {
                            color: colorBgTextEdit
                            border.color: portInput.enabled ? colorSaveActivGreen : "transparent"
                            border.width: 1
                            radius: 4
                        }
                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 25
                        color: "black"
                    }

                    Button {
                        text: serverController.isServerRunning ? "Stop Server" : "Start Server"
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 30

                        background: Rectangle {
                            color: serverController.isServerRunning ? colorAlarmStopRed : colorSaveActivGreen
                            radius: 4
                        }

                        contentItem: Label {
                            text: parent.text
                            color: "black"
                            font.bold: true
                            font.pixelSize: 14
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {
                            if (portInput.text !== "") {
                                serverController.toggleServer(parseInt(portInput.text))
                            }
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                    }
                }
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }
}
