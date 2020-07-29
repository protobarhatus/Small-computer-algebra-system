#include "equationrootsconditions.h"
#include "some_algebra_expression_conversions.h"
#include "constant.h"
#include "number.h"
EquationRootsConditions::EquationRootsConditions()
{

}

EquationRootsConditions::EquationRootsConditions(std::vector<RootCondition> &&conds)
{
    this->conditions = std::move(conds);
}

EquationRootsConditions::EquationRootsConditions(std::initializer_list<RootCondition> &&conds)
{
    for (auto &it : conds)
        this->conditions.push_back(std::move(it));
}

EquationRootsConditions::EquationRootsConditions(RootCondition &&condition)
{
    this->conditions.push_back(std::move(condition));
}

EquationRootsConditions::EquationRootsConditions(const RootCondition &condition)
{
    this->conditions.push_back(condition);
}

EquationRootsConditions::EquationRootsConditions(const EquationRootsConditions &copy)
{
    this->conditions = copy.conditions;
}

EquationRootsConditions::EquationRootsConditions(EquationRootsConditions &&mov)
{
    this->conditions = std::move(mov.conditions);
}

void EquationRootsConditions::addCondition(const RootCondition &condition)
{
    this->conditions.push_back(condition);
}

void EquationRootsConditions::addCondition(RootCondition &&condition)
{
    this->conditions.push_back(std::move(condition));
}

std::list<abs_ex> EquationRootsConditions::selectRoots(std::list<abs_ex> &&roots) const
{
    std::list<abs_ex> result;

    for (auto &it : roots)
        if (check(it))
            result.push_back(std::move(it));
    return result;

}

bool EquationRootsConditions::check(const abs_ex &root) const
{
    for (auto &it : this->conditions)
        if (!it.check(root))
            return false;
    return true;
}

std::list<abs_ex > EquationRootsConditions::selectRoots(const std::list<abs_ex > &roots) const
{
    std::list<abs_ex> result;
    for (auto &it : roots)
        if (this->check(it))
            result.push_back(copy(it));
    return result;
}

RootCondition::RootCondition()
{

}

RootCondition::RootCondition(const RootCondition &cop) :
    var(cop.var), is_expr_independent_of_var(cop.is_expr_independent_of_var),
    res_of_independent_condition(cop.res_of_independent_condition), type(cop.type),
    condition_expression(copy(cop.condition_expression))
{

}

RootCondition::RootCondition(RootCondition &&mov) :
    var(mov.var), is_expr_independent_of_var(mov.is_expr_independent_of_var),
    res_of_independent_condition(mov.res_of_independent_condition), type(mov.type),
    condition_expression(std::move(mov.condition_expression))
{

}

RootCondition &RootCondition::operator=(const RootCondition &cop)
{
    var = cop.var;
    is_expr_independent_of_var = cop.is_expr_independent_of_var;
    res_of_independent_condition = cop.res_of_independent_condition;
    type = cop.type;
    condition_expression = copy(cop.condition_expression);
    return *this;
}

RootCondition &RootCondition::operator=(RootCondition &&cop)
{
    var = cop.var;
    is_expr_independent_of_var = cop.is_expr_independent_of_var;
    res_of_independent_condition = cop.res_of_independent_condition;
    type = cop.type;
    condition_expression = std::move(cop.condition_expression);
    return *this;
}

RootCondition::RootCondition(RootCondition::ConditionType condition, const abs_ex &expr)
{
    this->type = condition;
    this->condition_expression = copy(expr);
}

RootCondition::RootCondition(int _var, RootCondition::ConditionType condition, const abs_ex &expression) :
    var(_var), condition_expression(licCopy(expression)), type(condition),
    is_expr_independent_of_var(false)
{
    if (!condition_expression->hasVariable(var))
    {

        res_of_independent_condition = check(nullptr);
        is_expr_independent_of_var = true;
    }
}

