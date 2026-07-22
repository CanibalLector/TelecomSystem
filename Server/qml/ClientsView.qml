import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Telecom.Models 1.0 // Импортируем, чтобы QML знал про C++ роли модели

Rectangle {

    id: background
    gradient: Gradient {
        GradientStop { position: 0.0; color: "white"}
        GradientStop { position: 1.0; color: root.colorBgClientsView }
    }

    Text {
        id: label
        text: "Clients"
        font.italic: true
        font.bold: true
        font.pixelSize: 30
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 5
    }

    // Таблица клиентов
    ListView {
        id: clientsListView
        anchors.top: label.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 10
        clip: true
        spacing: 5

        // Привязываем модель напрямую к C++ контроллеру из контекста
        model: serverController.clientsModel

        // Описание строки таблицы
        delegate: Rectangle {
            id: rowDelegate
            width: clientsListView.width - 20
            height: 36
            color: model.clientActivityState !== ClientsListModel.Disconnected ? (index % 2 === 0 ? "#f9f9f9" : "#ffffff") : "#f4f4f4"
            border.color: "#e0e0e0"
            radius: 6

            // Локальные флаги ожидания ответа
            property bool waitingForConnection: false
            property bool waitingForSending: false
            property int currentServerState: model.clientActivityState

            onCurrentServerStateChanged: {
                if (waitingForConnection) {
                    waitingForConnection = false
                    connTimeoutTimer.stop()
                }
                if (waitingForSending) {
                    waitingForSending = false
                    sendTimeoutTimer.stop()
                }
            }

            // Таймеры ожидания
            Timer {
                id: connTimeoutTimer
                interval: 2000
                repeat: false
                onTriggered: rowDelegate.waitingForConnection = false
            }

            Timer {
                id: sendTimeoutTimer
                interval: 2000
                repeat: false
                onTriggered: rowDelegate.waitingForSending = false
            }

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 15
                anchors.rightMargin: 15
                spacing: 12

                // Индикатор активности
                Rectangle {
                    width: 14; height: 14; radius: 7
                    Layout.alignment: Qt.AlignVCenter
                    color: {
                        switch (model.clientActivityState) {
                            case ClientsListModel.Connected: return "#4CAF50" // Connected
                            case ClientsListModel.Sending: return "#2196F3" // Sending
                            default: return "#9e9e9e" // Disconnected
                        }
                    }
                    border.color: "white"
                    border.width: 1
                    Behavior on color { ColorAnimation { duration: 200 } }
                }

                // Чекбокс
                CheckBox {
                    checked: model.clientChecked
                    onCheckedChanged: model.clientChecked = checked
                    scale: 0.85
                }

                // Текст описания
                Text {
                    text: model.clientDescription
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                    font.pixelSize: 13
                    color: model.clientActivityState !== ClientsListModel.Disconnected ? "#212121" : "#888888"
                }

                // Кнопка подключения клиента
                Button {
                    id: connButton
                    implicitWidth: 100
                    implicitHeight: 26
                    enabled: !rowDelegate.waitingForConnection

                    // Только отображение текста, никаких изменений переменных внутри!
                    text: rowDelegate.waitingForConnection ? "Ждите..." :
                          (model.clientActivityState !== ClientsListModel.Disconnected ? "Отключить" : "Подключить")

                    contentItem: Text {
                        text: connButton.text
                        font.pixelSize: 11; font.bold: true
                        horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                        color: rowDelegate.waitingForConnection ? "#757575" :
                               (model.clientActivityState !== ClientsListModel.Disconnected ? "#d32f2f" : "#388e3c")
                    }

                    background: Rectangle {
                        radius: height / 2; border.width: 1
                        color: rowDelegate.waitingForConnection ? "#e0e0e0" :
                               (model.clientActivityState !== ClientsListModel.Disconnected ? "#ffebee" : "#e8f5e9")
                        border.color: rowDelegate.waitingForConnection ? "#bdbdbd" :
                                     (model.clientActivityState !== ClientsListModel.Disconnected ? "#ffcdd2" : "#c8e6c9")
                    }

                    onClicked: {
                        rowDelegate.waitingForConnection = true
                        connTimeoutTimer.start()
                        if (!model.clientActivityState){
                            serverController.setClientConnection(model.clientId, true)
                        } else {
                            serverController.setClientConnection(model.clientId, false)
                        }
                    }
                }

                // Кнопка включения передачи данных
                Button {
                    id: sendButton
                    implicitWidth: 80
                    implicitHeight: 26
                    enabled: (model.clientActivityState !== ClientsListModel.Disconnected) && !rowDelegate.waitingForSending

                    // Только отображение текста
                    text: rowDelegate.waitingForSending ? "Ждите..." :
                          (model.clientActivityState === ClientsListModel.Sending ? "Стоп" : "Старт")

                    contentItem: Text {
                        text: sendButton.text
                        font.pixelSize: 11; font.bold: true
                        horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                        color: !sendButton.enabled ? "#9e9e9e" :
                               rowDelegate.waitingForSending ? "#757575" :
                               (model.clientActivityState === ClientsListModel.Sending ? "#1976D2" : "#f57c00")
                    }

                    background: Rectangle {
                        radius: height / 2; border.width: 1
                        color: !sendButton.enabled ? "#e0e0e0" :
                               rowDelegate.waitingForSending ? "#eee0e0" :
                               (model.clientActivityState === ClientsListModel.Sending ? "#e1f5fe" : "#fff3e0")
                        border.color: !sendButton.enabled ? "#bdbdbd" :
                                     rowDelegate.waitingForSending ? "#bdbdbd" :
                                     (model.clientActivityState === ClientsListModel.Sending ? "#b3e5fc" : "#ffe0b2")
                    }

                    onClicked: {
                        rowDelegate.waitingForSending = true
                        sendTimeoutTimer.start()
                        if (model.clientActivityState === ClientsListModel.Sending){
                            serverController.setClientDataSending(model.clientId, false)
                        } else if (model.clientActivityState === ClientsListModel.Connected){
                            serverController.setClientDataSending(model.clientId, true)
                        }
                    }
                }
            }
        }
    }
}
