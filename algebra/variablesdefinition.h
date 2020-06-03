#ifndef VARIABLESDEFINITION_H
#define VARIABLESDEFINITION_H
//по необходимости можно переделать в более точный вид
typedef double definition_area_type;
//не учитываются интервалы с включенными или исключенными границами, т. к. пока не надо
//здесь все определение переменной, а не только ее область определения (D)
class VariablesDefinition
{
public:
    VariablesDefinition();
    VariablesDefinition(const VariablesDefinition & cop);
    void setMinValue(definition_area_type min);
    void setMaxValue(definition_area_type max);
    definition_area_type getMinValue();
    definition_area_type getMaxValue();
private:
    definition_area_type min, max;
    bool has_min_border = false, has_max_border = false;


};

VariablesDefinition getPositiveDefinition();

#endif // VARIABLESDEFINITION_H
