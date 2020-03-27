#ifndef VARIABLESDISTRIBUTOR_H
#define VARIABLESDISTRIBUTOR_H
#include "variable.h"
#include <vector>
#include "variablesdefinition.h"

class VariablesDistributor
{
public:
    VariablesDefinition * system_var_def;
    static VariablesDistributor& get();
    static Variable createVariable(VariablesDefinition definition);
    void deleteVariables();
    static VariablesDefinition * getVariablesDefinition(int id);
    static int firstSystemNum();

private:
    //системные переменные нужны чтобы заменить какую-либо функцию для выполнения операции по типу деления или выделения степени
    const int first_system_num = 1000000000;
    VariablesDistributor();
    VariablesDistributor(const VariablesDistributor &) = delete;
    VariablesDistributor& operator=(const VariablesDistributor &) = delete;
    std::vector<VariablesDefinition> variables;

};
void deleteVariables();
Variable getVariable(int id);
Variable systemVar(int num);
Variable systemVar();
QString makeVariablesName(int id);
#endif // VARIABLESDISTRIBUTOR_H
