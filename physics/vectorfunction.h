#ifndef VECTORFUNCTION_H
#define VECTORFUNCTION_H
#include <algebra/algexpr.h>
#include <algebra/Vector.h>

class VectorFunction
{
public:
    VectorFunction(const std::vector<Variable>& arguments);
    VectorFunction(const Variable & argument);
    VectorFunction(nullptr_t);

    const VectorFunction& operator=(nullptr_t);
    bool operator==(nullptr_t);

    void setExplicitFunction(const AlgVector & func);
    void setExplicitFunction(AlgVector && func);

    void setInexplicitFunction(const AlgVector & func, const std::vector<Variable> & params);
    void setInexplicitFunction(AlgVector && func, std::vector<Variable> && params);

    AlgVector result(const AlgExpr & argument);
    AlgVector result(const std::vector<AlgExpr> & args);

    void setFunction(const std::vector< DifurResult> & func,
                     const std::vector<Variable> & args_for_each_params, const std::vector<Variable> & params);
    void setFunction(const DifurResult & func, const Variable & arg, const Variable & param);


    friend VectorFunction integrate(const VectorFunction & func, int var);
    friend VectorFunction derivative(const VectorFunction & func, int var);

    void addConstant(const AlgVector & constant);

    void replaceSystemVariablesToExpressions(const std::map<int, abs_ex> & expr);

    bool isExplicit() const;
    const AlgVector& getFunction() const;
private:
    AlgVector function;
    //только для неявного задания функции, funcs_params.size() == function.size();
    //сообщает какие переменные нужно искать при решении уравнения
    std::vector<Variable> funcs_params;
    bool is_explicitly;
    std::vector<Variable> arguments;
};
//x - x0 - v0t - a0t^2 / 2 == 0
//v - vo -a0 t == 0
//a - a0 == 0
VectorFunction integrate(const VectorFunction & func, int var);
VectorFunction derivative(const VectorFunction & func, int var);

VectorFunction getExplicitFunction(const AlgVector & func, const Variable& param);
#endif // VECTORFUNCTION_H
