#ifndef NETWORK_WORKER_HPP
#define NETWORK_WORKER_HPP

#include <QObject>
#include <QTcpServer>
#include <QMap>
#include <QJsonObject>
#include "NetworkProtocol.hpp"

namespace Telecom::Server {

// NetworkWorker работа с сетью в другм потоке
class NetworkWorker : public QObject {
    Q_OBJECT
public:
    explicit NetworkWorker(QObject* parent = nullptr);
    ~NetworkWorker() override;

public slots:

    void startServer(quint16 port);
    void stopServer();

private slots:

    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    QTcpServer* m_tcpServer{nullptr};

};

} // namespace Telecom::Server

#endif // NETWORK_WORKER_HPP
