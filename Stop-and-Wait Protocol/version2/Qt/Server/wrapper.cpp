#include "wrapper.h"

Wrapper::Wrapper(QObject *parent) : QObject(parent){}

void Wrapper::callModel() {
    bool p_ready = receive_Obj.getready();
    // Perform the Simulink model call function
    receive_Obj.call();

    // Emit the signal with the model output
//    qDebug() << "data:" << receive_Obj.getdata() << "- ack:" << receive_Obj.getACK() << "- ready:" << (bool)(receive_Obj.getready() ^ p_ready);
    if (p_ready != receive_Obj.getready())
        emit outputsReady(receive_Obj.getdata(), receive_Obj.getACK());
}

void Wrapper::onInputReady(uint16_t packet) {
    // Set the inputs for the Simulink model and call the model
    receive_Obj.setpacket(packet);
//    qDebug() << "packet:" << packet;
    this->callModel();
}
