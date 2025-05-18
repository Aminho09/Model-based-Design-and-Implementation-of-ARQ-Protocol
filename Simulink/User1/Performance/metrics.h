// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QDebug>
#include <QElapsedTimer>
#include <iostream>
#include "udphandler.h"
#include "wrapper.h"


class Metrics: public QObject {
    Q_OBJECT
public:
    explicit Metrics(QObject *parent = nullptr, const QString &localIP = QString(), int localPort = 0,
                        const QString &targetIP = QString(), int targetPort = 0);

    void sendMessage();

signals:
    void sendData(uint8_t data);
    void reset_sender();
    void reset_receiver();

public slots:
    void send_next();
    void calculateMetrics();
    void processOutputs(uint8_t data);
    void countTransmissions();
    void countRetransmissions();

private:
    QElapsedTimer delay;
    QElapsedTimer string_delay;

    UdpHandler handler;
    Wrapper wrapper;

    QQueue<uint8_t> queue;
    QString receivedMessage = "";

    quint16 sent_packets = 0;
    quint16 transmissions = 0;
    quint16 retransmissions = 0;
    uint8_t last_char = 0;

    void countPacket();
};

#endif // MAINWINDOW_H
