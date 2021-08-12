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

void VariablesDefinition::setUndefined()
{
    this->undefined = true;
}

bool VariablesDefinition::isUndefined() const
{
    return this->undefined;
}

void VariablesDefinition::addDifferentialLine(int arg, const std::pair<int, int> &line)
{
    this->differential_lines.insert({arg, line});
}

int VariablesDefinition::getSpecificDerivative(int deriv_var) const
{
    auto it = this->differential_lines.find(deriv_var);
    if (it == this->differential_lines.end())
        return -1;
    return it->second.second;
}

int VariablesDefinition::getSpecificIntegral(int integr_var) const
{
    auto it = this->differential_lines.find(integr_var);
    if (it == this->differential_lines.end())
        return -1;
    return it->second.first;
}
