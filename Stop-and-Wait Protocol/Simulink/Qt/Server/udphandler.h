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
    void messageReceived(const QString &sender, quint16 senderPort, const uint16_t data);

public slots:
    void sendMessage(const uint8_t &message);
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
