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
        qDebug() << "server started";
    }

    // Остановка сервера
    void NetworkWorker::stopServer() {
        if (!m_tcpServer) return;

        // Закрываем сервер
        m_tcpServer->close();
        delete m_tcpServer;
        m_tcpServer = nullptr;
        qDebug() << "server stopped";
    }

    // Обработка нового соединения
    void NetworkWorker::onNewConnection() {
        if (!m_tcpServer) return;

        // Извлекаем сокет подключившегося клиента из очереди сервера
        while (m_tcpServer->hasPendingConnections()) {
            QTcpSocket* clientSocket = m_tcpServer->nextPendingConnection();
            if (!clientSocket) continue;

            // Настраиваем подписки на события сокета
            connect(clientSocket, &QTcpSocket::readyRead, this, &NetworkWorker::onReadyRead);
            connect(clientSocket, &QTcpSocket::disconnected, this, &NetworkWorker::onDisconnected);
        }
        qDebug() << "onNewConnection";
    }

    // Слот вызввается при подключении данных в сокет
    void NetworkWorker::onReadyRead() {
        // Определяем какой сокет прислал сигнал
        auto* clientSocket = qobject_cast<QTcpSocket*>(sender());
        if (!clientSocket) return;

        qDebug() << "read data";
        // Читаем всё из сокета
        while (true) {
            // Читаем JSON из сокета
            QJsonObject obj = Network::NetworkProtocol::readPacket(clientSocket);

            if (obj.isEmpty()) {
                break;
                qDebug() << "read data empty";
            }

            qDebug().noquote() << QJsonDocument(obj).toJson(QJsonDocument::Indented);
        }
    }

    // Слот обработки отключения клиента
    void NetworkWorker::onDisconnected() {
        auto* clientSocket = qobject_cast<QTcpSocket*>(sender());
        if (!clientSocket) return;

        qDebug() << "NetworkWorker::onDisconnected()";

        clientSocket->deleteLater();
    }


} // namespace Telecom::Server
