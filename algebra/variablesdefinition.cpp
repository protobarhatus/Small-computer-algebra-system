#include "variablesdefinition.h"
#include <cmath>
#include "number.h"
VariablesDefinition::VariablesDefinition()
{
    this->range = FunctionRange(nullptr, nullptr, false, false);
}

VariablesDefinition::VariablesDefinition(const VariablesDefinition &cop) : range(cop.range)
{

}

VariablesDefinition::VariablesDefinition(const FunctionRange &def)
{
    this->range = def;
}


VariablesDefinition getPositiveDefinition()
{
    return VariablesDefinition(FunctionRange(FunctionRangeSegment(zero, nullptr, true, false)));
}
double VariablesDefinition::getMinValue()
{
    auto expr = this->range.getMin();
    if (expr == nullptr)
        return std::nan("0");
    return expr->getApproximateValue();
}
double VariablesDefinition::getMaxValue()
{
    auto expr = this->range.getMax();
    if (expr == nullptr)
        return std::nan("0");
    return expr->getApproximateValue();
}

const FunctionRange &VariablesDefinition::getRange() const
{
    return range;
}

void VariablesDefinition::setRange(const FunctionRange &rang)
{
    this->range = rang;
}
