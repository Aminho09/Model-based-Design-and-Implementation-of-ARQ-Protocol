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


signals:
    void sendOutputReady(uint16_t packet);
    void ackReady(uint8_t ack);
    void showMessage(QString message);
    void messageSent();

public slots:
    void storeString(const QString message);
    void sendAck(uint8_t ack);
    void receivePacket(uint16_t packet);

private:
    User1 user_Obj;
    QQueue<uint8_t> queue;
    QString receivedMessage = "";
    QTimer timer;

    void callModel();
    void timeout();
    void sendData(uint8_t data);
    void processOutputs(uint8_t data, uint8_t ack);
};

#endif // WRAPPER_H
