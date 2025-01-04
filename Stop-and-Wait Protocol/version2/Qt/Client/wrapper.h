#ifndef WRAPPER_H
#define WRAPPER_H

#include <QObject>
#include <QTimer>

#include "send.h"

class Wrapper : public QObject
{
    Q_OBJECT
public:
    explicit Wrapper(QObject *parent = nullptr);
    void startModel(int intervalMs);

signals:
    void modelStepCompleted();
    void outputsReady(uint16_t packet, bool ready);

public slots:
    void onInputReady(uint8_t ACK);

private slots:
    void onModelStep();

private:
    send send_Obj;
    QTimer timer;

};

#endif // WRAPPER_H
