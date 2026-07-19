#ifndef CLIENT_DATA_HPP
#define CLIENT_DATA_HPP

#include <QObject>
#include <QTcpSocket>
#include <QUuid>
#include <QDateTime>

namespace Telecom::Server {

// Класс хранит данные подключённых клиентов.
class ClientData : public QObject {
    Q_OBJECT
public:
    // Конструктор принимает укзатель на сокет и родительский объект
    explicit ClientData(QTcpSocket* socket, QObject* parent = nullptr);

    ~ClientData() override;

    // Геттеры
    QString id() const { return m_id; }               // ID сессии
    QString ipAddress() const { return m_ipAddress; } // Адрес клиента
    quint16 port() const { return m_port; }           // Порт клиента
    bool isActive() const { return m_isActive; }      // Статус подключения
    QTcpSocket* socket() const { return m_socket; }   // Указатель на сокет

    // Метод изменения статуса активности
    void setActive(bool active) { m_isActive = active; }

private:
    QTcpSocket* m_socket{nullptr}; // Сетевой сокет для обмена данными с этим клиентом
    QString m_id;                  // Идентификатор клиента
    QString m_ipAddress;           // Адрес клиента
    quint16 m_port{0};             // Порт клиента
    bool m_isActive{true};         // Флаг, подключен ли клиент в данный момент
};

} // namespace Telecom::Server

#endif // CLIENT_DATA_HPP
