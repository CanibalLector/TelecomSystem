#include "ServerController.hpp"

namespace Telecom::Server {

ServerController::ServerController(QObject* parent)
    : QObject(parent)
    , m_networkThread(new QThread(this))
    , m_worker(new NetworkWorker())                          
    , m_clientsModel(new QStringListModel(this)) {

    // Помещаем NetworkWorker в отдельный поток
    m_worker->moveToThread(m_networkThread);

    // Связываем завершение потока NetworkWorker с удалением объекта
    connect(m_networkThread, &QThread::finished, m_worker, &QObject::deleteLater);

    // Настройка связей сетевого потока с контроллером
    connect(m_worker, &NetworkWorker::clientConnected, this, &ServerController::handleClientConnected);
    connect(m_worker, &NetworkWorker::clientDisconnected, this, &ServerController::handleClientDisconnected);
    connect(this, &ServerController::requestStartServer, m_worker, &NetworkWorker::startServer);
    connect(this, &ServerController::requestStopServer, m_worker, &NetworkWorker::stopServer);

    // Стартуем сетевой поток
    m_networkThread->start();
}

ServerController::~ServerController() {
    // ТОрмозим сервер
    emit requestStopServer();
    m_networkThread->quit();
    // Ждем завершения потока
    m_networkThread->wait();
}

// Включенние/выключение сервера
void ServerController::toggleServer(quint16 port) {
    if (m_isServerRunning) {
        emit requestStopServer();
        m_isServerRunning = false;
    } else {
        emit requestStartServer(port);
        m_isServerRunning = true;
    }
}

// Добавление клиента в список подключений
void ServerController::handleClientConnected(const QString& id, const QString& ip, quint16 port) {
    m_activeClients.append(QString("%1 (%2:%3)").arg(id, ip).arg(port));
    m_clientsModel->setStringList(m_activeClients);
    qDebug() << m_clientsModel->stringList();
}

// Удаление клиента из списка подключений
void ServerController::handleClientDisconnected(const QString& id) {
    // Ищем клиента и удаляем ее из списка
    m_activeClients.erase(std::remove_if(m_activeClients.begin(), m_activeClients.end(),
                                         [&id](const QString& item) { return item.startsWith(id); }), m_activeClients.end());

    m_clientsModel->setStringList(m_activeClients);
    qDebug() << m_clientsModel->stringList();
}

} // namespace Telecom::Server
