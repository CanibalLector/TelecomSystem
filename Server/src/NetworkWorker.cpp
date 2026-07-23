#include "NetworkWorker.hpp"
#include "NetworkProtocol.hpp"
#include <QJsonDocument>
#include <QDateTime>

namespace Telecom::Server {

    NetworkWorker::NetworkWorker(QObject* parent) : QObject(parent) {

        // Привязываем публичные сигналы управления  клиентами к приватным слотам
        connect(this, &NetworkWorker::requestDisconnectClient, this, &NetworkWorker::processDisconnectClient);
        connect(this, &NetworkWorker::requestConnectClient, this, &NetworkWorker::processConnectClient);

    }

    NetworkWorker::~NetworkWorker() {
        stopServer();
    }

    // Запуск прослушивания порта
    void NetworkWorker::startServer(quint16 port) {
        if (m_tcpServer) {
            return;
        }

        m_tcpServer = new QTcpServer(this);

        // Соединяем синнал нового клиента со слотом со слотом соединения
        connect(m_tcpServer, &QTcpServer::newConnection, this, &NetworkWorker::onNewConnection);

        // Слушаем порт
        if (m_tcpServer->listen(QHostAddress::Any, port)) {
        } else {
            delete m_tcpServer;
            m_tcpServer = nullptr;
        }
    }

    // Остановка сервера
    void NetworkWorker::stopServer() {
        if (!m_tcpServer) return;

        // Закрываем сервер
        m_tcpServer->close();
        delete m_tcpServer;
        m_tcpServer = nullptr;

        // Удаляем клиентов
        qDeleteAll(m_clientsMap.values());
        qDeleteAll(m_pendingClientsMap.values());
        m_clientsMap.clear();
        m_pendingClientsMap.clear();
    }

    // Подключение ожидающего клиента в своём потоке
    void NetworkWorker::processConnectClient(const QString& id){

        // Ищем клиента в списке ожидающих
        if (m_pendingClientsMap.contains(id)) {
            ClientData* client = m_pendingClientsMap[id];
            QTcpSocket* socket = client->socket();

            if (!socket) return;

            // Проверяем, не переведён ли он уже в список подключенных
            if (m_clientsMap.contains(socket)) {
                // Клиент уже подключён, подтверждаем статус
                emit clientConnected(client->id(), client->ipAddress(), client->port());
                return;
            }

            // Переводим из ожидающих в подкючённые
            m_pendingClientsMap.remove(id);   // Удаляем из карты ожидания
            m_clientsMap.insert(socket, client); // Вставляем в карту подключённых по сокету

            // Настраиваем подписки на события сокета
            connect(socket, &QTcpSocket::readyRead, this, &NetworkWorker::onReadyRead);
            connect(socket, &QTcpSocket::disconnected, this, &NetworkWorker::onDisconnected);

            // Оповещаем контроллер контроллер о подключении
            emit clientConnected(client->id(), client->ipAddress(), client->port());

        } else {
            // Не нашли клиента, отпрвляем сигнал об удалении в GUI
            emit clientDisconnected(id);
        }
    }


    // Отключение клиента в своём потоке
    void NetworkWorker::processDisconnectClient(const QString& id) {
        QTcpSocket* targetSocket = nullptr;
        ClientData* targetClient = nullptr;

        // Ищем клиента среди активных по его ID
        for (auto it = m_clientsMap.begin(); it != m_clientsMap.end(); ++it) {
            if (it.value()->id() == id) {
                targetSocket = it.key();
                targetClient = it.value();
                break;
            }
        }

        if (targetSocket && targetClient) {
            // Закрытие сокета автоматически вызовет слот onDisconnected(),
            // где клиент очистится из памяти и подаст сигнал в GUI
            targetSocket->disconnectFromHost();
        } else if (m_pendingClientsMap.contains(id)) {
            // Если клиент висел в ожидающих и его отклонили
            ClientData* pendingClient = m_pendingClientsMap.take(id);
            if (pendingClient->socket()) {
                pendingClient->socket()->disconnectFromHost();
                pendingClient->socket()->deleteLater();
            }
            pendingClient->deleteLater();
            emit clientDisconnected(id);
        }
    }


    // Обработка нового соединения
    void NetworkWorker::onNewConnection() {
        if (!m_tcpServer) return;

        // Извлекаем сокет подключившегося клиента из очереди сервера
        while (m_tcpServer->hasPendingConnections()) {
            QTcpSocket* clientSocket = m_tcpServer->nextPendingConnection();
            if (!clientSocket) continue;

            // Запоминаем клиента в списке ожидающих
            auto* client = new ClientData(clientSocket, this);
            m_pendingClientsMap.insert(client->id(), client);

            // Посылаем сигнал в контроллер об ожидании подключения нового клиента
            emit clientPendingConnection(client->id(), client->ipAddress(), client->port());
        }
    }

    // Слот вызывается при получении данных в сокет
    void NetworkWorker::onReadyRead() {
        // Определяем какой сокет прислал сигнал
        auto* clientSocket = qobject_cast<QTcpSocket*>(sender());
        if (!clientSocket || !m_clientsMap.contains(clientSocket)) return;

        ClientData* client = m_clientsMap[clientSocket];

        // Читаем всё из сокета
        while (true) {
            // Читаем JSON из сокета
            QJsonObject obj = Network::NetworkProtocol::readPacket(clientSocket);

            if (obj.isEmpty()) {
                break;
            }

            processIncomingJson(client, obj);
        }
    }

    //Обработка принятого JSON
    void NetworkWorker::processIncomingJson(ClientData* client, const QJsonObject& json) {
        QString type = json["type"].toString();
        QDateTime now = QDateTime::currentDateTime();

        // Передаём данные в GUI поток о приёме данных
        emit dataReceived(client->id(), type, json, now);
    }

    // Слот обработки отключения клиента
    void NetworkWorker::onDisconnected() {
        auto* clientSocket = qobject_cast<QTcpSocket*>(sender());
        if (!clientSocket || !m_clientsMap.contains(clientSocket)) return;

        ClientData* client = m_clientsMap[clientSocket];
        QString clientId = client->id();

        // Посылаем сигнал в GUI об отключении клиента
        emit clientDisconnected(clientId);

        // Удаляем клиента
        m_clientsMap.remove(clientSocket);
        clientSocket->deleteLater();
    }


} // namespace Telecom::Server
