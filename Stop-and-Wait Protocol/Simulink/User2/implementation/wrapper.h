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
    void sendOutputReady(uint16_t packet);
    void ackReady(uint8_t ack);
    void showMessage(QString message);
    void messageSent();

public slots:
    void initialize();
    void storeString(const QString message);
    void sendAck(uint8_t ack);
    void receivePacket(uint16_t packet);

private:
    User2 user_Obj;
    QQueue<uint8_t> queue;
    QString receivedMessage = "";
    QTimer stepTimer;

    bool initial_flag = false;
    bool data_event = false;
    bool ack_event = false;
    bool packet_event = false;
    bool timeout_event = false;
    bool reset_send_event = false;
    bool reset_receive_event = false;

    void startModel(int intervalMs);
    void onModelStep();
    void sendData(uint8_t data);
    void processOutputs(bool send_ready, bool receive_ready, bool dequeue);
    void resetSender();
    void resetReceiver();
};

#endif // WRAPPER_H
