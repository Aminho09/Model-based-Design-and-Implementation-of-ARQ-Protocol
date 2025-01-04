#ifndef UDPCOMMUNICATION_H
#define UDPCOMMUNICATION_H

#include <QObject>

class UdpCommunication
{
    Q_OBJECT
public:
    explicit UdpCommunication(QObject *parent = nullptr);
    void receiveAck(int intervalMs);
signals:
    void setAck(uint8_t ACK);
};

#endif // UDPCOMMUNICATION_H
