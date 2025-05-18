#ifndef WRAPPER_H
#define WRAPPER_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QDebug>

#include "User1.h"

class Wrapper : public QObject
{
    Q_OBJECT
public:
    explicit Wrapper(QObject *parent = nullptr);
    void initialize();

signals:
    void send_packet(uint16_t packet);
    void receive_ACK(uint8_t ack);
    void receive_data(uint8_t data);
    void messageSent();
    void dequeue();
    void countRetransmissions();

public slots:
    void sendData(uint8_t data);
    void sendAck(uint8_t ack);
    void receivePacket(uint16_t packet);
    void reset_sender();
    void reset_receiver();

private:
    User1 user_Obj;
    QQueue<uint8_t> queue;
    QString receivedMessage = "";
    QTimer timer;

    void callModel();
    void timeout();
};

#endif // WRAPPER_H
