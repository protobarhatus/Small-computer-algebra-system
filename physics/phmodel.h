#ifndef PHMODEL_H
#define PHMODEL_H
#include "algebra/algexpr.h"
#include "vector"
#include "phobject.h"
#include "stageresultdescriptor.h"
#include "constantsmanager.h"
typedef std::unique_ptr<PhObject> PhObjectPtr;




typedef int ObjId;

class PhModel
{
public:
    PhModel();

    int addObject(const QString & name, PhObjectPtr && object);

    PhObjectPtr& operator[](int id);
    const PhObjectPtr& operator[](int id) const;

    StageResultDescriptor completeContinuityCicle();

    PhObjectPtr makeSimpleKinematicObject(const AlgVector & pos, const AlgVector & vel, const AlgVector & acc);
    PhObjectPtr makeSimpleDynamicObject(const AlgVector & pos, const AlgVector & vel, const AlgVector & acc,
                                        const AlgExpr & mass);

    void setDimensionNumber(int d);

    void addEquation(AlgExpr && equation);
    void addEquation(const AlgExpr & equation);
    void addEquation(AlgVector && equation);
    void addEquation(const AlgVector & equation);

    void setEnableGravityField(bool enabled);

    void setGiven(const std::set<AlgExpr> & vars);
private:


    void setKinematicParams(PhObjectPtr & res, const AlgVector & pos, const AlgVector & vel, const AlgVector & acc);

    AlgVector countForce(int obj_id);

    StageResultDescriptor findContinuityCicleEnd();

    std::vector<InterruptingEvent> findCollisionEvents();
    void solveEquations();
    //!ВСЕ НОВЫЕ ПЕРЕМЕННЫЕ ДОЛЖНЫ ПОПАСТЬ СЮДА
    void recountAllWithNewVariablesValues(const std::map<int, abs_ex> & vars_values);



    std::vector<abs_ex> equations;

    AlgVector createPositionUknownVector();
    AlgVector createVelocityUknownVector();
    AlgVector createAccelerationUknownVector();

    std::vector<StageResultDescriptor> timeStagesMarks;

    std::vector<PhObjectPtr> objects;
    std::vector<QString> objects_names;

    std::set<int> known_variables;

    int dimensions;

    bool enabled_gravity_field;
};




#endif // PHMODEL_H
