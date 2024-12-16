#include <QCoreApplication>
#include <QTimer>
#include <QObject>
#include <QThread>
#include <QQueue>
#include <QTcpSocket>
#include <QTcpServer>
#include <iostream>
#include <string>
#include "wrapper.cpp"

// External declaration of send object
extern send send_Obj;

// Global queue to hold uint8_t values
QQueue<uint8_t> globalQueue;

// Worker class for rt_OneStep function
class RtOneStepWorker : public QObject
{
    Q_OBJECT

public slots:
    void executeStep()
    {
        rt_OneStep();
    }
};

// Worker class for sending TCP packets
class TcpSenderWorker : public QObject
{
    Q_OBJECT

public:
    TcpSenderWorker() : socket(new QTcpSocket(this)) {}

    void connectToServer(const QString& host, quint16 port)
    {
        socket->connectToHost(host, port);
        if (!socket->waitForConnected(3000)) {
            std::cerr << "Failed to connect to server." << std::endl;
        } else {
            std::cout << "Connected to server." << std::endl;
        }
    }

public slots:
    void sendPacket()
    {
        if (send_Obj.getready()) {
            uint16_t packet = send_Obj.getpacket();
            QByteArray data;
            data.append(static_cast<char>((packet >> 8) & 0xFF)); // High byte
            data.append(static_cast<char>(packet & 0xFF));        // Low byte

            if (socket->state() == QAbstractSocket::ConnectedState) {
                socket->write(data);
                socket->flush();
                std::cout << "Packet sent: " << packet << std::endl;
            } else {
                std::cerr << "Socket not connected." << std::endl;
            }
        }
    }

private:
    QTcpSocket* socket;
};

// Worker class for receiving uint8 values
class TcpReceiverWorker : public QObject
{
    Q_OBJECT

public:
    TcpReceiverWorker() : socket(new QTcpSocket(this)) {}

    void connectToServer(const QString& host, quint16 port)
    {
        socket->connectToHost(host, port);
        if (!socket->waitForConnected(3000)) {
            std::cerr << "Failed to connect to server." << std::endl;
        } else {
            std::cout << "Connected to server for receiving ACK." << std::endl;
        }

        connect(socket, &QTcpSocket::readyRead, this, &TcpReceiverWorker::receiveAck);
    }

public slots:
    void receiveAck()
    {
        while (socket->bytesAvailable()) {
            QByteArray data = socket->read(1); // Read one byte
            if (!data.isEmpty()) {
                uint8_t ack = static_cast<uint8_t>(data.at(0));
                send_Obj.setACK(ack);
                std::cout << "Received ACK: " << static_cast<int>(ack) << std::endl;
            }
        }
    }

private:
    QTcpSocket* socket;
};

// Function to convert a string to uint8_t values and enqueue them
void enqueueString(const std::string& input)
{
    for (char ch : input) {
        globalQueue.enqueue(static_cast<uint8_t>(ch));
    }
}

int main(int argc, char *argv[])
{
    // Initialize the model
    send_Obj.initialize();
    QCoreApplication app(argc, argv);

    // Thread for rt_OneStep
    QThread rtOneStepThread;
    RtOneStepWorker rtWorker;
    rtWorker.moveToThread(&rtOneStepThread);

    QTimer timer;
    timer.moveToThread(&rtOneStepThread);

    // Connect the timer to the rt_OneStep execution
    QObject::connect(&rtOneStepThread, &QThread::started, [&]() {
        QObject::connect(&timer, &QTimer::timeout, &rtWorker, &RtOneStepWorker::executeStep);
        timer.start(10); // Call executeStep every 10 ms
    });

    rtOneStepThread.start();

    // Thread for TCP sender
    QThread tcpSenderThread;
    TcpSenderWorker tcpSender;
    tcpSender.moveToThread(&tcpSenderThread);

    // Connect the TCP sender to the ready signal
    QObject::connect(&tcpSenderThread, &QThread::started, [&]() {
        tcpSender.connectToServer("127.0.0.1", 12345); // Replace with server's IP and port
    });

    QTimer tcpTimer;
    tcpTimer.moveToThread(&tcpSenderThread);
    QObject::connect(&tcpTimer, &QTimer::timeout, &tcpSender, &TcpSenderWorker::sendPacket);

    QObject::connect(&tcpSenderThread, &QThread::started, [&]() {
        tcpTimer.start(10); // Check and send packet every 10 ms
    });

    tcpSenderThread.start();

    // Thread for TCP receiver
    QThread tcpReceiverThread;
    TcpReceiverWorker tcpReceiver;
    tcpReceiver.moveToThread(&tcpReceiverThread);

    // Connect the TCP receiver to the server
    QObject::connect(&tcpReceiverThread, &QThread::started, [&]() {
        tcpReceiver.connectToServer("127.0.0.1", 54321); // Replace with server's IP and port
    });

    tcpReceiverThread.start();

    // Main thread handles user input
    QThread::create([&]() {
        while (true) {
            std::string input;
            std::cout << "Enter a string: ";
            std::getline(std::cin, input);
            enqueueString(input);
            std::cout << "String enqueued as uint8 values!" << std::endl;
        }
    })->start();

    return app.exec();
}

#include "main.moc"
