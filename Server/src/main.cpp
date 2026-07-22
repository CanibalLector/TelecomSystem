#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "ServerController.hpp"

int main(int argc, char* argv[]) {
    // Init Qt Graphics
    QGuiApplication app(argc, argv);

    // Set cross platform style
    QQuickStyle::setStyle("Basic");

    // Set ddescription
    app.setOrganizationName("CanibalCorp");
    app.setApplicationName("Telecom_Server");

    Telecom::Server::ServerController controller;

    // Регистрируем тип
    qmlRegisterUncreatableType<Telecom::Server::ClientsListModel>(
        "Telecom.Models", 1, 0, "ClientsListModel",
        "Модель создается только на стороне C++ в ServerController"
        );

    //Create QML engine
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("serverController", &controller);

    //  Load Main.qml
    engine.loadFromModule("Telecom.Server.Components", "Main");

    // Check creation
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    // Launch GUI loop
    return app.exec();
}
