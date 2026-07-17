#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

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

    // Launch GUI loop
    return app.exec();
}
