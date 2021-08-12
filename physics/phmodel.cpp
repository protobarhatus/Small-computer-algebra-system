#include "phmodel.h"
#include "algebra/variablesdistributor.h"
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

void PhModel::completeContinuityCicle()
{
    //силы


    for (auto &it : this->objects)
        it->countItsKinematicFunctions();


}

Variable getTimeArgumentVariable()
{
    static Variable time = VariablesDistributor::createVariable(getPositiveDefinition(), "$T");
//    static Variable time = systemVar();
    return time;
}

PhObjectPtr PhModel::makeSimpleKinematicObject(const AlgVector &pos, const AlgVector &vel, const AlgVector &acc)
{
    PhObjectPtr res = std::make_unique<PhObject>(this);
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




    res->setForceInfluenceAbility(false);
    return res;
}

void PhModel::setDimensionNumber(int d)
{
    this->dimensions = d;
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
