#include "mainwindow.h"
#include "udphandler.h"
#include "User.h"
#include "generated-lib/sc_qt_timerservice.h"

#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    quint16 sendPort = 12345;
    quint16 receivePort = 12346;

    if (argc >= 3) {
        sendPort = QString(argv[1]).toUShort();
        receivePort = QString(argv[2]).toUShort();
    }

    std::shared_ptr<MainWindow> window = std::make_shared<MainWindow>(nullptr);
    std::shared_ptr<UdpHandler> handler = std::make_shared<UdpHandler>(nullptr, "127.0.0.1", sendPort, "127.0.0.1", receivePort);
    std::shared_ptr<User> machine = std::make_shared<User>(nullptr);
    std::shared_ptr<sc::qt::SCTimerService> timerService = std::make_shared<sc::qt::SCTimerService>(nullptr);
    machine->setTimerService(timerService);

    QObject::connect(window.get(), &MainWindow::sendData, machine.get(), &User::send_data);
    QObject::connect(machine.get(), &User::send_packet, handler.get(), &UdpHandler::sendPacket);
    QObject::connect(handler.get(), &UdpHandler::ackReceived, machine.get(), &User::send_ack);
    QObject::connect(machine.get(), &User::dequeue, window.get(), &MainWindow::sendNext);
    QObject::connect(window.get(), &MainWindow::resetSend, machine.get(), &User::reset_sender);
    QObject::connect(handler.get(), &UdpHandler::packetReceived, machine.get(), &User::receive_packet);
    QObject::connect(machine.get(), &User::receive_ack, handler.get(), &UdpHandler::sendAck);
    QObject::connect(machine.get(), &User::receive_data, window.get(), &MainWindow::receivedData);
    QObject::connect(window.get(), &MainWindow::resetReceive, machine.get(), &User::reset_receiver);

    machine->enter();
    window->show();
    return a.exec();
}

