#include "variablesdefinition.h"
#include <cmath>
VariablesDefinition::VariablesDefinition()
{
    this->max = std::nan("0");
    this->min = std::nan("0");
}
void VariablesDefinition::setMinValue(definition_area_type imin)
{
    this->min = imin;
}
void VariablesDefinition::setMaxValue(definition_area_type imax)
{
    this->max = imax;
}
VariablesDefinition getPositiveDefinition()
{
    VariablesDefinition def;
    def.setMinValue(0);
    return def;
}
definition_area_type VariablesDefinition::getMinValue()
{
    return this->min;
}
definition_area_type VariablesDefinition::getMaxValue()
{
    return this->max;
}
