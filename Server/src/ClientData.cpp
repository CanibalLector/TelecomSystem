#include "ClientData.hpp"

namespace Telecom::Server {

ClientData::ClientData(QTcpSocket* socket, QObject* parent)
    : QObject(parent)
    , m_socket(socket) {

    if (m_socket) {
        // Генерируем уникальный ID для клиента на основе криптографически стойкого UUID
        m_id = QUuid::createUuid().toString(QUuid::WithoutBraces).left(8);

        // Запоминаем строковый IP-адрес удаленного устройства
        m_ipAddress = m_socket->peerAddress().toString();

        // Запоминаем порт, с которого пришло подключение
        m_port = m_socket->peerPort();

        // Привязываем сам сокет к этой сессии как дочерний объект для автоматического удаления памяти
        m_socket->setParent(this);
    }
}

ClientData::~ClientData() {
    if (m_socket) {
        // Закрываем сокет перед уничтожением сессии
        m_socket->close();
    }
}

} // namespace Telecom::Server
