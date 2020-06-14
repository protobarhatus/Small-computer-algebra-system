#ifndef EQUATIONROOTSCONDITIONS_H
#define EQUATIONROOTSCONDITIONS_H
#include "abstractexpression.h"
#include "vector"
class RootCondition
{
public:
    enum ConditionType
    {
        LESS_THAN_ZERO,
        BIGGER_THAN_ZERO,
        DONT_EQUAL_ZERO,
        EQUAL_ZERO
    };
    RootCondition();
    RootCondition(const RootCondition & cop);
    RootCondition(RootCondition && mov);
    RootCondition(int var, ConditionType condition, const abs_ex &expression);
    RootCondition(int var, ConditionType condition, abs_ex &&expression);
    RootCondition &operator=(const RootCondition & cop);
    RootCondition &operator=(RootCondition && cop);
    bool check(const abs_ex & root) const;
private:
    int var;
    bool is_expr_independent_of_var;
    bool res_of_independent_condition;
    abs_ex condition_expression;
    ConditionType type;
};

class EquationRootsConditions
{
public:
    EquationRootsConditions();
    EquationRootsConditions(std::vector<RootCondition> && conds);
    EquationRootsConditions(std::initializer_list<RootCondition> && conds);
    EquationRootsConditions(RootCondition && condition);
    EquationRootsConditions(const RootCondition & condition);
    EquationRootsConditions(const EquationRootsConditions & copy);
    EquationRootsConditions(EquationRootsConditions && mov);
    void addCondition(const RootCondition & condition);
    void addCondition(RootCondition && condition);
    bool check(const abs_ex & root) const;
    std::list<abs_ex> selectRoots(const std::list<abs_ex> & roots) const;
    std::list<abs_ex> selectRoots(std::list<abs_ex> && roots) const;
private:
    std::vector<RootCondition> conditions;
};

#endif // EQUATIONROOTSCONDITIONS_H
