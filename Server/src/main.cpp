#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "ServerController.hpp"

int main(int argc, char* argv[]) {
    // Init Qt Graphics
    QGuiApplication app(argc, argv);

    // Set ddescription
    app.setOrganizationName("CanibalCorp");
    app.setApplicationName("Telecom_Server");

    Telecom::Server::ServerController controller;

    //Create QML engine
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("serverController", &controller);

    //  Load Main.qml
    engine.loadFromModule("Telecom.Server.Components", "Main");

    // Check creation
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    controller.toggleServer(12345);

    // Launch GUI loop
    return app.exec();
}
