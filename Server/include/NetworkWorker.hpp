#ifndef NETWORK_WORKER_HPP
#define NETWORK_WORKER_HPP

#include <QObject>
#include <QTcpServer>
#include <QMap>
#include <QJsonObject>
#include "NetworkProtocol.hpp"
#include "ClientData.hpp"

namespace Telecom::Server {

// NetworkWorker работа с сетью в другм потоке
class NetworkWorker : public QObject {
    Q_OBJECT
public:
    explicit NetworkWorker(QObject* parent = nullptr);
    ~NetworkWorker() override;

signals:
    // Сигналы в GUI поток о событиях сети
    void dataReceived(const QString& id, const QString& type, const QJsonObject& data, const QDateTime& timestamp);
    void clientConnected(const QString& id, const QString& ip, quint16 port);
    void clientDisconnected(const QString& id);

public slots:

    void startServer(quint16 port);
    void stopServer();

private slots:

    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    QTcpServer* m_tcpServer{nullptr};
    QMap<QTcpSocket*, ClientData*> m_clientsMap;            // Подключённые клиенты

    //Обработка принятого JSON
    void processIncomingJson(ClientData* client, const QJsonObject& json);

};

} // namespace Telecom::Server

#endif // NETWORK_WORKER_HPP
