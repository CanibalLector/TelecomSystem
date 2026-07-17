#include "NetworkProtocol.hpp"
#include <QDataStream>
#include <QJsonDocument>

namespace Telecom::Network {

    // Реализация отправки JSON в сокет
    qint64 NetworkProtocol::sendPacket(QTcpSocket* socket, const QJsonObject& jsonObject) {

        if (!socket ) {
            return false; 
        }

        // Преобразуем QJsonObject в QByteArray со сжатием
        QByteArray jsonData = QJsonDocument(jsonObject).toJson(QJsonDocument::Compact);

        // Буффер пакета
        QByteArray packetBuffer;
        
        // Используем QDataStream для правильной сериализации типов
        QDataStream stream(&packetBuffer, QIODevice::WriteOnly);

        // Сначала будет идти 4 байта с размером пакета, что бы избежать склейки пакетов
        stream << static_cast<quint32>(jsonData.size());

        // Дописываем тело JSON-сообщения следом за заголовком длины
        packetBuffer.append(jsonData);

        // Записываем буффер в сокет
        qint64 bytesWritten = socket->write(packetBuffer);


        return bytesWritten;
    }

} // namespace Telecom::Network

