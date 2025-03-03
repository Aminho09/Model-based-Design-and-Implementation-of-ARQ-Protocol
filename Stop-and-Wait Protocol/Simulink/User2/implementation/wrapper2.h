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
    void storeString(const QString message);
    void setAckEvent(uint8_t ack);
    void setPacketEvent(uint16_t packet);

private:
    User2 user_Obj;
    QQueue<uint8_t> queue;
    QString receivedMessage = "";
    QTimer timeoutTimer;
    QTimer stepTimer;

    bool data_event = false;
    bool ack_event = false;
    bool packet_event = false;
    bool timeout_event = false;

    void startModel(int intervalMs);
    void onModelStep();
    void setTimeoutEvent();
    void stopModel();
    void setDataEvent(uint8_t data);
    void processOutputs();
};

#endif // WRAPPER_H
