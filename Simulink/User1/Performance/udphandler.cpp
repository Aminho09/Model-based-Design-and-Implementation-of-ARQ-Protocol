#include "udphandler.h"
#include <QDebug>

UdpHandler::UdpHandler(QObject *parent, const QString &localIP, int localPort,
                       const QString &targetIP, int targetPort)
    : QObject(parent), udpSocket(new QUdpSocket(this)), bindAddress(QHostAddress::Any), bindPort(localPort),
      targetAddress(QHostAddress::Any), targetPort(targetPort) {
    if (!localIP.isEmpty()) {
        bindAddress = QHostAddress(localIP);
    }
    if (!targetIP.isEmpty()) {
        targetAddress = QHostAddress(targetIP);
    }
    initSocket();
}

void UdpHandler::initSocket() {
    if (bindPort == 0) {
        qFatal("Port must be set before initializing the socket.");
    }
    if (!udpSocket->bind(bindAddress, bindPort)) {
        qFatal("Failed to bind port: %s", qPrintable(udpSocket->errorString()));
    }
    connect(udpSocket, &QUdpSocket::readyRead, this, &UdpHandler::receiveMessage);
}

void UdpHandler::sendPacket(const uint16_t &message) {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);
    out << message;
    udpSocket->writeDatagram(data, QHostAddress(targetAddress.toString()), targetPort);
    emit countTransmissions();
}

void UdpHandler::sendAck(const uint8_t &message) {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);
    out << message;
    udpSocket->writeDatagram(data, QHostAddress(targetAddress.toString()), targetPort);
}

void UdpHandler::receiveMessage() {
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        if (!datagram.isEmpty()) {
                if (datagram.length() == 1){
                    uint8_t data = static_cast<uint8_t>(datagram[0]);
                    emit ackReceived(data);
                }
                else {
                    uint16_t data = (static_cast<uint16_t>(datagram[0]) << 8) + static_cast<uint16_t>(datagram[1]);
                    emit packetReceived(data);
                }
        }
    }
}

