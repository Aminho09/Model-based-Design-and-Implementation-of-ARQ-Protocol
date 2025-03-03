#ifndef WRAPPER_H
#define WRAPPER_H

#include <QObject>
#include <QDebug>

#include "receive.h"

class Wrapper : public QObject
{
    Q_OBJECT
public:
    explicit Wrapper(QObject *parent = nullptr);
    receive receive_Obj;

signals:
    void modelStepCompleted();
    void outputsReady(uint8_t data, uint8_t ACK);

public slots:
    void onInputReady(uint16_t packet);

private:
    void callModel();
};

#endif // WRAPPER_H
