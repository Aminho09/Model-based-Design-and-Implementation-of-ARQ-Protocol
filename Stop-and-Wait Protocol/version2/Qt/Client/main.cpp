// Client Code using Qt
#include <QCoreApplication>
#include <QTcpSocket>
#include <QHostAddress>
#include <iostream>
#include <queue>
#include <string>

#define PORT 8080

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QTcpSocket socket;

    socket.connectToHost(QHostAddress::LocalHost, PORT);
    if (!socket.waitForConnected(5000)) {
        std::cerr << "Connection Failed" << std::endl;
        return 1;
    }

    while (true) {
        std::string input;
        std::cout << "Enter a string (or 'exit' to quit): ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        // Convert string to uint8_t queue
        std::queue<uint8_t> uintQueue;
        for (char c : input) {
            uintQueue.push(static_cast<uint8_t>(c));
        }

        // Send uint8_t queue to server
        while (!uintQueue.empty()) {
            uint8_t value = uintQueue.front();
            uintQueue.pop();
            socket.write(reinterpret_cast<const char*>(&value), sizeof(value));
        }
        socket.flush();

        // Signal the end of data
        uint8_t endSignal = 0;
        socket.write(reinterpret_cast<const char*>(&endSignal), sizeof(endSignal));
        socket.flush();

        // Receive processed string from server
        if (socket.waitForReadyRead(5000)) {
            QByteArray receivedData = socket.readAll();
            std::cout << "Processed string from server: " << receivedData.toStdString() << std::endl;
        } else {
            std::cerr << "Failed to receive data from server." << std::endl;
        }
    }

    socket.disconnectFromHost();
    if (socket.state() == QTcpSocket::ConnectedState) {
        socket.waitForDisconnected(5000);
    }

    return 0;
}
