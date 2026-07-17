#include <QCoreApplication>
#include <QDebug>
#include "NetworkProtocol.hpp"

int main(int argc, char *argv[]) {
    //Use QCoreApplication for service
    QCoreApplication app(argc, argv);

    app.setApplicationName("Telecom_Client");

    qDebug() << "client started";

    QTcpSocket socket;

    socket.connectToHost("127.0.0.1", 12345);

    QJsonObject json;
    json["field_1"] = "field_1";
    json["field_2"] = 30;
    json["field_3"] = true;

    Telecom::Network::NetworkProtocol::sendPacket(&socket,json);

    socket.close();

    // Exec loop
    return app.exec();
}
