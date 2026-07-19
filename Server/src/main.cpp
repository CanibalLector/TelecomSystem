#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>
#include "NetworkProtocol.hpp"
#include "QtCore/qjsonobject.h"
#include "NetworkWorker.hpp"

int main(int argc, char* argv[]) {
    // Init Qt Graphics
    QGuiApplication app(argc, argv);

    // Set ddescription
    app.setOrganizationName("CanibalCorp");
    app.setApplicationName("Telecom_Server");

    //Create QML engine
    QQmlApplicationEngine engine;


    //  Load Main.qml
    engine.loadFromModule("Telecom.Server.Components", "Main");

    // Check creation
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    QThread* m_networkThread = new QThread();
    Telecom::Server::NetworkWorker* m_worker = new Telecom::Server::NetworkWorker{nullptr};

    m_worker->moveToThread(m_networkThread);

    emit m_worker->startServer(12345);

    m_networkThread->start();

    // Принимаем данные от клиента (id, тип, JSON-объект, время)
    QObject::connect(m_worker, &Telecom::Server::NetworkWorker::dataReceived,
                     &app, [](const QString& id, const QString& type, const QJsonObject& data, const QDateTime& timestamp) {

        qDebug() << "dataReceived" << "ID клиента:" << id << "Тип пакета:" << type << "Время:"
                 << timestamp.toString("yyyy-MM-dd hh:mm:ss.zzz");

        qDebug().noquote() << "JSON данные:" << QJsonDocument(data).toJson(QJsonDocument::Compact);
    });

    // Принимаем событие подключения нового клиента (id, ip, порт)
    QObject::connect(m_worker, &Telecom::Server::NetworkWorker::clientConnected,
                     &app, [](const QString& id, const QString& ip, quint16 port, const QStringList& ids) {

        qDebug() << "clientConnected" << id << "Адрес:" << QString("%1:%2").arg(ip).arg(port) ;
        qDebug() << ids;
    });

    // Принимаем событие отключения клиента (только id)
    QObject::connect(m_worker, &Telecom::Server::NetworkWorker::clientDisconnected,
                     &app, [](const QString& id, const QStringList& ids) {

        qDebug() << "clientDisconnected" << id;
        qDebug() << ids;
    });



    // Launch GUI loop
    return app.exec();
}
