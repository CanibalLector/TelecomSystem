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

    // Чтение JSON из сокета
    QJsonObject NetworkProtocol::readPacket(QTcpSocket* socket) {

        //Пустой JSON
        QJsonObject obj;

        //ЕСли что, возвращаем пустой JSON
        if (!socket) {
            return obj;
        }

        // Считываем 4 байта заголовка длины
        QByteArray sizeBuffer = socket->read(sizeof(quint32));
        QDataStream stream(sizeBuffer);
        quint32 expectedSize;
        stream >> expectedSize;

        //Проверяем сколько байт осталось
        if (socket->bytesAvailable() < static_cast<qint64>(expectedSize)) {

            return obj;
        }

        //Вычитываем JSON
        QByteArray jsonData = socket->read(expectedSize);

        QJsonParseError jsonError;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData, &jsonError);

        // Если битый JSON возвращаем пустой
        if (jsonError.error != QJsonParseError::NoError) {
            return obj;
        }

        // Превращаем документ в JSON-объект
        obj = doc.object();

        return obj;
    }





} // namespace Telecom::Network

