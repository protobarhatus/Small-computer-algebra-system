#include "phmodel.h"
#include "algebra/variablesdistributor.h"
#include "algebra/solving_equations.h"
#include "algebra/constant.h"
#include <QIODevice>
PhModel::PhModel()
{

}

int PhModel::addObject(const QString &name, PhObjectPtr &&object)
{
    this->objects_names.push_back(name);
    this->objects.push_back(std::move(object));
    return this->objects.size() - 1;
}

PhObjectPtr &PhModel::operator[](int id)
{
    return this->objects[id];
}

const PhObjectPtr &PhModel::operator[](int id) const
{
    return this->objects[id];
}

StageResultDescriptor PhModel::completeContinuityCicle()
{
    //силы
    for (int i = 0; i < this->objects.size(); ++i)
    {
        if (objects[i]->canBeInfluencedByForce())
        {
            AlgVector force = this->countForce(i);
            objects[i]->setAccelerationFunction(getExplicitFunction(force / objects[i]->getMass(), getTimeArgumentVariable()));
        }
    }



    for (auto &it : this->objects)
        it->countItsKinematicFunctions();

    StageResultDescriptor cicle_end = this->findContinuityCicleEnd();


    return cicle_end;

}


void PhModel::setKinematicParams(PhObjectPtr & res, const AlgVector & pos, const AlgVector & vel, const AlgVector & acc)
{
    if (pos.size() > 0)
        res->setStartPosition(pos);
    else
        res->setStartPosition(createPositionUknownVector());

    if (vel.size() > 0)
        res->setStartVelocity(vel);
    else
        res->setStartVelocity(createVelocityUknownVector());

    if (acc.size() > 0)
    {
        res->setStartAcceleration(acc);
        res->setAccelerationFunction(getExplicitFunction(acc, getTimeArgumentVariable()));
    }

    else
    {
        auto unk = createAccelerationUknownVector();
        res->setStartAcceleration(unk);
        res->setAccelerationFunction(getExplicitFunction(unk, getTimeArgumentVariable()));
    }
}

AlgVector PhModel::countForce(int obj_id)
{
    AlgVector force = AlgVector::create(this->dimensions);
    if (this->enabled_gravity_field)
    {
        if (this->dimensions == 2)
            force += AlgVector(0, -this->objects[obj_id]->getMass() * grav());
        if (this->dimensions == 3)
            force += AlgVector(0, -this->objects[obj_id]->getMass() * grav());
    }

    return force;
}
PhObjectPtr PhModel::makeSimpleKinematicObject(const AlgVector &pos, const AlgVector &vel, const AlgVector &acc)
{
    PhObjectPtr res = std::make_unique<PhObject>(this);


    setKinematicParams(res, pos, vel, acc);


    res->setForceInfluenceAbility(false);
    return res;
}

PhObjectPtr PhModel::makeSimpleDynamicObject(const AlgVector &pos, const AlgVector &vel, const AlgVector &acc, const AlgExpr &mass)
{
    PhObjectPtr res = std::make_unique<PhObject>(this);
    setKinematicParams(res, pos, vel, acc);
    res->setMass(mass);
    res->setForceInfluenceAbility(true);
    return res;
}

void PhModel::setDimensionNumber(int d)
{
    this->dimensions = d;
}

void PhModel::addEquation(AlgExpr &&equation)
{
    this->equations.push_back(std::move(equation.getExpr()));
    this->solveEquations();
}

void PhModel::addEquation(const AlgExpr &equation)
{
    this->equations.push_back(copy(equation.getExpr()));
    this->solveEquations();
}

void PhModel::addEquation(AlgVector &&equation)
{
    for (int i = 0; i < equation.size(); ++i)
        addEquation(std::move(equation[i].getExpr()));
}

void PhModel::addEquation(const AlgVector &equation)
{
    for (int i = 0; i < equation.size(); ++i)
        addEquation(copy(equation[i].getExpr()));
}

void PhModel::setEnableGravityField(bool enabled)
{
    this->enabled_gravity_field = enabled;
}

void PhModel::setGiven(const std::set<AlgExpr> &vars)
{
    this->known_variables.clear();
    for (auto &it : vars)
    {
        this->known_variables.insert(it.getExpr()->getId());
    }

}

StageResultDescriptor PhModel::findContinuityCicleEnd()
{
    return inifniteEnd();
}

std::vector<InterruptingEvent> PhModel::findCollisionEvents()
{
    //for (auto &it : this->objects)
   //     it-
    return std::vector<InterruptingEvent>();
}

void PhModel::solveEquations()
{
    std::vector<int> eqs_variables;
    std::set<int> vars_set;
    for (auto &it : this->equations)
    {
        auto vars = it->getSetOfVariables();
        for (auto &it1 : vars)
            if (!has(this->known_variables, it1) && !has(vars_set, it1))
            {
                eqs_variables.push_back(it1);
                vars_set.insert(it1);
            }
    }
    if (eqs_variables.size() > this->equations.size())
        return;
    auto res = solveSystemOfEquations(equations, eqs_variables);

    std::map<int, abs_ex> vars_replacements;
    for (int i = 0; i < res.size(); ++i)
    {
        vars_replacements.insert({eqs_variables[i], std::move(res[i][0])});
    }

    this->recountAllWithNewVariablesValues(vars_replacements);
    equations.clear();
}

void PhModel::recountAllWithNewVariablesValues(const std::map<int, abs_ex> &vars_values)
{
    for (auto &it : timeStagesMarks)
        replaceSystemVariablesToExpressions(it.time().getExpr(), vars_values);
    for (auto &it : objects)
        it->recountAllWithNewVariablesValues(vars_values);
}

AlgVector PhModel::createPositionUknownVector()
{
    if (this->dimensions == 1)
        return AlgVector(var("x"));
    if (this->dimensions == 2)
        return AlgVector(var("x"), var("y"));
    if (this->dimensions == 3)
        return AlgVector(var("x"), var("y"), var("z"));
    throw QIODevice::tr("Неверное количество измерений");
}

AlgVector PhModel::createVelocityUknownVector()
{
    if (this->dimensions == 1)
        return AlgVector(var("V"));
    if (this->dimensions == 2)
        return AlgVector(var("Vx"), var("Vy"));
    if (this->dimensions == 3)
        return AlgVector(var("Vx"), var("Vy"), var("Vz"));
    throw QIODevice::tr("Неверное количество измерений");
}

AlgVector PhModel::createAccelerationUknownVector()
{
    if (this->dimensions == 1)
        return AlgVector(var("a"));
    if (this->dimensions == 2)
        return AlgVector(var("a_x"), var("a_y"));
    if (this->dimensions == 3)
        return AlgVector(var("a_x"), var("a_y"), var("a_z"));
    throw QIODevice::tr("Неверное количество измерений");
}

