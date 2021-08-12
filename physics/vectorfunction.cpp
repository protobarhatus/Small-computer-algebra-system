#include "vectorfunction.h"
#include "algebra/variablesdistributor.h"
VectorFunction::VectorFunction(const std::vector<Variable> & args)
{
    this->arguments = args;
}
VectorFunction::VectorFunction(const Variable & args)
{
    this->arguments = { args };
}

VectorFunction::VectorFunction(std::nullptr_t)
{
    this->function = AlgVector::create(0);
    this->arguments.clear();
}

const VectorFunction &VectorFunction::operator=(std::nullptr_t)
{
    this->function = AlgVector::create(0);
    this->arguments.clear();
    return *this;
}

bool VectorFunction::operator==(std::nullptr_t)
{
    return this->function.size() == 0;
}

void VectorFunction::setExplicitFunction(const AlgVector &func)
{
    this->function = func;
    this->is_explicitly = true;
}

void VectorFunction::setExplicitFunction(AlgVector &&func)
{
    this->function = std::move(func);
    this->is_explicitly = true;
}

void VectorFunction::setInexplicitFunction(const AlgVector &func, const std::vector<Variable> & params)
{
    assert(func.size() == params.size());
    this->function = func;
    this->is_explicitly = false;
    this->funcs_params = params;
}

void VectorFunction::setInexplicitFunction(AlgVector &&func,  std::vector<Variable> && params)
{
    assert(func.size() == params.size());
    this->function = std::move(func);
    this->is_explicitly = false;
    this->funcs_params = std::move(params);
}

AlgVector VectorFunction::result(const AlgExpr &argument)
{
    assert(this->arguments.size() == 1);
    AlgVector res = AlgVector::create(function.size());
    for (int i = 0; i < function.size(); ++i)
    {
        if (is_explicitly)
        {
            res[i] = function[i];
          //  qDebug() << res[i].toString();
            setUpExpressionIntoVariable(res[i].getExpr(), argument.getExpr(), this->arguments[0].getId());
        }
        else
        {
            res[i] = function[i];
            setUpExpressionIntoVariable(res[i].getExpr(), argument.getExpr(), this->arguments[0].getId());
            auto eq_res = solveEquation(res[i].getExpr(), funcs_params[i].getId());
            if (eq_res.size() == 0)
                assert(false); //вообще надо бросать исключение типо не может получить результат функции
            if (eq_res.size() > 1)
                assert(false); //хз че делать нужно выбирать как-то.
            res[i] = std::move(*eq_res.begin());
        }
    }
    return res;

}

AlgVector VectorFunction::result(const std::vector<AlgExpr> &args)
{
    assert(this->arguments.size() == args.size());
    AlgVector res = AlgVector::create(function.size());
    for (int i = 0; i < function.size(); ++i)
    {
        if (is_explicitly)
        {
            res[i] = function[i];
            for (int j = 0; j < this->arguments.size(); ++j)
                setUpExpressionIntoVariable(res[i].getExpr(), args[j].getExpr(), this->arguments[0].getId());
        }
        else
        {
            res[i] = function[i];
            for (int j = 0; j < this->arguments.size(); ++j)
                setUpExpressionIntoVariable(res[i].getExpr(), args[j].getExpr(), this->arguments[0].getId());
            auto eq_res = solveEquation(res[i].getExpr(), funcs_params[i].getId());
            if (eq_res.size() == 0)
                assert(false); //вообще надо бросать исключение типо не может получить результат функции
            if (eq_res.size() > 1)
                assert(false); //хз че делать нужно выбирать как-то.
            res[i] = std::move(*eq_res.begin());
        }
    }
    return res;
}

void VectorFunction::setFunction(const std::vector<DifurResult> &func,
                                 const std::vector<Variable> & args_for_each_params, const std::vector<Variable>& params)
{
    assert(func.size() == params.size());
    //если для каждой переменной параметром служит время t, к примеру, то вектор должен иметь func.size() переменных t
    assert(func.size() == args_for_each_params.size());
    this->function = AlgVector::create(func.size());
    bool is_Inexplicit= false;
    for (auto &it : func)
        //решая дифур к примеру F(t) = m*x''(t), SOLVED_FOR_X означает что результат - ф-я t(x), что неправильно, поэтому
        //ф-я сохранится как t - t(x) = 0, откуда будет выражаться x
        //x - всегда аргумент, y - требуемый результат и это должно гарантироваться вовне
        if (it.getType() == DifurResult::COMMON_INTEGRAL || it.getType() == DifurResult::SOLVED_FOR_X)
        {
            is_Inexplicit = true;
            break;
        }
    if (is_Inexplicit)
    {
        AlgVector a_func = AlgVector::create(func.size());
        for (int i = 0; i < func.size();++i)
        {
            a_func[i] = func[i].toCommonIntegral(args_for_each_params[i].getId(), params[i].getId());
        }
        this->setInexplicitFunction(std::move(a_func), std::vector<Variable>(params));
    }
    else
    {
        AlgVector a_func = AlgVector::create(func.size());
        for (int i = 0; i < func.size(); ++i)
            a_func[i] = func[i].expr();
        this->setExplicitFunction(a_func);
    }


}

void VectorFunction::setFunction(const DifurResult &func, const Variable &arg, const Variable &param)
{
    this->setFunction({func}, {arg}, {param});
}

void VectorFunction::addConstant(const AlgVector &constant)
{
    assert(constant.size() == this->function.size());
    if (this->is_explicitly)
    {
        for (int i = 0; i < this->function.size(); ++i)
            this->function[i] += constant[i];
        return;
    }
    else
    {
        for (int i = 0; i < this->function.size(); ++i)
        {
            setUpExpressionIntoVariable(this->function[i].getExpr(),
                                        varToAbsEx(this->funcs_params[i]) + constant[i].getExpr(), this->funcs_params[i].getId());
        }
        return;
    }
}

VectorFunction integrate(const VectorFunction &func, int var)
{
    //да выполняем лишнее копирование но зато не нужно беспокоится о копировании других элементов
    VectorFunction res = func;

    for (int i = 0; i < func.function.size(); ++i)
        res.function[i] = integral(func.function[i], getVariableExpr(var));
    return res;
}

VectorFunction derivative(const VectorFunction &func, int var)
{
    //да выполняем лишнее копирование но зато не нужно беспокоится о копировании других элементов
    VectorFunction res = func;
    for (int i = 0; i < func.function.size(); ++i)
        res.function[i] = derivative(func.function[i], getVariableExpr(var));
    return res;
}

VectorFunction getExplicitFunction(const AlgVector &func, const Variable &param)
{
    VectorFunction res(param);
    res.setExplicitFunction(func);
    return res;
}
