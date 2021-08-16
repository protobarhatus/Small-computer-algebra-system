#include "stageresultdescriptor.h"

StageResultDescriptor::StageResultDescriptor()
{

}

void StageResultDescriptor::addEvent(const InterruptingEvent &event)
{
    this->events.push_back(event);
}

AlgExpr &StageResultDescriptor::time()
{
    return time_mark;
}

const AlgExpr &StageResultDescriptor::time() const
{
    return time_mark;
}

StageResultDescriptor inifniteEnd()
{
    StageResultDescriptor res;
    res.addEvent(InterruptingEvent(InterruptingEvent::EVENT_TYPE_INFINITY));
    return res;
}
