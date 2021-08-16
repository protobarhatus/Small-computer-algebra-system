#ifndef CONSTANTSMANAGER_H
#define CONSTANTSMANAGER_H
#include "algebra/variablesdistributor.h"
#include "algebra/algexpr.h"

class ConstantsManager
{
public:
    static ConstantsManager& get();
    //здесь происходит очистка всех переменных в физ. моделе (после выполнения задачи)
    //вызывать надо это вместо VariableDistributor::clear();
    static void clearVariables();
private:
    ConstantsManager();
    ConstantsManager(const ConstantsManager &) = delete;
    void operator=(const ConstantsManager &) = delete;

    enum Constants {
        TIME_FUNCTION_ARGUMENT,
        GRAVITY_ACCELERATION,
        CONSTANTS_LIST_END
    };

    void createConstants();
    std::vector<Variable> constants;


    friend AlgExpr grav();
    friend Variable getTimeArgumentVariable();

};

AlgExpr grav();
Variable getTimeArgumentVariable();

#endif // CONSTANTSMANAGER_H
