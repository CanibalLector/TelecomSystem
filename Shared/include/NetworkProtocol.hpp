#ifndef NETWORK_PROTOCOL_HPP
#define NETWORK_PROTOCOL_HPP

#include <QTcpSocket>
#include <QByteArray>
#include <QJsonObject>

namespace Telecom::Network {

    // Класс отправляет и принимает JSON через TCP
    class NetworkProtocol {
    public:
        // Отправка JSON в сокет
        static qint64 sendPacket(QTcpSocket* socket, const QJsonObject& jsonObject);

        // Чтение JSON из сокета
        static QJsonObject readPacket(QTcpSocket* socket);

    };

} // namespace Telecom::Network

#endif // NETWORK_PROTOCOL_HPP

