#include <QCoreApplication>
#include <QTimer>
#include <QTcpSocket>
#include <QQueue>
#include <iostream>
#include <string>
#include <QThread>
#include "wrapper.cpp"

// External declaration of send object
extern send send_Obj;

// Global queue to hold uint8_t values
QQueue<uint8_t> globalQueue;

// Function to convert a string to uint8_t values and enqueue them
void enqueueString(const std::string& input)
{
    for (char ch : input) {
        globalQueue.enqueue(static_cast<uint8_t>(ch));
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Model Initialization
    send_Obj.initialize();

    // TCP Socket for sending packets
    QTcpSocket senderSocket;
    senderSocket.connectToHost("127.0.0.1", 12345); // Replace with the actual IP and port

    // TCP Socket for receiving ACK
    QTcpSocket receiverSocket;
    receiverSocket.connectToHost("127.0.0.1", 54321); // Replace with the actual IP and port

    // Single Timer for rt_OneStep and Sending Packets
    QTimer mainTimer;
    QObject::connect(&mainTimer, &QTimer::timeout, [&]() {
        // Run the model step
        rt_OneStep();

        // Check and send packets if ready
        if (send_Obj.getready()) {
            uint16_t packet = send_Obj.getpacket();
            QByteArray data;
            data.append(static_cast<char>((packet >> 8) & 0xFF)); // High byte
            data.append(static_cast<char>(packet & 0xFF));        // Low byte

            if (senderSocket.state() == QAbstractSocket::ConnectedState) {
                senderSocket.write(data);
                senderSocket.flush();
                std::cout << "Packet sent: " << packet << std::endl;
            } else {
                std::cerr << "Socket not connected." << std::endl;
            }
        }
    });
    mainTimer.start(10); // Combined timer runs every 10 ms

    QObject::connect(&receiverSocket, &QTcpSocket::readyRead, [&]() {
        while (receiverSocket.bytesAvailable()) {
            QByteArray data = receiverSocket.read(1); // Read 1 byte
            if (!data.isEmpty()) {
                uint8_t ack = static_cast<uint8_t>(data.at(0));
                send_Obj.setACK(ack);
                std::cout << "Received ACK: " << static_cast<int>(ack) << std::endl;
            }
        }
    });

    // Input Loop: Read strings from user input
    QThread::create([&]() {
        while (true) {
            std::string input;
            std::cout << "Enter a string: ";
            std::getline(std::cin, input);
            enqueueString(input);
            std::cout << "String enqueued as uint8 values!" << std::endl;
        }
    })->start();

    // Gracefully close sockets when application exits
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&]() {

        if (senderSocket.state() == QAbstractSocket::ConnectedState) {
            senderSocket.disconnectFromHost();
            if (senderSocket.state() != QAbstractSocket::UnconnectedState) {
                senderSocket.waitForDisconnected();
            }
        }

        if (receiverSocket.state() == QAbstractSocket::ConnectedState) {
            receiverSocket.disconnectFromHost();
            if (receiverSocket.state() != QAbstractSocket::UnconnectedState) {
                receiverSocket.waitForDisconnected();
            }
        }
    });

    return app.exec();
}
