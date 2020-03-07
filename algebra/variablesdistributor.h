#ifndef VARIABLESDISTRIBUTOR_H
#define VARIABLESDISTRIBUTOR_H
#include "variable.h"
#include <vector>
#include "variablesdefinition.h"
class VariablesDistributor
{
public:
    static VariablesDistributor& get();
    static Variable createVariable(VariablesDefinition definition);
    void deleteVariables();
    static VariablesDefinition * getVariablesDefinition(int id);
private:

    VariablesDistributor();
    VariablesDistributor(const VariablesDistributor &) = delete;
    VariablesDistributor& operator=(const VariablesDistributor &) = delete;
    std::vector<VariablesDefinition> variables;

};
void deleteVariables();
Variable getVariable(int id);
#endif // VARIABLESDISTRIBUTOR_H
