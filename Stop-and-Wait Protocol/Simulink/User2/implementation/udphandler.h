#ifndef UDPHANDLER_H
#define UDPHANDLER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QDebug>

class UdpHandler : public QObject {
    Q_OBJECT

public:
    UdpHandler(QObject *parent = nullptr, const QString &localIP = QString(), int localPort = 0,
               const QString &targetIP = QString(), int targetPort = 0);

signals:
    void ackReceived(const uint8_t data);
    void packetReceived(const uint16_t data);

public slots:
    void sendPacket(const uint16_t &message);
    void sendAck(const uint8_t &message);

private slots:
    void receiveMessage();


private:
    QUdpSocket *udpSocket;
    QHostAddress bindAddress;
    quint16 bindPort;
    QHostAddress targetAddress;
    quint16 targetPort;
    void initSocket();
};

#endif // UDPHANDLER_H
