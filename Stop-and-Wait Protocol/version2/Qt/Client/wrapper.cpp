#include "wrapper.h"

Wrapper::Wrapper(QObject *parent) : QObject(parent){
    connect(&timer, &QTimer::timeout, this, &Wrapper::onModelStep);
}

void Wrapper::startModel(int intervalMs) {
    // Start the timer with the specified interval
    timer.start(intervalMs);
}

void Wrapper::onInputReady(uint8_t ACK) {
    // Set the inputs for the Simulink model
    send_Obj.setACK(ACK);
}

void Wrapper::onModelStep() {
    // Perform the Simulink model step function
    send_Obj.step();

    // Emit the signal with the model output
    emit outputsReady(send_Obj.getpacket(), send_Obj.getready());
}
