#include "constantsmanager.h"
#include "algebra/algexpr.h"
ConstantsManager &ConstantsManager::get()
{
    static ConstantsManager manager;
    return manager;
}

void ConstantsManager::clearVariables()
{

    ConstantsManager::get().constants.clear();
    VariablesDistributor::clear();
    ConstantsManager::get().createConstants();
}

ConstantsManager::ConstantsManager()
{
    this->createConstants();
}

void ConstantsManager::createConstants()
{
    //this->constants.resize(CONSTANTS_LIST_END);

    this->constants.push_back(VariablesDistributor::createVariable(getPositiveDefinition(), "$T"));
    //сделаем так потому что если сделать определение просто точкой, то переменная даункастнется в нее
    //потом исправим
    this->constants.push_back(
            VariablesDistributor::createVariable(VariablesDefinition(FunctionRange(toAbsEx(Number(49, 5)), numToAbs(10), true, true)), "g"));

}


Variable getTimeArgumentVariable()
{
    return ConstantsManager::get().constants[ConstantsManager::TIME_FUNCTION_ARGUMENT];
}
AlgExpr grav()
{
    return AlgExpr(varToAbsEx(ConstantsManager::get().constants[ConstantsManager::GRAVITY_ACCELERATION]));
}
