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

void UdpHandler::sendMessage(const uint16_t &message) {
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
                    uint8_t data = static_cast<uint8_t>(datagram[0]);
                    emit messageReceived(sender.toString(), senderPort, data);
        }
    }
}

