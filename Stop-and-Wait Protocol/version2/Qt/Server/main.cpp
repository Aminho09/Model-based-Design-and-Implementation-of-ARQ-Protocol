#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QObject>
#include <iostream>
#include <string>
#include "receive.h"

// External declaration of the receive model
receive receive_Obj;

// Class for handling TCP connections
class ReceiveWorker : public QObject
{
    Q_OBJECT

public:
    explicit ReceiveWorker(QObject *parent = nullptr)
        : QObject(parent), server(new QTcpServer(this)), clientSocket(nullptr), senderSocket(new QTcpSocket(this)) {}

    void startServer()
    {
        // Start listening for incoming connections on port 12345
        if (!server->listen(QHostAddress::Any, 12345)) {
            std::cerr << "Server failed to start: " << server->errorString().toStdString() << std::endl;
            return;
        }
        std::cout << "Server started. Listening on port 12345." << std::endl;

        // Accept new connections
        connect(server, &QTcpServer::newConnection, this, &ReceiveWorker::handleNewConnection);

        // Connect sender socket to send ACKs
        senderSocket->connectToHost("127.0.0.1", 54321);
        QObject::connect(senderSocket, &QTcpSocket::connected, []() {
            std::cout << "Sender socket connected to ACK server." << std::endl;
        });
    }

public slots:
    void handleNewConnection()
    {
        clientSocket = server->nextPendingConnection();
        std::cout << "New connection established with sender." << std::endl;

        // Connect to readyRead signal to receive data
        connect(clientSocket, &QTcpSocket::readyRead, this, &ReceiveWorker::receivePacket);
    }

    void receivePacket()
    {
        // Read data from the connected socket
        while (clientSocket->bytesAvailable()) {
            QByteArray data = clientSocket->read(2); // Read 2 bytes
            if (data.size() == 2) {
                uint16_t packet = (static_cast<uint8_t>(data[0]) << 8) | static_cast<uint8_t>(data[1]);

                // Set the packet to the receive model
                receive_Obj.setpacket(packet);

                // Call the receive model
                receive_Obj.call();

                // Retrieve the output (data and ACK)
                uint8_t ack = receive_Obj.getACK();
                uint16_t processedData = receive_Obj.getdata();

                std::cout << "Received Packet: " << packet << ", Processed Data: " << processedData
                          << ", letter: " << static_cast<char>(packet)
                          << ", ACK: " << static_cast<int>(ack) <<  std::endl;

                // Send the ACK back through the sender socket
                sendACK(ack);
            }
        }
    }

    void sendACK(uint8_t ack)
    {
        if (senderSocket->state() == QAbstractSocket::ConnectedState) {
            QByteArray ackData;
            ackData.append(static_cast<char>(ack));
            senderSocket->write(ackData);
            senderSocket->flush();
            std::cout << "Sent ACK: " << static_cast<int>(ack) << std::endl;
        } else {
            std::cerr << "Sender socket is not connected." << std::endl;
        }
    }

private:
    QTcpServer *server;
    QTcpSocket *clientSocket; // Socket for receiving data
    QTcpSocket *senderSocket; // Socket for sending ACKs
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Initialize the receive model
    receive_Obj.initialize();

    // Worker to handle TCP connections
    ReceiveWorker worker;
    worker.startServer();

    // Run the event loop
    return app.exec();
}

#include "main.moc"
