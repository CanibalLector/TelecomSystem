#include "ServerController.hpp"
#include <QTimer>

namespace Telecom::Server {

ServerController::ServerController(QObject* parent)
    : QObject(parent)
    , m_networkThread(new QThread(this))
    , m_worker(new NetworkWorker())
    , m_clientsModel(new ClientsListModel(this)) {

    // Помещаем NetworkWorker в отдельный поток
    m_worker->moveToThread(m_networkThread);

    // Связываем завершение потока NetworkWorker с удалением объекта
    connect(m_networkThread, &QThread::finished, m_worker, &QObject::deleteLater);

    // Настройка связей сетевого потока с контроллером
    connect(m_worker, &NetworkWorker::clientPendingConnection, this, &ServerController::handleClientPending);
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

// Включенние/выключение сервера из QML
void ServerController::toggleServer(quint16 port) {
    if (m_isServerRunning) {
        emit requestStopServer();
        m_isServerRunning = false;
    } else {
        emit requestStartServer(port);
        m_isServerRunning = true;
    }

    emit serverStateChanged(); // Отправляем сигнал в GUI
}

// Добавление ожидающего клиента в список
void ServerController::handleClientPending(const QString& id, const QString& ip, quint16 port) {
    m_clientsModel->addClient(id,QString("%1 (%2:%3)").arg(id, ip).arg(port), ClientsListModel::ActivityState::Disconnected);
}

// Добавление подключённого клиента в список
void ServerController::handleClientConnected(const QString& id, const QString& ip, quint16 port) {
    m_clientsModel->addClient(id,QString("%1 (%2:%3)").arg(id, ip).arg(port), ClientsListModel::ActivityState::Connected);
}

// Удаление клиента из списка подключений
void ServerController::handleClientDisconnected(const QString& id) {
    m_clientsModel->removeClient(id);
}

// Команда на подключение клиента  из QML
void ServerController::setClientConnection(const QString &id, bool state) {

    if(state)
       emit m_worker->requestConnectClient(id);
    else
       emit m_worker->requestDisconnectClient(id);
}

// Команда на старт отправки данных из QML
void ServerController::setClientDataSending(const QString &id, bool state) {

}

} // namespace Telecom::Server
