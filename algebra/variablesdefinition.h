#ifndef VARIABLESDEFINITION_H
#define VARIABLESDEFINITION_H
#include "functionrange.h"
class VariablesDefinition
{
public:
    VariablesDefinition();
    VariablesDefinition(const VariablesDefinition & cop);
    VariablesDefinition(const FunctionRange & def);

    double getMinValue();
    double getMaxValue();
    const FunctionRange& getRange() const;
    void setRange(const FunctionRange & rang);
private:
    FunctionRange range;


};

VariablesDefinition getPositiveDefinition();

#endif // VARIABLESDEFINITION_H
