#ifndef INTERRUPTINGEVENT_H
#define INTERRUPTINGEVENT_H


class InterruptingEvent
{
public:
    enum EventType {
        EVENT_TYPE_INFINITY
    };

    InterruptingEvent(const EventType & type);





private:
    EventType type;
};

#endif // INTERRUPTINGEVENT_H
