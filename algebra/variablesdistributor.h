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
    static Variable createVariable(VariablesDefinition definition, const QString & name);
    void deleteVariables();
    static VariablesDefinition * getVariablesDefinition(int id);
    static int firstSystemNum();
    //не знаю насколько синглтону нужен деструктор, но да пусть будет
    ~VariablesDistributor();
    static int firstIntegrateConstant();
    friend abs_ex integratingConstantExpr(int index, const FunctionRange & range);
    static int amountOfVariable(int i);
    static void addSpecialName(int index, const QString & name);
    static bool hasSpecialName(int id);
    static QString getSpecialName(int id);

    static void clear();
    //к примеру {t, {x, v_x, a_x}}
    static void addDifferentialLine(const Variable & var, const std::vector<Variable> & line);
private:
    friend Variable getVariable(int id);
    //системные переменные нужны чтобы заменить какую-либо функцию для выполнения операции по типу деления или выделения степени
    const int first_system_num = 1000000000;
    const int first_integrate_constant = 1500000000;
    friend Variable systemVar(int min, int max);
    friend Variable systemVar();
    std::map<int, QString> special_names;
    VariablesDistributor();
    VariablesDistributor(const VariablesDistributor &) = delete;
    VariablesDistributor& operator=(const VariablesDistributor &) = delete;
    //мы используем указатели, так как когда при добавлении переменной вектор перевыделяется, адреса объектов
    //изменяются, а так как переменные хранят указатели на область определения, то, если бы тут были объекты, то
    //после перевыделения указатели стали бы невалидными
    std::vector<VariablesDefinition*> variables;
    std::vector<VariablesDefinition*> system_variables;
    std::vector<VariablesDefinition*> integrating_constants;

    std::vector<int> amount_of_integrating_constants;
    //к примеру {t, {x, v_x, a_x}}
    //std::vector<std::pair<Variable, std::vector<Variable>>> differential_line;


    friend bool isIntegratingConstant(int index);
    friend Variable integratingConstant();
    friend Variable integratingConstant(const FunctionRange & range);
    friend QString makeIntegratingConstantName(int id);
    static void increaseIntegratingConstant(int id);
    static void decreaseIntegratingConstant(int id);
    friend Variable systemVar(const abs_ex & min, const abs_ex & max, bool min_included, bool max_included);
    friend Variable systemVar(const FunctionRange & range);
    friend class Variable;
    friend bool isIntegratingConstantAndCanChangeIt(int id);
};
int amountOfIntegratingConstant(int index);
QString makeIntegratingConstantName(int id);
bool isIntegratingConstant(int index);
void deleteVariables();
Variable getVariable(int id);
abs_ex getVariableExpr(int id);
Variable systemVar(int num);
Variable systemVar();
abs_ex systemVarExpr();
Variable systemVar(int min, int max);
Variable systemVar(const FunctionRange & range);
Variable systemVar(const abs_ex & min, const abs_ex & max, bool min_included, bool max_included);
abs_ex systemVarExpr(const abs_ex & min, const abs_ex & max, bool min_included, bool max_included);
abs_ex systemVarExpr(const FunctionRange & range);
Variable integratingConstant();
Variable integratingConstant( const FunctionRange & range);
bool isIntegratingConstantAndCanChangeIt(int id);
abs_ex integratingConstantExpr();
abs_ex integratingConstantExpr(const FunctionRange & range);
abs_ex integratingConstantExpr(int index);
abs_ex integratingConstantExpr(int index, const FunctionRange & range);
QString makeVariablesName(int id);
#endif // VARIABLESDISTRIBUTOR_H
