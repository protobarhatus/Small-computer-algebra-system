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
    //не знаю насколько синглтону нужен деструктор, но да пусть будет
    ~VariablesDistributor();
private:
    friend Variable getVariable(int id);
    //системные переменные нужны чтобы заменить какую-либо функцию для выполнения операции по типу деления или выделения степени
    const int first_system_num = 1000000000;
    const int first_integrate_constant = 1500000000;
    friend Variable systemVar(int min, int max);
    friend Variable systemVar();
    VariablesDistributor();
    VariablesDistributor(const VariablesDistributor &) = delete;
    VariablesDistributor& operator=(const VariablesDistributor &) = delete;
    //мы используем указатели, так как когда при добавлении переменной вектор перевыделяется, адреса объектов
    //изменяются, а так как переменные хранят указатели на область определения, то, если бы тут были объекты, то
    //после перевыделения указатели стали бы невалидными
    std::vector<VariablesDefinition*> variables;
    std::vector<VariablesDefinition*> system_variables;
};

void deleteVariables();
Variable getVariable(int id);
Variable systemVar(int num);
Variable systemVar();
Variable systemVar(int min, int max);
Variable integratingConstant();
QString makeVariablesName(int id);
#endif // VARIABLESDISTRIBUTOR_H
