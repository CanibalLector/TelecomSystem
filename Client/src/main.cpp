#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    //Use QCoreApplication for service
    QCoreApplication app(argc, argv);

    app.setApplicationName("Telecom_Client");

    qDebug() << "client started";

    // Event loop
    return app.exec();
}
