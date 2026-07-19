#include "NetworkWorker.hpp"
#include "NetworkProtocol.hpp"
#include <QJsonDocument>
#include <QDateTime>

namespace Telecom::Server {

    NetworkWorker::NetworkWorker(QObject* parent) : QObject(parent) {
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
        m_clientsMap.clear();
    }

    // Обработка нового соединения
    void NetworkWorker::onNewConnection() {
        if (!m_tcpServer) return;

        // Извлекаем сокет подключившегося клиента из очереди сервера
        while (m_tcpServer->hasPendingConnections()) {
            QTcpSocket* clientSocket = m_tcpServer->nextPendingConnection();
            if (!clientSocket) continue;

            // Запоминаем клиента
            auto* client = new ClientData(clientSocket, this);
            m_clientsMap.insert(clientSocket, client);

            // Настраиваем подписки на события сокета
            connect(clientSocket, &QTcpSocket::readyRead, this, &NetworkWorker::onReadyRead);
            connect(clientSocket, &QTcpSocket::disconnected, this, &NetworkWorker::onDisconnected);

            // Передаём список клиентов
            QStringList clientsList;
            auto n = 0;
            for (auto it = m_clientsMap.constBegin() ; it != m_clientsMap.constEnd(); ++it) {
                n++;
                QString item = QString::number(n) + ". id: " +QString(it.value()->id());
                clientsList.append(item);
            }

            // Посылаем сигнал в GUI о подключении нового клиента
            emit clientConnected(client->id(), client->ipAddress(), client->port(), clientsList);
        }
    }

    // Слот вызввается при подключении данных в сокет
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

        ClientData* session = m_clientsMap[clientSocket];
        QString clientId = session->id();

        // Удаляем клиента
        m_clientsMap.remove(clientSocket);
        clientSocket->deleteLater();

        // Формируем список клиентов
        QStringList clientsList;
        auto n = 0;
        for (auto it = m_clientsMap.constBegin() ; it != m_clientsMap.constEnd(); ++it) {
            n++;
            QString item = QString::number(n) + ". id: " +QString(it.value()->id());
            clientsList.append(item);
        }

        if(clientsList.isEmpty()) clientsList.append("No clients");

        // Посылаем сигнал в GUI об отключении клиента
        emit clientDisconnected(clientId, clientsList);
    }


} // namespace Telecom::Server
