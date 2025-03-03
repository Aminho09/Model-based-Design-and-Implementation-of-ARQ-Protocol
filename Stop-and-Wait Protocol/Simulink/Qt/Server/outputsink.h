#ifndef OUTPUTSINK_H
#define OUTPUTSINK_H

#include <QObject>
#include <QDebug>

class OutputSink : public QObject
{
    Q_OBJECT
public:
    explicit OutputSink(QObject *parent = nullptr);

signals:
    void sendtoUDP(uint8_t ACK);

public slots:
    void processOutput(uint8_t data, uint8_t ACK);

};

#endif // OUTPUTSINK_H
