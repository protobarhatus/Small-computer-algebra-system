#include "phobject.h"
#include "phmodel.h"
PhObject::PhObject(PhModel * mod) : model(mod)
{

}

void PhObject::setModel(PhModel *mod)
{
    this->model = mod;
}

void PhObject::setStartPosition(const AlgVector &position)
{
    this->start_position = position;
}

void PhObject::setStartVelocity(const AlgVector &vel)
{
    this->start_velocity = vel;
}

void PhObject::setStartAcceleration(const AlgVector &ac)
{
    this->start_acceleration = ac;
}

void PhObject::setAccelerationFunction(const VectorFunction &func)
{
    this->acceleration_function = func;
}

void PhObject::setVelocityFunction(const VectorFunction &func)
{
    this->velocity_function = func;
}

void PhObject::setPositionFunction(const VectorFunction &func)
{
    this->position_function = func;
}

void PhObject::countItsKinematicFunctions()
{
    if (this->position_function == nullptr)
    {
        if (this->velocity_function == nullptr)
        {
            //пока что так, но это работает только если ускорение не зависит от координаты, а вообще здесь нужно
            //решать дифуры
            this->velocity_function = integrate(this->acceleration_function, getTimeArgumentVariable().getId());
            this->velocity_function.addConstant(this->start_velocity);

            this->position_function = integrate(this->velocity_function, getTimeArgumentVariable().getId());
            this->position_function.addConstant(this->start_position);
        }
        else
        {
            this->position_function = integrate(this->velocity_function, getTimeArgumentVariable().getId());
            this->position_function.addConstant(this->start_position);
        }
    }
    if (this->velocity_function == nullptr)
    {
        this->velocity_function = derivative(this->position_function, getTimeArgumentVariable().getId());
    }
    if (this->acceleration_function == nullptr)
    {
        this->acceleration_function = derivative(this->velocity_function, getTimeArgumentVariable().getId());
    }
}

AlgVector PhObject::positionAt(const AlgExpr &time)
{
    return this->position_function.result(time);
}

AlgVector PhObject::velocityAt(const AlgExpr &time)
{
    return this->velocity_function.result(time);
}

AlgVector PhObject::accelerationAt(const AlgExpr &time)
{
    return this->acceleration_function.result(time);
}

void PhObject::statePositionAt(const AlgExpr &time, const AlgVector &position)
{
    this->model->addEquation(this->positionAt(time) - position);
}

void PhObject::stateVelocityAt(const AlgExpr &time, const AlgVector &velocity)
{
    this->model->addEquation(this->velocityAt(time) - velocity);
}

void PhObject::stateAccelerationAt(const AlgExpr &time, const AlgVector &acceleration)
{
    this->model->addEquation(this->accelerationAt(time) - acceleration);
}

void PhObject::setForceInfluenceAbility(bool can_be)
{
    this->can_be_force_influenced = can_be;
}

bool PhObject::canBeInfluencedByForce() const
{
    return this->can_be_force_influenced;
}
