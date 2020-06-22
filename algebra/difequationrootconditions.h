#ifndef DIFEQUATIONROOTCONDITIONS_H
#define DIFEQUATIONROOTCONDITIONS_H
#include "abstractexpression.h"
#include "solving_differential_equations.h"
#include <vector>
class DifEquationRootCondition
{
public:
    enum Type
    {
        DONT_EQUAL_ZERO
    };
    DifEquationRootCondition(const abs_ex & condition, int x, int y, Type t = DONT_EQUAL_ZERO);
    DifEquationRootCondition(const DifEquationRootCondition & difcond);
    DifEquationRootCondition(DifEquationRootCondition && difcond);
    DifEquationRootCondition& operator=(const DifEquationRootCondition & difcond);
    bool checkRootAndCorrectConstant(DifurResult & result) const;

private:
    bool is_false_condition = false;
    bool has_condition_x;
    bool has_condition_y;
    Type type;
    int x, y;
    bool is_condition_difur;
    abs_ex condition;
    std::list<DifurResult> solved_as_difur_condition;
};
class DifursRootConditions
{
public:
    DifursRootConditions(const DifEquationRootCondition & cond);
    DifursRootConditions(DifEquationRootCondition && cond);
    DifursRootConditions(std::initializer_list<DifEquationRootCondition> && conds);
    DifursRootConditions(const DifursRootConditions & conds);
    DifursRootConditions(DifursRootConditions && conds);
    DifursRootConditions();
    bool check( DifurResult& res) const;
    void addCondition(DifEquationRootCondition && condition);
private:
    std::vector<DifEquationRootCondition> conditions;
};

#endif // DIFEQUATIONROOTCONDITIONS_H