RootCondition::RootCondition(int _var, RootCondition::ConditionType condition, abs_ex &&expression) :
    var(_var), condition_expression(std::move(expression)), type(condition),
    is_expr_independent_of_var(false)
{
    if (!condition_expression->hasVariable(var))
    {

        res_of_independent_condition = check(nullptr);
        is_expr_independent_of_var = true;
    }
}

bool RootCondition::check(const abs_ex &root) const
{
   // qDebug() << "CHecking root";


    if (is_expr_independent_of_var)
        return res_of_independent_condition;
    abs_ex expr_with_root = copy(condition_expression);

    if (root != nullptr)
        setUpExpressionIntoVariable(expr_with_root, root, var);
   // qDebug() << this->condition_expression->makeStringOfExpression();
    //qDebug() << root->makeStringOfExpression();
   // qDebug () << expr_with_root->makeStringOfExpression();
    abs_ex div_res;
    switch(this->type)
    {
    case BIGGER_THAN_ZERO:
        return expr_with_root->getPositionRelativelyZero() >= 0;
        break;
    case LESS_THAN_ZERO:
        return expr_with_root->getPositionRelativelyZero() <= 0;
        break;
    case DONT_EQUAL_ZERO:
        return !isZero(expr_with_root);
        break;
    case EQUAL_ZERO:
        return isZero(expr_with_root);
    case EQUAL_TWO_PI_INTEGER:
        div_res = expr_with_root / (two * getPi());
        qDebug() << div_res->makeStringOfExpression();
        return div_res->getId() > 0 || (-div_res)->getId() > 0;
        break;
    default:
        assert(false);
    }
}

RootCondition::StrongCheckResult RootCondition::strongCheck(const abs_ex &root, int var) const
{
    abs_ex expr = copy(this->condition_expression);
    setUpExpressionIntoVariable(expr, root, var);
   // qDebug() << expr->toString();
    int pos = expr->getPositionRelativelyZero();
    switch (this->type) {
    case LESS_THAN_ZERO:
            if (pos < 0)
            {
                if (!expr->canBeZero())
                    return TRUE;
                return UNDEFINED;
            }
            if (pos == 0)
                return UNDEFINED;
            return WRONG;
        break;
    case LESS_OR_EQUAL_ZERO:
        if (pos < 0 || isZero(expr))
            return TRUE;
        if (pos == 0)
            return UNDEFINED;
        if (!expr->canBeZero())
            return WRONG;
        return UNDEFINED;
    case EQUAL_ZERO:
        if (isZero(expr))
            return TRUE;
        return WRONG;
    case DONT_EQUAL_ZERO:
        if (isZero(expr))
            return WRONG;
        if (expr->getSetOfVariables().empty())
            return TRUE;
        if (!expr->canBeZero())
            return TRUE;
        return UNDEFINED;
    case BIGGER_THAN_ZERO:
        if (isZero(expr))
            return WRONG;
        if (pos < 0)
            return WRONG;
        if (pos == 0)
            return UNDEFINED;
        if (!expr->canBeZero())
            return TRUE;
        return UNDEFINED;
    case BIGGER_OR_EQUAL_ZERO:
        if (pos > 0)
            return TRUE;
        if (pos == 0)
            return UNDEFINED;
        return WRONG;
    default:
        //сюда никогда не должно входить
        throw;
    }

}

QString RootCondition::toString() const
{
    QString res = this->condition_expression->toString() + " ";
    switch (type) {
    case LESS_THAN_ZERO:
        res += "<";
        break;
    case LESS_OR_EQUAL_ZERO:
        res += QString(QChar(8804));
        break;
    case EQUAL_ZERO:
        res += "=";
        break;
    case DONT_EQUAL_ZERO:
        res += QString(QChar(8800));
        break;
    case BIGGER_OR_EQUAL_ZERO:
        res += QString(QChar(8805));
        break;
    case BIGGER_THAN_ZERO:
        res += ">";
        break;
    case EQUAL_TWO_PI_INTEGER:
        res += "= 2*" + QString(QChar(960)) + "*k, k " + QString(QChar(8712)) + " " + QString(QChar(8469));
        return res;
        break;
    }
    return res + " 0";

}


