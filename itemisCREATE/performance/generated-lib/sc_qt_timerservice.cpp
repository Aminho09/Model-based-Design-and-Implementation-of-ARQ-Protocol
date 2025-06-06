/* Generated by itemis CREATE code generator. */

#include "sc_qt_timerservice.h"


using namespace sc::qt;


SCTimer::SCTimer(QObject *parent, std::weak_ptr<sc::timer::TimedInterface> machine, const sc::eventid id) :
    machine(machine),
    eventId(id)
{
    connect(this, SIGNAL(timeout()), this, SLOT(triggerTimeEvent()));
}

void SCTimer::triggerTimeEvent()
{
    emit fireTimeEvent(machine.lock(), eventId);
}

SCTimer::~SCTimer()
{
}

SCTimerService::SCTimerService(QObject *parent)
{
}

SCTimerService::~SCTimerService()
{
}

void SCTimerService::setTimer(std::shared_ptr<sc::timer::TimedInterface> statemachine, sc::eventid event, sc::time time_ms, bool isPeriodic)
{
    std::shared_ptr<SCTimer> timer = nullptr;

    // retrieve the timer map for the state machine
    QMap<sc::eventid, std::shared_ptr<SCTimer>> *eventTimerMap = machineTimerMapMap.value(statemachine);

    // retrieve and stop a timer if it already exists for the event.
    if (eventTimerMap != nullptr) {

        if (eventTimerMap->contains(event)) {
           std::shared_ptr<SCTimer> timer = eventTimerMap->value(event);
           timer->stop();
        }
    }

    //setup the event timer map for the state machine if it not alrewady exists
    if ( eventTimerMap == nullptr) {
        eventTimerMap = new QMap<sc::eventid, std::shared_ptr<SCTimer>>();
        machineTimerMapMap.insert(statemachine, eventTimerMap);
    }

    // create a timer if it not already exists
    if (timer == nullptr) {
        timer = std::make_shared<SCTimer>(this, statemachine, event);
        eventTimerMap->insert(event, timer);
        timer->connect(timer.get(), SIGNAL(fireTimeEvent(std::shared_ptr<sc::timer::TimedInterface>,sc::eventid)), this, SLOT(raiseTimeEvent(std::shared_ptr<sc::timer::TimedInterface>,sc::eventid)));
    }

    // amor the timer
    timer->setTimerType(Qt::TimerType::PreciseTimer);
    timer->setInterval(time_ms);
    timer->setSingleShot(!isPeriodic);
    timer->start();

}


std::shared_ptr<SCTimer> SCTimerService::getTimer(std::shared_ptr<sc::timer::TimedInterface> machine, sc::eventid event)
{
    std::shared_ptr<SCTimer> timer = nullptr;

    // retrieve the timer map for the state machine
    QMap<sc::eventid, std::shared_ptr<SCTimer>> *eventTimerMap = machineTimerMapMap.value(machine);

    // retrieve and a timer registered for the event.
    if (eventTimerMap != nullptr) {

        if (eventTimerMap->contains(event)) {
            timer = eventTimerMap->value(event);
        }
    }

    return timer;
}


void SCTimerService::unsetTimer(std::shared_ptr<sc::timer::TimedInterface> statemachine, sc::eventid event)
{
    std::shared_ptr<SCTimer> timer = this->getTimer(statemachine, event);

    if (timer != nullptr) {
        timer->stop();
    }

    // do nothing else as the timer may be reused...
}


void SCTimerService::raiseTimeEvent(std::shared_ptr<sc::timer::TimedInterface> machine, sc::eventid event)
{
    std::shared_ptr<SCTimer> timer = this->getTimer(machine, event);
    if (timer != nullptr && timer->isSingleShot()) {
        timer->stop();
    }

    if (machine != nullptr) {
        machine->raiseTimeEvent(event);
    }
}


void SCTimerService::cancel()
{
}


