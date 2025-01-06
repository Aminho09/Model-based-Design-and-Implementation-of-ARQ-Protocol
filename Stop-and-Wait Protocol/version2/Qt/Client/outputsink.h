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
    void sendtoUDP(uint16_t packet);

public slots:
    void processOutput(uint16_t packet, bool ready);

};

#endif // OUTPUTSINK_H
