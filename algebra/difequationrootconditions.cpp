#include "difequationrootconditions.h"
#include "some_algebra_expression_conversions.h"
#include "variablesdistributor.h"
DifEquationRootCondition::DifEquationRootCondition(const abs_ex &cond, int x, int y, Type t)
{
    this->type = t;
    this->condition = copy(cond);
    this->x = x;
    this->y = y;
    this->is_condition_difur = this->condition->hasDifferential();
    if (this->is_condition_difur)
        solved_as_difur_condition = solveDifur(condition, x, y);
    else
        solved_as_difur_condition.push_back(DifurResult(copy(condition), DifurResult::COMMON_INTEGRAL));
    this->has_condition_x = has(cond->getSetOfVariables(), x);
    this->has_condition_y = has(cond->getSetOfVariables(), y);
    if (!has_condition_x && !has_condition_y)
    {
        if (this->type == DONT_EQUAL_ZERO)
            is_false_condition = isZero(condition);
    }

}

DifEquationRootCondition::DifEquationRootCondition(const DifEquationRootCondition &difcond) :
    has_condition_x(difcond.has_condition_x), has_condition_y(difcond.has_condition_y),
    type(difcond.type), x(difcond.x), y(difcond.y), is_condition_difur(difcond.is_condition_difur),
    condition(copy(difcond.condition)),solved_as_difur_condition(difcond.solved_as_difur_condition),
    is_false_condition(difcond.is_false_condition)
{

}

DifEquationRootCondition::DifEquationRootCondition(DifEquationRootCondition &&difcond) :
    has_condition_x(difcond.has_condition_x), has_condition_y(difcond.has_condition_y),
    type(difcond.type), x(difcond.x), y(difcond.y), is_condition_difur(difcond.is_condition_difur),
    condition(std::move(difcond.condition)),solved_as_difur_condition(difcond.solved_as_difur_condition),
    is_false_condition(difcond.is_false_condition)
{

}

DifEquationRootCondition &DifEquationRootCondition::operator=(const DifEquationRootCondition &difcond)
{
    is_false_condition = difcond.is_false_condition;
     has_condition_x = difcond.has_condition_x;
     has_condition_y = difcond.has_condition_y;
     type = difcond.type;
     x = difcond.x;
     y = difcond.y;
     is_condition_difur = difcond.is_condition_difur;
     condition = copy(difcond.condition);
     solved_as_difur_condition = difcond.solved_as_difur_condition;
     return *this;
}

bool DifEquationRootCondition::checkRootAndCorrectConstant(DifurResult &result) const
{
    if (this->is_false_condition)
        return false;
    if (!has_condition_x && !has_condition_y)
        return true;
    if (this->type == DONT_EQUAL_ZERO)
    {

        abs_ex cond = copy(condition);
      //  qDebug() << cond->makeStringOfExpression();
      //  qDebug() << result.expr()->makeStringOfExpression();
        if (result.getType() == DifurResult::SOLVED_FOR_X)
        {
            setUpExpressionIntoVariable(cond, copy(result.expr()), x);
        }
        if (result.getType() == DifurResult::SOLVED_FOR_Y)
        {
            setUpExpressionIntoVariable(cond, copy(result.expr()), y);
        }
        auto checkAndCorrenct = [&result](const abs_ex & cond)->bool {
            if (isZero(cond))
                return false;
            if (isIntegratingConstant(cond->getId()))
            {
                auto range1 = getRangeOfConstantAddictivesThatCanBeChangedAndTakeThemAway(result.expr());
                auto copy_cond = copy(cond);
                auto range2 = getRangeOfConstantAddictivesThatCanBeChangedAndTakeThemAway(copy_cond);
                FunctionRange new_range = range1.subtract(range2);
                if (new_range.isEmpty())
                    return false;
                result.expr() = result.expr() + integratingConstantExpr(new_range);
                return true;
            }
            return true;
        };
        if (result.getType() == DifurResult::COMMON_INTEGRAL)
        {
            if (!this->is_condition_difur)
                cond = cond - result.expr();
            else
            {
                if (this->solved_as_difur_condition.size() == 0)
                    return true;
                for (auto &it : this->solved_as_difur_condition)
                {
                    bool res;
                    if (it.getType() == DifurResult::SOLVED_FOR_X)
                        res = checkAndCorrenct(getVariableExpr(x) - it.expr());
                    else if (it.getType() == DifurResult::SOLVED_FOR_Y)
                        res = checkAndCorrenct(getVariableExpr(y) - it.expr());
                    else
                        res = checkAndCorrenct(it.expr());
                    if (!res)
                        return false;
                }
                return true;
            }
        }
        return checkAndCorrenct(cond);

    }
}

DifursRootConditions::DifursRootConditions(const DifEquationRootCondition &cond)
{
    this->conditions.push_back(cond);
}

DifursRootConditions::DifursRootConditions(DifEquationRootCondition &&cond)
{
    this->conditions.push_back(std::move(cond));
}

DifursRootConditions::DifursRootConditions(std::initializer_list<DifEquationRootCondition> &&conds)
{
    for (auto &it : conds)
        this->conditions.push_back(std::move(it));
}

DifursRootConditions::DifursRootConditions(const DifursRootConditions &conds)
{
    this->conditions = conds.conditions;
}

DifursRootConditions::DifursRootConditions(DifursRootConditions &&conds)
{
    this->conditions = std::move(conds.conditions);
}

DifursRootConditions::DifursRootConditions()
{

}

bool DifursRootConditions::check(DifurResult &res) const
{
    for (auto &it : this->conditions)
        if (!it.checkRootAndCorrectConstant(res))
            return false;
    return true;
}

void DifursRootConditions::addCondition(DifEquationRootCondition &&condition)
{
    this->conditions.push_back(std::move(condition));
}
