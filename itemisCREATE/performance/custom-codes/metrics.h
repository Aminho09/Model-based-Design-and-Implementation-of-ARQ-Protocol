// metrics.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QDebug>
#include <QElapsedTimer>
#include <QQueue>
#include <iostream>

class Metrics: public QObject {
    Q_OBJECT
public:
    explicit Metrics(QObject *parent = nullptr);

    void sendMessage();

signals:
    void sendData(uint8_t data);
    void resetSend();
    void resetReceive();

public slots:
    void sendNext();
    void calculateMetrics();
    void processOutputs(uint8_t data);
    void countTransmissions();
    void countRetransmissions(uint8_t data);

private:
    QElapsedTimer delay;
    QElapsedTimer string_delay;

    QQueue<uint8_t> queue;
    QString receivedMessage = "";

    quint16 sent_packets = 0;
    quint16 transmissions = 0;
    quint16 retransmissions = 0;
    uint8_t last_char = 0;

    void countPacket();
};

#endif // METRICS_H
