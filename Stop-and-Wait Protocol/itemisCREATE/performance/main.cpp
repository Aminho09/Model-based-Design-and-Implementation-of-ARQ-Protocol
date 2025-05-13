#include "custom-codes/metrics.h"
#include "custom-codes/udphandler.h"
#include "User.h"
#include "generated-lib/sc_qt_timerservice.h"

#include <QCoreApplication>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    quint16 sendPort = 12345;
    quint16 receivePort = 12346;

    if (argc >= 3) {
        sendPort = QString(argv[1]).toUShort();
        receivePort = QString(argv[2]).toUShort();
    }

    std::shared_ptr<Metrics> metrics = std::make_shared<Metrics>(nullptr);
    std::shared_ptr<UdpHandler> handler = std::make_shared<UdpHandler>(nullptr, "127.0.0.1", sendPort, "127.0.0.1", receivePort);
    std::shared_ptr<User> machine = std::make_shared<User>(nullptr);
    std::shared_ptr<sc::qt::SCTimerService> timerService = std::make_shared<sc::qt::SCTimerService>(nullptr);
    machine->setTimerService(timerService);

    QObject::connect(metrics.get(), &Metrics::sendData, machine.get(), &User::send_data);
    QObject::connect(machine.get(), &User::send_packet, handler.get(), &UdpHandler::sendPacket);
    QObject::connect(handler.get(), &UdpHandler::ackReceived, machine.get(), &User::send_ack);
    QObject::connect(machine.get(), &User::dequeue, metrics.get(), &Metrics::sendNext);
    QObject::connect(metrics.get(), &Metrics::resetSend, machine.get(), &User::reset_sender);
    QObject::connect(handler.get(), &UdpHandler::packetReceived, machine.get(), &User::receive_packet);
    QObject::connect(machine.get(), &User::receive_ack, handler.get(), &UdpHandler::sendAck);
    QObject::connect(machine.get(), &User::receive_data, metrics.get(), &Metrics::processOutputs);
    QObject::connect(metrics.get(), &Metrics::resetReceive, machine.get(), &User::reset_receiver);
    QObject::connect(handler.get(), &UdpHandler::countTransmissions, metrics.get(), &Metrics::countTransmissions);
    QObject::connect(machine.get(), &User::send_packet, metrics.get(), &Metrics::countRetransmissions);

    machine->enter();
    metrics->sendMessage();
    return a.exec();
}
