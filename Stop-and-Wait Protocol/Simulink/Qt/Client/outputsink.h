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
    void getInput();

public slots:
    void processOutput(uint16_t packet, bool ready, bool empty);

};

#endif // OUTPUTSINK_H
