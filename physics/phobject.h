#ifndef PHOBJECT_H
#define PHOBJECT_H
#include "algebra/algexpr.h"
#include "algebra/Vector.h"
#include "vectorfunction.h"
class PhModel;
enum ObjectType {
    OBJECT_TYPE_MAT_POINT
};
//такой объект по сути описывает материальную точку, при этом является основой более сложных объектов
class PhObject
{
public:

    PhObject(PhModel * mod);

    void setModel(PhModel * mod);

    void setStartPosition(const AlgVector & position);
    void setStartVelocity(const AlgVector & vel);
    void setStartAcceleration(const AlgVector & ac);

    void setAccelerationFunction(const VectorFunction & func);
    void setVelocityFunction(const VectorFunction & func);
    void setPositionFunction(const VectorFunction & func);

    void setMass(const AlgExpr & m);


    virtual void countItsKinematicFunctions();

    AlgVector positionAt(const AlgExpr & time);
    AlgVector velocityAt(const AlgExpr & time);
    AlgVector accelerationAt(const AlgExpr & time);

    void statePositionAt(const AlgExpr & time, const AlgVector & position);
    void stateVelocityAt(const AlgExpr & time, const AlgVector & velocity);
    void stateAccelerationAt(const AlgExpr & time, const AlgVector & acceleration);

    void setForceInfluenceAbility(bool can_be);
    bool canBeInfluencedByForce() const;

    virtual void recountAllWithNewVariablesValues(const std::map<int, abs_ex> & vars);

    AlgExpr getMass();

    virtual AlgExpr findCollisionTime(PhObject * obj);

    virtual ObjectType type() const;
private:
    //вектора 0 размера показывают что значение отсутствует (nullptr)
    AlgVector start_position = AlgVector::create(0);
    AlgVector start_velocity = AlgVector::create(0);
    AlgVector start_acceleration = AlgVector::create(0);

    VectorFunction position_function = nullptr;
    VectorFunction velocity_function = nullptr;
    VectorFunction acceleration_function = nullptr;

    bool can_be_force_influenced;

    AlgExpr mass;

    PhModel * model;
};


#endif // PHOBJECT_H
