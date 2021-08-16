#ifndef STAGERESULTDESCRIPTOR_H
#define STAGERESULTDESCRIPTOR_H
#include "algebra/algexpr.h"
#include "interruptingevent.h"
#include "vector"
class StageResultDescriptor
{
public:
    StageResultDescriptor();

    void addEvent(const InterruptingEvent & event);

    AlgExpr& time();
    const AlgExpr& time() const;
private:
    AlgExpr time_mark;

    std::vector<InterruptingEvent> events;

};

StageResultDescriptor inifniteEnd();

#endif // STAGERESULTDESCRIPTOR_H
