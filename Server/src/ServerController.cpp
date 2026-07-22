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
    connect(m_worker, &NetworkWorker::clientConnected, this, &ServerController::handleClientConnected);
    connect(m_worker, &NetworkWorker::clientDisconnected, this, &ServerController::handleClientDisconnected);
    connect(this, &ServerController::requestStartServer, m_worker, &NetworkWorker::startServer);
    connect(this, &ServerController::requestStopServer, m_worker, &NetworkWorker::stopServer);

    // Имитация добавления клиентов
    handleClientConnected("111111","192.168.1.23",39393);
    handleClientConnected("222222","192.168.1.23",39343);
    handleClientConnected("333333","192.168.1.23",39493);
    handleClientConnected("444444","192.168.1.23",39394);

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

// Добавление клиента в список подключений
void ServerController::handleClientConnected(const QString& id, const QString& ip, quint16 port) {
    m_activeClients.append(QString("%1 (%2:%3)").arg(id, ip).arg(port));
    m_clientsModel->addClient(id,QString("%1 (%2:%3)").arg(id, ip).arg(port));
}

// Удаление клиента из списка подключений
void ServerController::handleClientDisconnected(const QString& id) {
    // Ищем клиента и удаляем ее из списка
    m_activeClients.erase(std::remove_if(m_activeClients.begin(), m_activeClients.end(),
                                         [&id](const QString& item) { return item.startsWith(id); }), m_activeClients.end());

    m_clientsModel->removeClient(id);
}

// Команда на подключение клиента  из QML
void ServerController::setClientConnection(const QString &id, bool state) {

    // Имитация подключения для проверки
    if(state){
        QTimer::singleShot(600, this, [this, id]() {
            m_clientsModel->setClientActivityState(id, ClientsListModel::ActivityState::Connected);
        });
    }
    else{
        QTimer::singleShot(600, this, [this, id]() {
            m_clientsModel->setClientActivityState(id, ClientsListModel::ActivityState::Disconnected);
        });
    }

}

// Команда на старт отправки данных из QML
void ServerController::setClientDataSending(const QString &id, bool state) {

    // Имитация подключения для проверки
    if(state){
        QTimer::singleShot(600, this, [this, id]() {
            m_clientsModel->setClientActivityState(id, ClientsListModel::ActivityState::Sending);
        });
    }
    else{
        QTimer::singleShot(600, this, [this, id]() {
            m_clientsModel->setClientActivityState(id, ClientsListModel::ActivityState::Connected);
        });
    }
}

} // namespace Telecom::Server
