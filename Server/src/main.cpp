#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>

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


    QTcpServer server;

    // listen port
    if (!server.listen(QHostAddress::LocalHost, 12345)) {
        qCritical() << "провал запуска сервера:" << server.errorString();
        return -1;
    }
    qDebug() << "сервер запущен" ;

    //ждём подключения
    while (true) {
        if (!server.waitForNewConnection(-1)) {
            qDebug() << "ошибка ожидания подключения:" << server.errorString();
            break;
        }

        // берём сокет
        QTcpSocket *clientSocket = server.nextPendingConnection();
        if (!clientSocket) continue;

        qDebug() << "клиент подключился:" << clientSocket->peerAddress().toString();

        while (clientSocket->state() == QAbstractSocket::ConnectedState) {

            // Ждём данных 30 сек
            if (clientSocket->waitForReadyRead(30000)) {


                //Cчитываем 4 байта длины
                QByteArray sizeBuffer = clientSocket->read(sizeof(quint32));
                QDataStream stream(sizeBuffer);

                quint32 val;
                stream >> val;
                qDebug() << val << "Packet length";

                QByteArray jsonData = clientSocket->read(val);

                QJsonDocument doc = QJsonDocument::fromJson(jsonData);

                qDebug().noquote() << doc.toJson(QJsonDocument::Indented);


            } else
                break;

        }

        delete clientSocket;
    }

    server.close();



    // Launch GUI loop
    return app.exec();
}
