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

    QThread tread;
    Telecom::Server::NetworkWorker nw;

    emit m_worker->startServer(12345);

    m_networkThread->start();

    // Launch GUI loop
    return app.exec();
}
