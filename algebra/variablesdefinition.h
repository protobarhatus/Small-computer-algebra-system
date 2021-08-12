#ifndef VARIABLESDEFINITION_H
#define VARIABLESDEFINITION_H
#include "functionrange.h"
#include <map>
//по сути вся мат. информация о переменной. объекты Variable плодятся постоянно, но каждый из них ссылается сюда и всю
//необходимую информацию берем от сюда
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

    void setUndefined();
    bool isUndefined() const;

    void addDifferentialLine(int arg, const std::pair<int, int>& line);
    //-1 значит что такой нет
    int getSpecificDerivative(int deriv_var) const;
    int getSpecificIntegral(int integr_var) const;
private:
    FunctionRange range;
    //Маркировка, не влияющая на выполнение мат. модуля, но служащая для разделения того, является ли эта переменная
    //известной (в физ. плане)
    bool undefined = false;
    //первый аргумент (ключ) - аргумент, к которому привязана линия. первое в паре - переменная сверху
    //(в которую эта переменная интегрируется), второй - "снизу" (в которую дифференцируется)
    //отрицательное означает отсутствие след. уровня
    //например для скорости линия будет выглядеть так: { {t, {x, a}} }
    //числа - индексы соотв. переменной
    std::map<int, std::pair<int, int>> differential_lines;


};

VariablesDefinition getPositiveDefinition();

#endif // VARIABLESDEFINITION_H
