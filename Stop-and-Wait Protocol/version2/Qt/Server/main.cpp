// Server Code using Qt
#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <iostream>
#include <queue>
#include <string>

#define PORT 8080

class Server : public QTcpServer {
    Q_OBJECT

public:
    Server(QObject *parent = nullptr) : QTcpServer(parent) {
        if (!this->listen(QHostAddress::Any, PORT)) {
            std::cerr << "Server could not start!" << std::endl;
        } else {
            std::cout << "Server started, waiting for connections..." << std::endl;
        }
    }

protected:
    void incomingConnection(qintptr socketDescriptor) override {
        QTcpSocket *clientSocket = new QTcpSocket(this);
        clientSocket->setSocketDescriptor(socketDescriptor);

        connect(clientSocket, &QTcpSocket::readyRead, this, &Server::readClient);
        connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    }

private slots:
    void readClient() {
        QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
        if (clientSocket) {
            QByteArray data = clientSocket->readAll();
            if (!data.isEmpty()) {
                std::cout << "Received: " << data.toStdString() << std::endl;
                QString processedData = processReceivedData(data);
                clientSocket->write(processedData.toUtf8());
            }
        }
    }

private:
    QString processReceivedData(const QString &data) {
        // Prepend 'z' to the received string
        return "z" + data;
    }
};

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    Server server;
    return a.exec();
}

#include "main.moc"
