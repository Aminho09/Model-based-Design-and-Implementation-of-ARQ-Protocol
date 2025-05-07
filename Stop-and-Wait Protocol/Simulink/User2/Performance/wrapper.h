#ifndef WRAPPER_H
#define WRAPPER_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QDebug>

#include "User2.h"

class Wrapper : public QObject
{
    Q_OBJECT
public:
    explicit Wrapper(QObject *parent = nullptr);

signals:
    void sendPacket(uint16_t packet);
    void receiveAck(uint8_t ack);
    void receiveData(uint8_t data);
    void dequeue();
    void countRetransmissions();

public slots:
    void initialize(int intervalMS);
    void sendAck(uint8_t ack);
    void sendData(uint8_t data);
    void receivePacket(uint16_t packet);
    void resetSender();
    void resetReceiver();

private:
    User2 user_Obj;
    QTimer stepTimer;

    bool data_event = false;
    bool ack_event = false;
    bool packet_event = false;
    bool reset_send_event = false;
    bool reset_receive_event = false;

    void onModelStep();
    void processOutputs(bool send_ready, bool receive_ready, bool dequeue);
};

#endif // WRAPPER_H
