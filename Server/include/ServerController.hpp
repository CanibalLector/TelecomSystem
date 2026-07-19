#ifndef SERVER_CONTROLLER_HPP
#define SERVER_CONTROLLER_HPP

#include <QObject>
#include <QThread>
#include <QStringListModel>
#include "NetworkWorker.hpp"

namespace Telecom::Server {

class ServerController : public QObject {
    Q_OBJECT

public:
    explicit ServerController(QObject* parent = nullptr);
    ~ServerController() override;

signals:
    // Сигналы управления NetworkWorker
    void requestStartServer(quint16 port);
    void requestStopServer();

public slots:
    // Включенние/выключение сервера
    void toggleServer(quint16 port);

private slots:
    // Слоты агрегации списков подключений
    void handleClientConnected(const QString& id, const QString& ip, quint16 port);
    void handleClientDisconnected(const QString& id);

private:
    QThread* m_networkThread{nullptr};       // Поток для NetworkWorker
    NetworkWorker* m_worker{nullptr};        // Объект сетевой логики (NetworkWorker)

    QStringListModel* m_clientsModel{nullptr}; // Простая модель списка клиентов

    bool m_isServerRunning{false};           // Флаг состояния сервера
    QStringList m_activeClients;             // Список клиентов
};

} // namespace Telecom::Server

#endif // SERVER_CONTROLLER_HPP
