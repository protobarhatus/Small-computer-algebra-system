#include "phobject.h"
#include "phmodel.h"
#include "algebra/solving_equations.h"
#include "algebra/equationrootsconditions.h"
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

void PhObject::setMass(const AlgExpr &m)
{
    this->mass = m;
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
void PhObject::recountAllWithNewVariablesValues(const std::map<int, abs_ex> &vars)
{
    replaceSystemVariablesToExpressions(this->start_position, vars);
    replaceSystemVariablesToExpressions(this->start_velocity, vars);
    replaceSystemVariablesToExpressions(this->start_acceleration, vars);

    this->position_function.replaceSystemVariablesToExpressions(vars);
    this->velocity_function.replaceSystemVariablesToExpressions(vars);
    this->acceleration_function.replaceSystemVariablesToExpressions(vars);

}

AlgExpr PhObject::getMass()
{
    return this->mass;
}

AlgExpr PhObject::findCollisionTime(PhObject *obj)
{
    if (obj->type() == OBJECT_TYPE_MAT_POINT)
    {
        std::list<abs_ex> equations;
        if (this->position_function.isExplicit() && obj->position_function.isExplicit())
        {
            auto& obj_func = obj->position_function.getFunction();
            auto& func = this->position_function.getFunction();
            for (int i = 0; i < func.size(); ++i)
                equations.push_back(std::move((obj_func[i] - func[i]).getExpr()));
            abs_ex v = systemVarExpr();
            auto res = solveEquationsForOneVariable(equations, getTimeArgumentVariable().getId(),
                                         EquationRootsConditions(RootCondition(v->getId(), RootCondition::BIGGER_THAN_ZERO, v)));
            //надо будет улучшить сортировку естессна а то иногда не понятно что первей
          /*  std::sort(res.begin(), res.end(), [](const abs_ex & a, const abs_ex & b)->bool {
                return (a - b)->getPositionRelativelyZero() < 0;
            }); */
            return *res.begin();

        }
        else
            //TODO
            return AlgExpr();
    }
    return AlgExpr();
}

ObjectType PhObject::type() const
{
    return OBJECT_TYPE_MAT_POINT;
}
