#include "solving_differential_equations.h"
#include "polynomial.h"
#include "fractal.h"
#include "number.h"
#include "degree.h"
#include "solving_equations.h"
#include "variablesdistributor.h"
#include "some_algebra_expression_conversions.h"
#include "constant.h"
#include "difequationrootconditions.h"
#include "differential.h"
#include "absexmemorychecker.h"
#include "derivativeobject.h"
#include "sinus.h"
#include "cosinus.h"
#include "algebra/algexpr.h"
#include <QIODevice>

QString formula(const QString & text)
{
    return "<big><font color=\"black\">" + text + "</font></big>";
}
abs_ex copyWithHtmlConstantsIndexes(const abs_ex & expr)
{
    abs_ex cop = copy(expr);
    downgradeIntegratingConstantsIndexes(cop);
    return cop;
}

int getOrderOfHighOrderEquation(const abs_ex & difur, int x, int y);
bool isItFirstOrderExpression(const abs_ex & difur, int x, int y);
bool isEquationOfAppropriateForm(const abs_ex & difur, int x, int y);
bool hasDerivativeObject(const abs_ex & difur, int x, int y);
std::list<abs_ex>  solveDifurInCommonIntegral(const abs_ex & difur, int x, int y, std::vector<QString> & steps);
std::list<abs_ex> solveDifurInCommonIntegral(const abs_ex & difur, int x, int y, std::vector<QString> & steps,
                                             const DifursRootConditions & conditions);
bool isDifferentialOf(abs_ex & expr, int x)
{
    return expr->getId() == DIFFERENTIAL && expr->hasVariable(x);
}

bool isRootOfEquation(abs_ex && equation, const abs_ex & root, int var)
{
 //   qDebug() << root->toString();
  //  qDebug() << equation->toString();
    setUpExpressionIntoVariable(equation, root, var);
   // qDebug() << equation->toString();
    return isZero(equation);
}
//true значит что это дифур с разделяющимися переменными, который по какой-то причине не удалось решить
//(не удалось взять интеграл) и в таком случае программа не будет проверять дифур на других методах

std::pair<std::list<abs_ex>, std::vector<QString>>  tryToSolveDifurWithSeparableVariables(const std::unique_ptr<Polynomial> & difur, int x, int y,
                                                                                          bool & is_that_type_difur)
{
    std::vector<QString> steps;
    //qDebug() << difur->makeStringOfExpression();
    std::unique_ptr<Polynomial> right(new Polynomial(zero.get()));
    std::unique_ptr<Polynomial> left(new Polynomial(zero.get()));
    for (auto &it : difur->getMonomialsPointers())
    {
        if (it->getFractal().first->size() == 0)
            return {std::list<abs_ex> (), steps};
        if (isDifferentialOf(it->getFractal().first->back(), x))
            *right = *right - it;
        else if (isDifferentialOf(it->getFractal().first->back(), y))
            *left = *left + it;
        else
            return {std::list<abs_ex> (), steps};
    }

    steps.push_back(QIODevice::tr("Перенести слагаемые с ") + formula("d" + getVariable(x).toString()) + QIODevice::tr(" на правую часть:"));
    steps.push_back(formula(left->toString() + " = " + right->toString()));

    bool reduced_something = false;
    abs_ex right_common_part = right->reduceCommonPart();
    if (*right_common_part != *one)
        reduced_something = true;
    std::unique_ptr<Fractal> right_frac(new Fractal(right_common_part * copy(right.get())));

    abs_ex left_common_part = left->reduceCommonPart();
    if (*left_common_part != *one)
        reduced_something = true;
    std::unique_ptr<Fractal> left_frac(new Fractal(left_common_part * copy(left.get())));
    if (reduced_something)
    {
        steps.push_back(QIODevice::tr("Вынести за скобки общий множитель:"));
        steps.push_back(formula(left_frac->toString() + " = " + right_frac->toString()));
    }

    right_frac->separatePolynomialsDegree();
    left_frac->separatePolynomialsDegree();
    std::list<abs_ex>  res;
    bool took_something = false;
    auto takeMultipliersWithVariablesAwayToOtherFractal = [&res, &difur, &took_something](fractal_argument * from_erase,
                                                        fractal_argument * to_insert, int var_take, int var_stay,
            bool is_from_erase_numerator)
    {
        for (auto it = from_erase->begin(); it != from_erase->end();)
        {
            if (it->get()->hasVariable(var_take) && !it->get()->hasVariable(var_stay))
            {
                if (is_from_erase_numerator)
                {
                   // qDebug() << it->get()->toString();
                    auto roots = solveEquation(*it, var_take);
                    for (auto &it1 : roots)
                        if (isRootOfEquation(copy(difur.get()), it1, var_take))
                            res.push_back(getVariableExpr(var_take) - std::move(it1));
                }
                to_insert->push_back(copy(it->get()));
                it = from_erase->erase(it);
                took_something = true;
            }
            else
                ++it;
        }
    };
  //  qDebug() << left_frac->makeStringOfExpression();
  //  qDebug() << right_frac->makeStringOfExpression();
   // qDebug() << left_frac->toString();
   // qDebug() << right_frac->toString();
    takeMultipliersWithVariablesAwayToOtherFractal(right_frac->getFractal().first, left_frac->getFractal().second,
                                                   y, x, true);
    takeMultipliersWithVariablesAwayToOtherFractal(right_frac->getFractal().second, left_frac->getFractal().first,
                                                   y, x, false);
    takeMultipliersWithVariablesAwayToOtherFractal(left_frac->getFractal().first, right_frac->getFractal().second,
                                                   x, y, true);
    takeMultipliersWithVariablesAwayToOtherFractal(left_frac->getFractal().second, left_frac->getFractal().first,
                                                   x, y, false);
    left_frac->simplify();
    right_frac->simplify();
    if (took_something)
    {
        steps.push_back(QIODevice::tr("Перенести множители с '") + getVariable(x).toString() + QIODevice::tr("' вправо, а множители с '") + getVariable(y).toString() +
                        QIODevice::tr("' - влево:"));
        steps.push_back(formula(left_frac->toString() + " = " + right_frac->toString()));
    }
    if (res.size() > 0)
    {
        steps.push_back(QIODevice::tr("В процессе деления потеряно ") + QString::number(res.size()) + QIODevice::tr(" решений:"));
        for (auto &it : res)
            steps.push_back(formula(it->toString() + " = 0"));
    }
  //  qDebug() << left_frac->makeStringOfExpression();
   // qDebug() << right_frac->makeStringOfExpression();
    if (left_frac->hasVariable(x) || right_frac->hasVariable(y))
        return {std::list<abs_ex> (), steps};

    is_that_type_difur = true;

    steps.push_back(QIODevice::tr("Проинтегрировать уравнение:"));
    steps.push_back(formula(QString(QChar(8747)) + left_frac->toString() + " = " + QString(QChar(8747)) + right_frac->toString()));


    auto left_integr = integrate(toAbsEx(left_frac));
    auto right_integr = integrate(toAbsEx(right_frac));
  //  qDebug() << left_integr->toString();
 //   qDebug() << right_integr->toString();
    if (left_integr == nullptr)
        steps.push_back(QIODevice::tr("Не удалось найти интеграл левой части"));
    if (right_integr == nullptr)
        steps.push_back(QIODevice::tr("Не удалось найти интеграл правой части"));
    if (left_integr == nullptr || right_integr == nullptr)
        return {std::move(res), std::move(steps)};
    steps.push_back(QIODevice::tr("Общий интеграл уравнения:"));
    steps.push_back(formula(left_integr->toString() + " = " + right_integr->toString() + " + C,    C = const"));

    auto ic = integratingConstantExpr();

    res.push_back(left_integr - right_integr + std::move(ic));
   // qDebug() << difur->toString();
   // qDebug() << res.back()->toString();
   // for (auto &it : res)
     //   qDebug() << it->toString();
    return {std::move(res), std::move(steps)};
}
std::pair<std::list<abs_ex>, std::vector<QString>> tryToSolveFirstOrderHomoheneousDifur(abs_ex && difur, int x, int y)
{
    std::vector<QString> steps;
    assert(difur->getId() == POLYNOMIAL);
    auto alpha = systemVarExpr(zero, nullptr, true, false);
    static_cast<Variable*>(alpha.get())->setName(QChar(955));
    auto dx = systemVarExpr();
    difur->changeSomePartOn(D(getVariableExpr(x))->makeStringOfExpression(), dx);
    auto dy = systemVarExpr();
    difur->changeSomePartOn(D(getVariableExpr(y))->makeStringOfExpression(), dy);
    setUpExpressionIntoVariable(difur, alpha*getVariableExpr(x), x);
    setUpExpressionIntoVariable(difur, alpha*getVariableExpr(y), y);


    setUpExpressionIntoVariable(difur, D(getVariableExpr(x)), dx->getId());

    QString xs = getVariable(x).toString();
    QString ys = getVariable(y).toString();

    steps.push_back(QIODevice::tr("Подставить: ") + formula(xs + " = " + QString(QChar(955)) + xs + "; " + ys + " = " + QString(QChar(955)) + ys));
    steps.push_back(formula(difur->toString()));
    if (difur->getId() != POLYNOMIAL)
        return {std::list<abs_ex> (), steps};
    static_cast<Polynomial*>(difur.get())->reduceCommonPart();
    if (difur->hasVariable(alpha->getId()))
        return {std::list<abs_ex> (), steps};
    steps.push_back(QIODevice::tr("Так как ") + formula(QString(QChar(955))) + " сократились, это однородное уравнение");
    steps.push_back(QIODevice::tr("Провести замену ") + formula(ys + " = " + unicode(951) + xs + ", => " + ys + "' = " + unicode(951) + "'" + xs + " + " + unicode(951)) + ":");

    abs_ex t = systemVarExpr();
    t->setName(unicode(951));
    setUpExpressionIntoVariable(difur, t*getVariableExpr(x), y);
    //qDebug() << dy->toString();
    //qDebug() << t->toString();
   // qDebug() << difur->toString();
    setUpExpressionIntoVariable(difur, (D(t)/D(getVariableExpr(x)) * getVariableExpr(x) + t)*D(getVariableExpr(x)), dy->getId());

    //это нужно для того, чтобы убрать появившиеся модули
    abs_ex var_to_open_abs = systemVarExpr(zero, nullptr, true, false);
    setUpExpressionIntoVariable(difur, var_to_open_abs, x);
    setUpExpressionIntoVariable(difur, getVariableExpr(x), var_to_open_abs->getId());

    steps.push_back(formula(difur->toString() + " = 0"));
    steps.push_back("");
    //  qDebug() << difur->toString();
    auto res = solveDifur(difur, x, t->getId());
    if (res.first.empty())
        return {std::list<abs_ex> (), steps};

    for (auto &it : res.second)
        steps.push_back(it);

    std::list<abs_ex> actual_res;
    //for (auto &it : res)
    //    qDebug() << it.toString();
    steps.push_back("");
    steps.push_back(QIODevice::tr("Провести обратную замену ") + formula(unicode(951) + " = " + ys + "/" + xs));
    for (auto &it : res.first)
        if (it.getType() != DifurResult::SOLVED_FOR_X &&
                !(it.getType() == DifurResult::COMMON_INTEGRAL &&
                  !it.expr()->hasVariable(y)))
        actual_res.push_back(it.toCommonIntegral(x, t->getId()));
  //  for (auto &it : actual_res)
     //   qDebug() << it->toString();
    for (auto &it : actual_res)
        setUpExpressionIntoVariable(it, getVariableExpr(y)/getVariableExpr(x), t->getId());


    return {std::move(actual_res), steps};
}
QString toMultString(const abs_ex & expr)
{
    if (*expr == *one)
        return "";
    if (expr->getId() == POLYNOMIAL)
        return "(" + expr->toString() + ")*";
    return expr->toString() + "*";
}

std::pair<std::list<abs_ex>, std::vector<QString>> tryToSolveHeterogeneousDifur(const std::unique_ptr<Polynomial> & difur, int x, int y)
{
    //ЛИНЕЙНОЕ
    //неоднородное уравнение в справочниках имеет вид r(x)*y' + p(x)*y = q(x), а в нашем случае,
    //  r(x)*dy + p(x)*y*dx +q(x)*dx = 0
    std::vector<QString> steps;
    abs_ex r = copy(zero);
    abs_ex p = copy(zero);
    abs_ex q = copy(zero);
    auto monoms = difur->getMonoms();

    QString xs = getVariable(x).toString();
    QString ys = getVariable(y).toString();

    for (auto &it : *monoms)
    {
        if (it->hasVariable(x) && it->hasVariable(y))
            return {std::list<abs_ex>(), steps};
        if (!it->hasVariable(y))
        {
            if (!it->hasDifferential())
                return {std::list<abs_ex> (), steps};
            q = std::move(q) + it->downcast()/D(getVariableExpr(x));
        }
        else
        {
            auto fr_without_var = it->getFractalWithoutVariable(y);
            auto fr_with_var = copy(it.get())->downcast()/copy(fr_without_var.get())->downcast();
            if (fr_with_var->getId() == y)
            {
                if (!fr_without_var->hasDifferential())
                    return {std::list<abs_ex> (), steps};
                p = std::move(p) + fr_without_var->downcast()/D(getVariableExpr(x));
            }
            else if (fr_with_var->getId() == DIFFERENTIAL)
            {
                r = std::move(r) + fr_without_var->downcast();
            }
            else
                return {std::list<abs_ex> (), steps};
        }
    }
    QString minus_q = (-q)->toString();
    steps.push_back(QIODevice::tr("Это неоднородное уравнение"));
    steps.push_back(QIODevice::tr("Привести уравнение к виду ") + formula(toMultString(r) + ys + "' + "  +toMultString(p) + ys + " = " + minus_q));
    steps.push_back(QIODevice::tr("Провести замену ") + formula(ys + " = " + unicode(956) + unicode(957) + ", => " + ys + "' = " + unicode(956) + "'" + unicode(957) + " + " +
                    unicode(956) + unicode(957) + "'") + ":");
    steps.push_back(formula(toMultString(r) + unicode(956) + "'" + unicode(957) + " + " + toMultString(r) + unicode(956) + unicode(957) + "'" + " + " + toMultString(p) +
                    unicode(956) + unicode(957) + " = " + minus_q));
    steps.push_back(QIODevice::tr("Сгруппировать данным образом:"));
    steps.push_back(formula(toMultString(r) + unicode(956) + "'" + unicode(957) + " + " + unicode(956) + "(" + toMultString(r) + unicode(957) + " + " + "'" + toMultString(p) +
                    unicode(957) + ") = " + minus_q));
    steps.push_back(QIODevice::tr("Составить систему уравнений: "));
    steps.push_back(formula(QIODevice::tr("{ ") + toMultString(r) + unicode(957) + "'" + " + " + toMultString(p) + unicode(957) + " = 0,"));
    steps.push_back(formula(QIODevice::tr("{ ") + toMultString(r) + unicode(956) + "'" + unicode(957) + " = " + minus_q + "."));
    steps.push_back(QIODevice::tr("Выразить ") + unicode(957) + QIODevice::tr(" из первого уравнения:"));

    auto p_r_div = -p/r;
    steps.push_back(formula(unicode(957) + " = e^" + unicode(8747) + "(" + p_r_div->toString() + ")d" + xs));

   // qDebug() << r->toString();
   // qDebug() << p->toString();
   // qDebug() << q->toString();
    abs_ex antideriv = (-p/r)->antiderivative(x);
    if (antideriv == nullptr)
        return {std::list<abs_ex> (), steps};
    antideriv = getExpressionWithoutAbsoluteValues(antideriv);
    abs_ex v = pow(getEuler(), antideriv);

    steps.push_back(formula(unicode(957) + " = " + v->toString()));

    abs_ex u = systemVarExpr();
    u->setName(unicode(956));

    //qDebug() << v->toString();
   // qDebug() << (r*D(u)*v + q*D(getVariableExpr(x)))->toString();
    steps.push_back(QIODevice::tr("Подставить ") + formula(unicode(957)) + " во второе уравнение:");
    auto second_difur = r*D(u)*v + q*D(getVariableExpr(x));
    steps.push_back(formula(second_difur->toString() + " = 0"));
    steps.push_back("");
    auto second_eq_res = solveDifur(second_difur, x, u->getId());

    for (auto &it : second_eq_res.second)
        steps.push_back(it);
    steps.push_back("");
    steps.push_back(formula(ys + " = " + u->toString() + unicode(957)));

    std::list<abs_ex> res;
    for (auto &it1 : second_eq_res.first)
    {
       // qDebug() << it1.toString();
        if (it1.getType() == DifurResult::SOLVED_FOR_Y)
            res.push_back(getVariableExpr(y) - v * it1.expr());
    }

    return {std::move(res), steps};

}
QString toDegreeString(const abs_ex & degr)
{
    if (degr->getId() == FRACTAL || degr->getId() == POLYNOMIAL)
        return + "(" + degr->toString() + ")";
    return degr->toString();
}
std::pair<std::list<abs_ex>, std::vector<QString>> tryToSolveBernullyEquation(std::unique_ptr<Polynomial> && difur, int x, int y)
{
    //уравнение бернулли имеет вид r(x)*y' + p(x)*y + q(x)*y^n = 0
    //или, в случае этой алгебр. системы r(x)*dy + p(x)*y*dx + q(x)*y^n*dx = 0
    //однако, если n < 0, то мы не сможем получить уравнение вида r(x)*dy + p(x)*y*dx + q(x) * 1/y^(-n) * dx = 0,
    //мы получим уравнение вида r(x)*y^n*dy + p(x)*y^(n + 1)*dx + q(x) * dx = 0, поэтому это надо исправить
    //почему я пишу "мы"  если я один?
    std::vector<QString> steps;
    QString xs = getVariable(x).toString();
    QString ys = getVariable(y).toString();

    auto monoms = difur->getMonoms();
    abs_ex y_mult = nullptr;

    for (auto &it : *monoms)
    {
        if (it->hasDifferentialOfVarAsMultiplier(y))
        {
            abs_ex frac_with_y = copy(it.get())/copy(it->getFractalWithoutVariable(y).get());
            frac_with_y = std::move(frac_with_y) / D(getVariableExpr(y));
            if (Degree::getArgumentOfDegree(frac_with_y.get())->getId() == y)
            {
                y_mult = std::move(frac_with_y);
                break;
            }
            else if (frac_with_y->hasVariable(y))
                return {std::list<abs_ex> (), steps};
            break;
        }
    }
    if (y_mult != nullptr)
    {
        steps.push_back(QIODevice::tr("Разделить уравнение на ") + formula(y_mult->toString()) + ":");
        for (auto &it : *monoms)
            it = toFrac(toAbsEx(std::move(it))/y_mult);
        steps.push_back(formula(difur->toString() + " = 0"));
    }
  //  qDebug() << difur->toString();
    abs_ex r = copy(zero);
    abs_ex p = copy(zero);
    abs_ex q = copy(zero);

    abs_ex n = nullptr;
    for (auto &it : *monoms)
    {
       // qDebug() << it->toString();
        auto frac_without_y = (it->getFractalWithoutVariable(y));
        auto frac_with_y = toAbsEx(it)/toAbsEx(frac_without_y);
        if (frac_with_y->getId() == DIFFERENTIAL)
            r = std::move(r) + std::move(frac_without_y);
        else
        {
            if (!frac_without_y->hasDifferentialOfVarAsMultiplier(x))
                return {std::list<abs_ex> (), steps};
            if (frac_with_y->getId() == y)
                p = std::move(p) + toAbsEx(frac_without_y)/D(getVariableExpr(x));
            else if (Degree::getArgumentOfDegree(frac_with_y.get())->getId() == y)
            {
                if (n == nullptr)
                    n = Degree::getDegreeOfExpression(frac_with_y.get());
                else if (*n != *Degree::getDegreeOfExpression(frac_with_y.get()))
                    return {std::list<abs_ex> (), steps};
                q = std::move(q) + toAbsEx(frac_without_y)/D(getVariableExpr(x));
            }
            else if (Degree::getArgumentOfDegree(one/frac_with_y)->getId() == y)
            {
                if (n == nullptr)
                    n = -Degree::getDegreeOfExpression(one/frac_with_y);
                else if (*n != *-Degree::getDegreeOfExpression(one/frac_with_y))
                    return {std::list<abs_ex> (), steps};
                q = std::move(q) + toAbsEx(frac_without_y)/D(getVariableExpr(x));
            }
            else
                return {std::list<abs_ex> (), steps};
        }

    }
    //r(x)*y' + p(x)*y + q(x)*y^n = 0
    auto minus_q = -q;
    steps.push_back(QIODevice::tr("Представить уравнение в следующем виде:"));
    steps.push_back(formula(toMultString(r) + ys + "'" + " + " + toMultString(p) + ys + " = " + toMultString(minus_q) + ys + "^" + toDegreeString(n)));
    steps.push_back(QIODevice::tr("Тип уравнения: уравнение Бернулли"));

    if (n->getPositionRelativelyZero() >= 0)
        steps.push_back(QIODevice::tr("Сохранить решение ") + formula(ys + " = 0"));

    std::list<abs_ex> res;
    //y = 0 - решение практически любого уравнения Бернулли, которое теряется в ходе дальнейших преобразований
    if (n->getPositionRelativelyZero() >= 0)
        res.push_back(getVariableExpr(y));

    // переделываем уравнение в вид r(x)dy/y^n + p(x)/y^(n-1) = -q(x), замена z(x) = 1/y^(n-1), dy/y^n = dz/(1-n)
    steps.push_back(QIODevice::tr("Привести уравнение к виду:"));
    steps.push_back(formula(toMultString(r) + ys + "'/" + ys + "^" + toDegreeString(n) + " + " + toMultString(p) + "/" + ys + "^" + toDegreeString(n - one) + " = " + minus_q->toString()));
    abs_ex z = systemVarExpr();
    z->setName(unicode(947));

    steps.push_back(QIODevice::tr("Провести замену: ") + formula(z->toString() + " = " + (one/pow(getVariableExpr(y), n - one))->toString() + ", => " + ys + "'/" + ys +
                    "^" + toDegreeString(n) + " = " + z->toString() + (one/(one - n))->toString()));
    auto subdifur = r*D(z)/(one - n) + p*z*D(getVariableExpr(x)) + q*D(getVariableExpr(x));
    //qDebug() << (r*D(z)/(one - n) + p*z*D(getVariableExpr(x)) + q*D(getVariableExpr(x)))->toString();
    steps.push_back(QIODevice::tr(""));
    auto eq_res = solveDifur(subdifur, x, z->getId());
    for (auto &it : eq_res.second)
        steps.push_back(it);
    steps.push_back(QIODevice::tr(""));
    steps.push_back(QIODevice::tr("Провести обратную замену ") + formula(ys + " = (1/" + z->toString() + ")^" + toDegreeString(one/(n - one))));
    for (auto &it : eq_res.first)
    {
       // qDebug() << it.toString();
        if (it.getType() == DifurResult::SOLVED_FOR_Y)
            res.push_back(one/pow(getVariableExpr(y), n - one) - it.expr());
        else
        {
            auto expr = copy(it.expr());
            setUpExpressionIntoVariable(expr, one/pow(getVariableExpr(y), n - one), z->getId());
            res.push_back(std::move(expr));
        }
       // qDebug() << res.back()->toString();
    }

    return {std::move(res), steps};
}
//https://portal.tpu.ru/SHARED/s/SHERSTNEVA/Study_work/Studentam_ETO/Lecture_documents_diff_ur/Tab/4.%20Уравнения%20в%20полных%20дифференц.pdf
//https://1cov-edu.ru/differentsialnye-uravneniya/integriruyuschii_mnozhitel/
abs_ex tryToFindIntegratingMultiplicator(const abs_ex & M, const abs_ex & N, int x, int y)
{
    abs_ex dM_dy = M->derivative(y);
    abs_ex dN_dx = N->derivative(x);

    abs_ex dm_dn_sub = dM_dy - dN_dx;
    if (!((dm_dn_sub)/N)->hasVariable(y))
    {
        auto integr = integrate(one/N * (dm_dn_sub) * D(getVariableExpr(x)));
        if (integr == nullptr)
            return integr;
        return pow(getEuler(), integr);
    }
    if (!((dm_dn_sub)/M)->hasVariable(x))
    {
        auto integr = integrate(one/M * dm_dn_sub * D(getVariableExpr(y)));
        if (integr == nullptr)
            return integr;
        return pow(getEuler(), -integr);
    }

    auto isItFunctionOfSuchExpr = [x, y, &dm_dn_sub, &M, &N](const abs_ex & expr)->bool
    {
        auto f_expr = dm_dn_sub/(N*expr->antiderivative(x) - M*expr->antiderivative(y));
        auto u = systemVarExpr();
        auto eq_res = solveEquation(expr - u, x);
        if (eq_res.size() == 0)
            return false;
        for (auto &it : eq_res)
        {
            auto cop = copy(f_expr);
            setUpExpressionIntoVariable(cop, it, x);
            if (cop->hasVariable(x) || cop->hasVariable(y))
                return false;
        }
        return true;
    };
    auto getRes = [x, y, &dm_dn_sub, &M, &N](const abs_ex & expr)->abs_ex {
        auto f_expr = dm_dn_sub/(N*expr->antiderivative(x) - M*expr->antiderivative(y));
        auto u = systemVarExpr();
        auto eq_res = solveEquation(expr - u, x);
        setUpExpressionIntoVariable(f_expr, *eq_res.begin(), x);
        auto integr = integrate(f_expr * D(u));
        if (integr == nullptr)
            return nullptr;
        return pow(getEuler(), integr);
    };
    auto xv = getVariableExpr(x);
    auto yv = getVariableExpr(y);
    //qDebug() << xv->toString();
    //qDebug() << yv->toString();
    //qDebug() << (xv + yv)->toString();
    if (isItFunctionOfSuchExpr(xv + yv))
    {
        return getRes(xv + yv);
    }
    if (isItFunctionOfSuchExpr(xv * yv))
        return getRes(xv * yv);
    if (isItFunctionOfSuchExpr(xv*xv + yv*yv))
        return getRes(xv*xv + yv*yv);
    if (isItFunctionOfSuchExpr(xv/yv))
        return getRes(xv/yv);
    if (isItFunctionOfSuchExpr(pow(xv, 3) + pow(yv, 3)))
        return getRes(pow(xv, 3) + pow(yv, 3));
    if (isItFunctionOfSuchExpr(xv*xv + xv*yv + yv*yv))
        return getRes(xv*xv + xv*yv + yv*yv);
    if (isItFunctionOfSuchExpr(sqrt(xv) + sqrt(yv)))
        return getRes(sqrt(xv) + sqrt(yv));
    return nullptr;
}
std::pair<std::list<abs_ex>, std::vector<QString>> tryToSolveDifurInFullDifferential(const std::unique_ptr<Polynomial> & difur, int x, int y)
{
    //qDebug() << difur->toString();
    std::vector<QString> steps;
    QString xs = getVariable(x).toString();
    QString ys = getVariable(y).toString();
    abs_ex p = copy(zero);
    abs_ex q = copy(zero);
    auto monoms = difur->getMonoms();
    for (auto &it : *monoms)
    {
        if (it->hasDifferentialOfVarAsMultiplier(x))
        {
            if ((toAbsEx(it) / D(getVariableExpr(x)))->hasDifferential())
                return {std::list<abs_ex> (), steps};
            p = std::move(p) + toAbsEx(it)/D(getVariableExpr(x));
        }
        else if (it->hasDifferentialOfVarAsMultiplier(y))
        {
            if ((toAbsEx(it) / D(getVariableExpr(y)))->hasDifferential())
                return {std::list<abs_ex> (), steps};
            q = std::move(q) + toAbsEx(it)/D(getVariableExpr(y));
        }
        else
            return {std::list<abs_ex> (), steps};
    }
  //  qDebug() << p->toString();
  //  qDebug() << q->toString();
  //  qDebug() << p->derivative(y)->toString();
 //   qDebug() << q->derivative(x)->toString();
    steps.push_back(QIODevice::tr("Представить уравнение в виде: "));
    steps.push_back(formula(toMultString(p) + "d" + xs + " + " + toMultString(q) + "d" + ys + " = 0"));
    if (!subCompare(p->derivative(y), q->derivative(x)))
    {
        auto integr_mult = tryToFindIntegratingMultiplicator(p, q, x, y);
        if (integr_mult == nullptr)
            return {std::list<abs_ex> (), steps};
        steps.push_back(QIODevice::tr("Домножить уравнение на ") + formula(integr_mult->toString()));
        p = std::move(p) * integr_mult;
        q = std::move(q) * std::move(integr_mult);
    }
    steps.push_back(QIODevice::tr("Обозначить: "));
    steps.push_back(formula(unicode(420) + " = " + p->toString()));
    steps.push_back(formula(unicode(490) + " = " + q->toString()));
    steps.push_back(QIODevice::tr("Т. к. ") + formula("d" + unicode(420) + "/d" + ys + " = d" + unicode(490) + "/d" + xs + " = " + p->derivative(y)->toString()) +
                    ", это уравнение в полных дифференциалах");
    steps.push_back(QIODevice::tr("Пусть ") + formula("F") + QIODevice::tr(" - искомая функция. Тогда ") + formula(unicode(420) + " = " + "dF/d" + xs + ", " + unicode(490) + " = " + "dF/d" + ys));
    //здесь есть два зеркальных пути. идем сначала по одному, а если не выходит, то по второму

    abs_ex p_x_integral = integrate(p*D(getVariableExpr(x)));
    if (p_x_integral != nullptr)
    {
        steps.push_back(formula(QIODevice::tr("F = ") + unicode(8747) + "(" + p->toString() + ")d" + xs));
        steps.push_back(formula("F = " + p_x_integral->toString() + " + " + unicode(966) + "(" + ys + ")"));
        abs_ex p_y_deriv = p_x_integral->derivative(y);
        steps.push_back(formula("dF/d" + ys + " = " + "(" + p_x_integral->toString() + ")'" + " = " + p_y_deriv->toString() + " + " + unicode(966) + "'(" + ys + ")" + " = " + unicode(490)));
        abs_ex phita1 = q - p_y_deriv;
        steps.push_back(formula(p_y_deriv->toString() + " + " + unicode(966) + "'" + "(" + ys + ")" + " = " + q->toString()));
        steps.push_back(formula(unicode(966) + "'(" + ys + ") = " + phita1->toString()));

        steps.push_back(formula(unicode(966) + "(" + ys + ") = " + unicode(8747) + "(" + phita1->toString() + ")d" + ys));
        abs_ex phita_integral = integrate(phita1*D(getVariableExpr(y)));
        if (phita_integral != nullptr)
        {
            steps.push_back(formula(unicode(966) + "(" + ys + ") = " + phita_integral->toString()));
            auto F = std::move(p_x_integral) + std::move(phita_integral);
            steps.push_back(formula("F = " + F->toString() + " + C,   C = const"));
            std::list<abs_ex> res;
            res.push_back(std::move(F) + integratingConstantExpr());
            return {std::move(res), steps};
        }

    }

    abs_ex q_y_integral = integrate(q*D(getVariableExpr(y)));
    if (q_y_integral != nullptr)
    {
        steps.push_back(formula(QIODevice::tr("F = ") + unicode(8747) + "(" + q->toString() + ")d" + ys));
        steps.push_back(formula("F = " + q_y_integral->toString() + " + " + unicode(966) + "(" + xs + ")"));
        abs_ex q_x_deriv = q_y_integral->derivative(x);
        steps.push_back(formula("dF/d" + xs + " = " + "(" + q_y_integral->toString() + ")'" + " = " + q_x_deriv->toString() + " + " + unicode(966) + "'(" + xs + ")" + " = " + unicode(420)));
        abs_ex phita2 = p - q_x_deriv;
        steps.push_back(formula(q_x_deriv->toString() + " + " + unicode(966) + "'" + "(" + xs + ")" + " = " + p->toString()));
        steps.push_back(formula(unicode(966) + "'(" + xs + ") = " + phita2->toString()));

        steps.push_back(formula(unicode(966) + "(" + xs + ") = " + unicode(8747) + "(" + phita2->toString() + ")d" + xs));
        abs_ex phita_integral = integrate(phita2 * D(getVariableExpr(x)));
        if (phita_integral != nullptr)
        {
            steps.push_back(formula(unicode(966) + "(" + xs + ") = " + phita_integral->toString()));
            std::list<abs_ex> res;
            auto F = std::move(q_y_integral) + std::move(phita_integral);
            steps.push_back(formula("F = " + F->toString() + " + C,   C = const"));
            res.push_back(std::move(F) + integratingConstantExpr());
            return {std::move(res), steps};
        }
    }
    return {std::list<abs_ex>(), steps};

}


std::pair<std::list<abs_ex>, std::vector<QString>> solveFirstOrderDifurByMethods(const abs_ex & difur, int x, int y)
{
    auto dif_pol = toPolynomialPointer(difur);
    bool is_that_type_difur = false;
    auto res = tryToSolveDifurWithSeparableVariables(dif_pol, x, y, is_that_type_difur);
    if (is_that_type_difur)
        return res;
    res = tryToSolveFirstOrderHomoheneousDifur(copy(difur), x, y);
    if (res.first.size() > 0)
        return res;
    res = tryToSolveHeterogeneousDifur(dif_pol, x, y);
    if (res.first.size() > 0)
        return res;
    res = tryToSolveBernullyEquation(toPolynomialPointer(difur), x, y);
    if (res.first.size() > 0)
        return res;
    res = tryToSolveDifurInFullDifferential(toPolynomialPointer(difur), x, y);
    if (res.first.size() > 0)
        return res;
    return {std::list<abs_ex> (), std::vector<QString>()};
}

std::pair<std::list<abs_ex>, std::vector<QString>> tryToSolveDifurByReintegration(const abs_ex & difur, int x, int y)
{
    std::vector<QString> steps;
    QString xs = getVariable(x).toString();
    QString ys = getVariable(y).toString();
    bool can_solve_by_reintegration = true;
    int order = getOrderOfHighOrderEquation(difur, x, y);
    difur->doSomethingInDerivativeObject([&can_solve_by_reintegration, order](int, int, int ord){
        if (ord != order)
            can_solve_by_reintegration = false;
    });
    if (!can_solve_by_reintegration)
        return {std::list<abs_ex> (), steps};

    abs_ex cop = copy(difur);
    abs_ex t = systemVarExpr();
    cop->changeSomePartOn(derivative(getVariableExpr(y), x, order)->makeStringOfExpression(), t);

    auto eq_res = solveEquation(cop, t->getId());
    steps.push_back(QIODevice::tr("Выразить производную и представить уравнение в виде:"));
    if (eq_res.size() == 1)
    {
        steps.push_back(formula(derivative(getVariableExpr(y), x, order)->toString() + " = " + eq_res.begin()->get()->toString()));
    }
    else if (eq_res.size() > 0)
    {
        for (auto &it : eq_res)
            steps.push_back(formula("[" + derivative(getVariableExpr(y), x, order)->toString() + " = " + it->toString()));
    }
    steps.push_back(QIODevice::tr("Проинтегрировать уравнение ") + formula(QString::number(order)) + " раз:");

    std::list<abs_ex> res;
    for (auto &it : eq_res)
    {
        auto integr = std::move(it);
        for (int i = 0; i < order && integr != nullptr; ++i)
        {
            integr = integrate(integr * D(getVariableExpr(x)));
            if (integr != nullptr)
            {
                integr = std::move(integr) + integratingConstantExpr();
                steps.push_back(formula(derivative(getVariableExpr(y), x, order - i - 1)->toString() + " = " + copyWithHtmlConstantsIndexes( integr)->toString()));
            }
        }
        if (integr != nullptr)
            res.push_back(getVariableExpr(y) - std::move(integr));
    }
    return {std::move(res), steps};
}

std::pair<std::list<abs_ex>, std::vector<QString>> tryToSolveHighOrderDifurByLoweringOrder(const abs_ex & difur, int x, int y)
{
    std::vector<QString> steps;
    QString xs = getVariable(x).toString();
    QString ys = getVariable(y).toString();
    int min_order = std::numeric_limits<int>::max();
  //  qDebug() << difur->toString();
    difur->doSomethingInDerivativeObject([&min_order, y](int var, int, int order) {
        if (var == y)
             min_order = std::min(min_order, order);
    });
    if (min_order == 0)
        return {std::list<abs_ex> (), steps};

    abs_ex z = systemVarExpr();
    z->setName(unicode(949));
    steps.push_back(QIODevice::tr("Сделать замену ") + formula(z->toString() + " = " + derivative(getVariableExpr(y), x, min_order)->toString()));

    int max_order = getOrderOfHighOrderEquation(difur, x, y);
    abs_ex cop = copy(difur);
    for (int i = min_order; i <= max_order; ++i)
    {
        abs_ex change = derivative(z, x, i - min_order);
        cop->changeSomePartOn(derivative(getVariableExpr(y), x, i)->toString(), change);
    }
    cop->setSimplified(false);
    cop = cop->downcast();

    auto pre_res = solveDifurInCommonIntegral(cop, x, z->getId(), steps);
    steps.push_back(QIODevice::tr("Решения: "));
    for (auto &it : pre_res)
        steps.push_back(formula(copyWithHtmlConstantsIndexes(  it)->toString() + " = 0"));
    steps.push_back(QIODevice::tr("Сделать обратную замену"));

    std::list<abs_ex> res;
    for (auto &it : pre_res)
    {
        setUpExpressionIntoVariable(it, derivative(getVariableExpr(y), x, min_order), z->getId());
        steps.push_back(formula(it->toString() + " = 0"));
        steps.push_back("");
        res.splice(res.end(), solveDifurInCommonIntegral(it, x, y, steps));
        steps.push_back("");
    }
    return {std::move(res), steps};
}

std::pair<std::list<abs_ex>, std::vector<QString>> tryToSolveHighOrderDifurWithoutX(const abs_ex & difur, int x, int y)
{
    std::vector<QString> steps;
    QString xs = getVariable(x).toString();
    QString ys = getVariable(y).toString();
    if (difur->hasVariable(x))
        return {std::list<abs_ex> (), steps};

    if (getOrderOfHighOrderEquation(difur,x ,y) > 2)
        return {std::list<abs_ex> (), steps};

    abs_ex cop = copy(difur);

    abs_ex z = systemVarExpr();
    z->setName(unicode(946));
    steps.push_back("Сделать замену " + formula(ys + "' = " + z->toString() + "(" + ys + ")"));
    steps.push_back("Тогда " + formula(ys + "'' = " + z->toString() + "'"  + z->toString()));

    cop->changeSomePartOn(derivative(getVariableExpr(y), x, 1)->toString(), z);
    abs_ex z_der = derivative(z, y, 1)*z;
    cop->changeSomePartOn(derivative(getVariableExpr(y), x, 2)->toString(), z_der);
   // qDebug() << cop->toString();
    steps.push_back("Решить получившееся уравнение относительно " + formula(ys + " и " + z->toString()));
    auto pre_res = solveDifurInCommonIntegral(cop, y, z->getId(), steps);

    std::list<abs_ex> res;
    for (auto &it : pre_res)
    {
        //qDebug() << it->toString();
        steps.push_back(QIODevice::tr("В выражении ") + formula(it->toString() + " = 0 ") + QIODevice::tr("выполнить обратную замену"));

        setUpExpressionIntoVariable(it, D(getVariableExpr(y))/D(getVariableExpr(x)), z->getId());

        steps.push_back(formula(it->toString() + " = 0"));
        steps.push_back(QIODevice::tr("Решить получившееся уравнение"));
        steps.push_back("");

        res.splice(res.end(), solveDifurInCommonIntegral(it, x, y, steps));

        steps.push_back("");
    }
    return {std::move(res), steps};
}
std::pair<std::list<abs_ex>, std::vector<QString>> tryToSolveHighOrderLinearHomogeneousDifur(const abs_ex & difur, int x, int y)
{
    std::vector<QString> steps;
    QString xs = getVariable(x).toString();
    QString ys = getVariable(y).toString();

    int order = getOrderOfHighOrderEquation(difur, x, y);
    std::vector<abs_ex> coefs(order + 1);
    for (auto &it : coefs)
        it = copy(zero);
    auto difur_pol = toPolynomialPointer(difur);
    auto monoms = difur_pol->getMonoms();
    for (auto &it : *monoms)
    {
        if (!it->hasVariable(y) || it->hasVariable(x))
            return {std::list<abs_ex> (), steps};
        auto frac_without_y = it->getFractalWithoutVariable(y);
        auto frac_with_y = toAbsEx(it)/toAbsEx(frac_without_y);
        if (frac_with_y->getId() == y)
            coefs[0] = std::move(coefs[0]) + std::move(frac_without_y);
        else if (frac_with_y->getId() == DERIVATIVE_OBJECT)
        {
            int ord  = getOrderOfHighOrderEquation(frac_with_y, x, y);
            coefs[ord] = std::move(coefs[ord]) + std::move(frac_without_y);
        }
        else
            return {std::list<abs_ex> (), steps};
    }
    steps.push_back(QIODevice::tr("Представить уравнение в следующем виде:"));
    QString equation_view;
    for (int i = order; i >= 0; --i)
        equation_view += toMultString(coefs[i]) + derivative(getVariableExpr(y), x, i)->toString() + (i == 0 ? "" : " + ");
    steps.push_back(formula(equation_view + " = 0"));
    steps.push_back(QIODevice::tr("Тип: однородное уравнение ") + formula(QString::number(order)) + QIODevice::tr("-го порядка"));

    auto t = systemVarExpr();
    t->setName(unicode(968));

    abs_ex charasteristic_equation = copy(zero);
    for (int i = 0; i < coefs.size(); ++i)
        charasteristic_equation = std::move(charasteristic_equation) + coefs[i] * pow(t, i);

    steps.push_back(QIODevice::tr("Составить характеристическое уравнение:"));
    QString charachteristic_equation_string;
    for (int i = order; i >= 0; --i)
        charachteristic_equation_string += toMultString(coefs[i]) + pow(t, i)->toString() + (i == 0 ? "" : " + ");
    steps.push_back(formula(charachteristic_equation_string + " = 0"));
    steps.push_back(QIODevice::tr("Решить характеристическое уравнение относительно ") + formula(t->toString()));
 //   qDebug() << charasteristic_equation->toString();

    auto roots = solvePolynomialEquationInComplexNumber(charasteristic_equation, t->getId());
    if (!roots.second)
        return {std::list<abs_ex> (), steps};

    abs_ex result = copy(zero);
    abs_ex x_var = getVariableExpr(x);
    for (auto &it : roots.first)
    {
        steps.push_back(formula(t->toString() + " = " + it.first.toString() + "       |x" + QString::number(it.second)));
        for (int i = 0; i < it.second; ++i)
        {
           // qDebug() << result->toString();
           // qDebug() << (integratingConstantExpr() * pow(getEuler(), it.first.a()*x_var) * pow(x_var, i))->toString();
            if (it.first.isReal())
                result = std::move(result) + integratingConstantExpr() * pow(getEuler(), it.first.a()*x_var) * pow(x_var, i);
            else
                result = std::move(result) + pow(getEuler(), it.first.a()*x_var) * pow(x_var, i) *
                        (integratingConstantExpr() * sin(it.first.b() * x_var) + integratingConstantExpr() * cos(it.first.b() * x_var));
        }
    }
    steps.push_back(QIODevice::tr("Построить ответ из корней характеристического уравнения:"));
    steps.push_back(formula(ys + " = " + copyWithHtmlConstantsIndexes(result)->toString()));
    result = getVariableExpr(y) - std::move(result);
    std::list<abs_ex> res;
    res.push_back(std::move(result));
    return {std::move(res), steps};
}
std::pair<std::list<abs_ex>, std::vector<QString>> tryToSolveHighOrderHeterogeneousDifur(const abs_ex & difur, int x, int y)
{
    std::vector<QString> steps;
    QString xs = getVariable(x).toString();
    QString ys = getVariable(y).toString();

    int order = getOrderOfHighOrderEquation(difur, x, y);
    std::vector<abs_ex> coefs(order + 1);
    for (auto &it : coefs)
        it = copy(zero);
    auto difur_pol = toPolynomialPointer(difur);
    auto monoms = difur_pol->getMonoms();
    for (auto it = monoms->begin(); it != monoms->end(); ++it)
    {
        if (it->get()->hasVariable(x) && it->get()->hasVariable(y))
        {
            auto frac_with_x = *it /  it->get()->getFractalWithoutVariable(x);
           // qDebug() << it->get()->toString();
           // qDebug() << it->get()->getFractalWithoutVariable(x)->toString();
           // qDebug() << frac_with_x->toString();
            if (frac_with_x->hasVariable(y))
                return {std::list<abs_ex> (), steps};
            for (auto &it : *monoms)
                it = it / frac_with_x;
            break;
        }
    }
    auto f_x = copy(zero);
    auto related_homogeneous_difur = copy(zero);
    for (auto &it : *monoms)
    {
        if (it->hasVariable(y))
        {
            if (it->hasVariable(x))
                return {std::list<abs_ex> (), steps};
            auto frac_without_y = it->getFractalWithoutVariable(y);
            auto frac_with_y = toAbsEx(it)/toAbsEx(frac_without_y);
            if (frac_with_y->getId() == y)
                coefs[0] = std::move(coefs[0]) + std::move(frac_without_y);
            else if (frac_with_y->getId() == DERIVATIVE_OBJECT)
            {
                int ord  = getOrderOfHighOrderEquation(frac_with_y, x, y);
                coefs[ord] = std::move(coefs[ord]) + std::move(frac_without_y);
            }
            else
                return {std::list<abs_ex> (), steps};
            related_homogeneous_difur = std::move(related_homogeneous_difur) + toAbsEx(it);
        }
        else
        {
            f_x = std::move(f_x) + toAbsEx(it);
        }
    }

    //a_0 * y''.. + a_1 * ...... + a_n * y = f_x
    f_x = -std::move(f_x);
    steps.push_back(QIODevice::tr("Представить уравнение в виде:"));
    steps.push_back(formula(related_homogeneous_difur->toString() + " = " + f_x->toString()));
    steps.push_back(QIODevice::tr("Тип: неоднородное уравнение ") + formula(QString::number(order)) + QIODevice::tr("-го порядка"));
    steps.push_back(QIODevice::tr("Решить соответствующее однородное дифференциальное уравнение:"));
    steps.push_back("");
    auto common_solving_list = solveDifur(related_homogeneous_difur, x, y);
    for (auto &it : common_solving_list.second)
        steps.push_back(it);
    steps.push_back("");
    steps.push_back("");

    assert(common_solving_list.first.size() == 1 &&
           common_solving_list.first.begin()->getType() == DifurResult::SOLVED_FOR_Y);
    auto common_solving_expr = copy(common_solving_list.first.begin()->expr());
    assert(common_solving_expr->getId() == POLYNOMIAL || common_solving_expr->getId() == FRACTAL);
    std::unique_ptr<Polynomial> common_solving;
    if (common_solving_expr->getId() == POLYNOMIAL)
        common_solving = toPolynomialPointer(common_solving_expr);
    else
    {
        auto fr = static_cast<Fractal*>(common_solving_expr.get());
        assert(fr->canTurnIntoPolynomWithOpeningParentheses(true));
        common_solving = fr->turnIntoPolynomWithOpeningParentheses(true);
    }
  //  qDebug()<< common_solving->toString();
    std::vector<abs_ex> y_coefs (order);
    for (auto &it : y_coefs)
        it = copy(zero);
    int y_coefs_counter = 0;
    std::map<int, int> integrating_constants_to_y_coefs_indexes;
    for (auto &it : *common_solving->getMonoms())
    {

        auto set = it->getSetOfVariables();
        assert(set.size() > 0 && isIntegratingConstant(*set.rbegin()));
        if (set.size() > 0 && isIntegratingConstant(*(++set.rbegin())))
            assert(false);
        abs_ex it_without_mult = toAbsEx(it)/getVariableExpr(*set.rbegin());
        assert(!it_without_mult->hasVariable(*set.rbegin()));
        int vec_index;
        auto dict_it = integrating_constants_to_y_coefs_indexes.find(*set.rbegin());
        if (dict_it == integrating_constants_to_y_coefs_indexes.end())
        {
            vec_index = y_coefs_counter;
            ++y_coefs_counter;
            integrating_constants_to_y_coefs_indexes.insert({*set.rbegin(), vec_index});
        }
        else
            vec_index = dict_it->second;
        y_coefs[vec_index] = std::move(y_coefs[vec_index]) + std::move(it_without_mult);
    }
    std::vector<int> variables(order);
    for (auto &it : variables)
        it = systemVarExpr()->getId();
    for (int i = 0; i < variables.size(); ++i)
        getVariable(variables[i]).setName(unicode(955) + "<sub>" + QString::number(i + 1) + "</sub>" + "'");

    std::vector<abs_ex> equations(order);
    for (auto &it : equations)
        it = copy(zero);
    std::vector<abs_ex> coefs_derivs(order);
    for (int i = 0; i < coefs_derivs.size(); ++i)
        coefs_derivs[i] = copy(y_coefs[i]);
    for (int i = 0; i < order; ++i)
    {
        for (int j = 0; j < order; ++j)
        {
            equations[i] = std::move(equations[i]) + getVariableExpr(variables[j])*coefs_derivs[j];
            coefs_derivs[j] = coefs_derivs[j]->derivative(x);
        }
        if (i == order - 1)
            equations[i] = std::move(equations[i]) - f_x/coefs[order];
    }

    //for (auto &it : equations)
     //   qDebug() << it->toString();
    steps.push_back(QIODevice::tr("Решение соответствующего однородного уравнения:"));
    abs_ex out_cop = licCopy(common_solving_expr);
    downgradeIntegratingConstantsIndexes(out_cop);
    steps.push_back(formula(ys + " = " + out_cop->toString()));
    steps.push_back(QIODevice::tr("В получившимся решении однородного уравнения заменить константы интегрирования на производные неизвестных функции ") + formula(unicode(955) +
                    "<sub>1</sub> ... " + unicode(955) + "<sub>" + QString::number(order) + "</sub>"));
    auto out_cop_set = out_cop->getSetOfVariables();
    int counter = 0;
    for (auto &it : out_cop_set)
        if (isIntegratingConstant(it))
            setUpExpressionIntoVariable(out_cop, getVariableExpr(variables[counter++]), it);
    steps.push_back(formula(ys + " = " + out_cop->toString()));
    steps.push_back(QIODevice::tr("Для нахождения этих функций составить систему уравнений:"));
    for (auto &it : equations)
        steps.push_back(formula("{  " + it->toString() + " = 0"));
    steps.push_back(QIODevice::tr("Решить систему линейных уравнений относительно производных функций ") + formula(unicode(955)));

    auto equations_result = solveSystemOfEquations(equations, variables);
    for (int i = 0; i < equations_result.size(); ++i)
        steps.push_back(formula(getVariable(variables[i]).toString() + " = " + equations_result[i][0]->toString()));
    steps.push_back(QIODevice::tr("Найти функции ") + formula(unicode(955)) + QIODevice::tr(" интегрированием"));

    if (equations_result.size() == 0)
        return {std::list<abs_ex> (), steps};
    auto answer = copy(zero);
    for (int i = 0; i < order; ++i)
    {

       // qDebug() << equations_result[i][0]->toString();
        auto integr = integrate(equations_result[i][0]*D(getVariableExpr(x)));

       // qDebug() << integr->toString();
        if (integr == nullptr)
            return {std::list<abs_ex> (), steps};

        steps.push_back(formula(unicode(955) + "<sub>" + QString::number(i + 1) + "</sub>" + " = " + unicode(8747) + "(" + equations_result[i][0]->toString() + ")d" + xs +
                " = " + integr->toString() + " + C,      C = const"));
        answer = std::move(answer) + y_coefs[i] * (std::move(integr) + integratingConstantExpr());
    }
    steps.push_back(QIODevice::tr("Подставить функции ") + formula(unicode(955)) + " в ответ:");
    steps.push_back(formula(ys + " = " + answer->toString()));

    answer = getVariableExpr(y) - answer;
    std::list<abs_ex> res;
    res.push_back(std::move(answer));
    return {std::move(res), steps};

}
std::pair<std::list<abs_ex>, std::vector<QString>> solveHighOrderDifurByMethods(const abs_ex & difur, int x, int y)
{
    auto res = tryToSolveDifurByReintegration(difur, x, y);
    if (res.first.size() > 0)
        return res;

     res = tryToSolveHighOrderLinearHomogeneousDifur(difur, x, y);
     if (res.first.size() > 0)
         return res;


     res = tryToSolveHighOrderDifurByLoweringOrder(difur, x, y);
     if (res.first.size() > 0)
         return res;
     res = tryToSolveHighOrderHeterogeneousDifur(difur, x, y);
     if (res.first.size() > 0)
         return res;

     res = tryToSolveHighOrderDifurWithoutX(difur, x, y);
     if (res.first.size() > 0)
         return res;


    return {std::list<abs_ex>(), std::vector<QString>()};
}






std::list<abs_ex>  solveDifurInCommonIntegral(const abs_ex & difur, int x, int y, std::vector<QString> & steps)
{
    QString xs = getVariable(x).toString();
    QString ys = getVariable(y).toString();

    std::list<abs_ex> res;
    if (isZero(difur))
    {
        res.push_back(copy(zero));
        return res;
    }
    if (difur->getId() > 0)
    {
        res.push_back(copy(difur));
        return res;
    }
    if (difur->getId() == NUMBER || difur->getId() == CONSTANT)
        return std::list<abs_ex> ();
    if (difur->getId() == DIFFERENTIAL)
    {
        assert(difur->getSetOfVariables().size() == 1);
        int var = *difur->getSetOfVariables().begin();
        if (var != x && var != y)
        {
            res.push_back(copy(zero));
            return res;
        }
        steps.push_back(QIODevice::tr("Решить уравнение ") + formula(difur->toString() + " = 0"));
        steps.push_back(formula(unicode(8747) + difur->toString() + " = " + unicode(8747) + "0"));
        steps.push_back(formula(getVariable(var).toString() + " =  C,   C = const"));

        res.push_back(getVariableExpr(var) - integratingConstantExpr());
        return res;
    }
    if (difur->getId() == DERIVATIVE_OBJECT && isEquationOfAppropriateForm(difur, x, y))
    {
        steps.push_back(QIODevice::tr("Решить уравнение ") + formula(difur->toString() + " = 0") + ":");
        int order = getOrderOfHighOrderEquation(difur, x, y);
        steps.push_back(QIODevice::tr("Проинтегрировать правую часть ") + formula(QString::number(order)) + QIODevice::tr(" раз"));
        abs_ex integr = copy(zero);
        for (int i = 0; i < order; ++i)
        {
            integr = integr->antiderivative(x) + integratingConstantExpr();
            steps.push_back(formula(derivative(getVariableExpr(y), x, order - i - 1)->toString() + " = " + copyWithHtmlConstantsIndexes(integr)->toString()));
        }
        res.push_back(getVariableExpr(y) - std::move(integr));
        return res;
    }
    if (difur->getId() == FRACTAL)
    {
        auto fr = static_cast<Fractal*>(difur.get())->getFractal();
        DifursRootConditions conditions;
        for (auto &it : *fr.second)
        {
            conditions.addCondition(DifEquationRootCondition(it, x, y));
            steps.push_back(QIODevice::tr("Условие ") + formula(it->toString() + " " + unicode(8800) + " 0"));
        }
        for (auto &it : *fr.first)
            res.splice(res.end(), solveDifurInCommonIntegral(it, x, y, steps, conditions));
        return res;
    }

    steps.push_back(QIODevice::tr("Решить дифференциальное уравнение ") + formula(difur->toString() + " = 0"));


    if (difur->getId() == DEGREE)
        return solveDifurInCommonIntegral(copy(Degree::getArgumentOfDegree(difur.get())), x, y, steps,
                                          DifEquationRootCondition(Degree::getDegreeOfExpression(difur.get()),
                                                                   x, y));
    if (difur->getId() == ABSOLUTE_VALUE || difur->getId() == ARCTANGENT || difur->getId() == ARCSINUS)
        return solveDifurInCommonIntegral(getArgumentOfFunction(difur), x, y, steps);
    if (difur->getId() == SINUS)
        return solveDifurInCommonIntegral(getArgumentOfFunction(difur) - getPi()*systemVarExpr(), x, y, steps);
    if (difur->getId() == COSINUS)
        return solveDifurInCommonIntegral(getArgumentOfFunction(difur) - getPi()/two - getPi()*systemVarExpr(), x, y, steps);
    if (difur->getId() == TANGENT)
        return solveDifurInCommonIntegral(getArgumentOfFunction(difur) - getPi()*systemVarExpr(), x, y, steps);
    if (difur->getId() == COTANGENT)
        return solveDifurInCommonIntegral(getArgumentOfFunction(difur) - getPi()/two - getPi()*systemVarExpr(), x, y, steps);
    if (difur->getId() == LOGARITHM)
        return solveDifurInCommonIntegral(getArgumentOfFunction(difur) - one, x, y, steps);
    if (isEquationOfAppropriateForm(difur, x, y))
    {
        if (isItFirstOrderExpression(difur, x, y))
        {
          //  qDebug() << difur->toString();
            if (hasDerivativeObject(difur, x, y))
            {
                steps.push_back(QIODevice::tr("Представить производную в дифференциальном виде ") + formula(ys + "' = d" + ys + "/d" + xs));
                abs_ex y_der = D(getVariableExpr(y))/D(getVariableExpr(x));
                difur->changeSomePartOn(derivative(getVariableExpr(y), x, 1)->makeStringOfExpression(), y_der);
                difur->setSimplified(false);
                difur->simplify();
               // qDebug() << difur->toString();
                return solveDifurInCommonIntegral(difur->downcast(), x, y, steps);
            }
            auto solv = solveFirstOrderDifurByMethods(difur, x, y);
            for (auto &it : solv.second)
                steps.push_back(std::move(it));
            return std::move(solv.first);
        }
        auto solv = solveHighOrderDifurByMethods(difur, x, y);
        for (auto &it : solv.second)
            steps.push_back(std::move(it));
        return std::move(solv.first);
    }
    abs_ex difur_with_try_to_change_differentials = copy(difur);
    abs_ex dy_on_change = D(getVariableExpr(x))*derivative(getVariableExpr(y), x, 1);
    difur_with_try_to_change_differentials->changeSomePartOn(D(getVariableExpr(y))->makeStringOfExpression(),
                                                             dy_on_change);
    difur_with_try_to_change_differentials = (std::move(difur_with_try_to_change_differentials)/D(getVariableExpr(x))) +
            zero;
    if (isEquationOfAppropriateForm(difur, x, y))
    {
        steps.push_back(QIODevice::tr("Поделить уравнение на dx и записать дробь ") + formula("d" + ys + "/d" + xs + " в виде " + ys + "'"));
        auto solv = solveHighOrderDifurByMethods(difur, x, y);
        for (auto &it : solv.second)
            steps.push_back(std::move(it));
        return std::move(solv.first);
    }
    return std::list<abs_ex>();
}
std::list<abs_ex> solveDifurInCommonIntegral(const abs_ex & difur, int x, int y, std::vector<QString> & steps,
                                             const DifursRootConditions & conditions)
{
    auto preres = solveDifur(difur, x, y);
    for (auto &it : preres.second)
        steps.push_back(it);
    std::list<abs_ex> res;
  //  qDebug() << difur->makeStringOfExpression();
    for (auto &it : preres.first)
    {

     //   qDebug() << it.toString();
        if (conditions.check(it))
            res.push_back(it.toCommonIntegral(x, y));
    }
    return  res ;
}
FunctionRange getRangeOfConstantAddictivesThatCanBeChangedAndTakeThemAway(AbstractExpression * expr)
{
    assert(expr->getId() == POLYNOMIAL);
    if (expr->getId() == POLYNOMIAL)
    {
        auto monoms = static_cast<Polynomial*>(expr)->getMonomialsPointers();

        if (isIntegratingConstantAndCanChangeIt(monoms.back()->getId()))
        {
            auto res = monoms.back()->getRange();
            static_cast<Polynomial*>(expr)->getMonoms()->erase(--static_cast<Polynomial*>(expr)->getMonoms()->end());
            return res;

        }
        FunctionRange range;
        bool initialized = false;
        auto monoms_p = static_cast<Polynomial*>(expr)->getMonoms();
        for (auto it = monoms_p->begin(); it != monoms_p->end();)
        {
            if (it->get()->getSetOfVariables().empty())
            {
                if (initialized)
                    range = rangeOfSum(range, it->get()->getRange());
                else
                {
                    range = it->get()->getRange();
                    initialized = true;
                }
                it = monoms_p->erase(it);
            }
            else
                ++it;
        }
        if (initialized)
            return range;
        else
            return FunctionRangeSegment(zero, zero, true, true);

    }
    return FunctionRangeSegment(zero, zero, true, true);
}
FunctionRange getRangeOfConstantAddictivesThatCanBeChangedAndTakeThemAway(abs_ex & expr)
{
    if (isIntegratingConstantAndCanChangeIt(expr->getId()) || expr->getSetOfVariables().empty())
    {
        auto res = expr->getRange();
        expr = copy(zero);
        return res;
    }
    expr = expr + zero;
    if (expr->getId() == POLYNOMIAL)
    {
        auto monoms = static_cast<Polynomial*>(expr.get())->getMonomialsPointers();

        if (isIntegratingConstantAddictiveThatCanBeChanged(monoms.back()))
        {
            auto res = monoms.back()->getRange();
            static_cast<Polynomial*>(expr.get())->getMonoms()->erase(--static_cast<Polynomial*>(expr.get())->getMonoms()->end());
            return res;

        }
        FunctionRange range;
        bool initialized = false;
        auto monoms_p = static_cast<Polynomial*>(expr.get())->getMonoms();
        for (auto it = monoms_p->begin(); it != monoms_p->end();)
        {
            if (it->get()->getSetOfVariables().empty())
            {
                if (initialized)
                    range = rangeOfSum(range, it->get()->getRange());
                else
                {
                    range = it->get()->getRange();
                    initialized = true;
                }
                it = monoms_p->erase(it);
            }
            else
                ++it;
        }
        if (initialized)
            return range;
        else
            return FunctionRangeSegment(zero, zero, true, true);

    }
    return FunctionRangeSegment(zero, zero, true, true);
}
FunctionRange getRangeOfConstantMultipliersThatCanBeChanged(const AbstractExpression * expr)
{
    assert(expr->getId() == FRACTAL);

    if (expr->getId() == FRACTAL)
    {
        auto fr = static_cast<const Fractal*>(expr)->getFractal();
        if (fr.first->empty())
            return FunctionRange(one, one, true, true);
        if (isIntegratingConstantAndCanChangeIt(fr.first->back()->getId()))
        {
            auto res = fr.first->back()->getRange();
            //fr.first->erase(--fr.first->end());
            return res;
        }
        bool initialized = false;
        Number coe = static_cast<const Fractal*>(expr)->getCoefficient();
        FunctionRange range = coe.getRange();
        initialized =  true;
        for (auto it = fr.first->begin(); it != fr.first->end();)
        {
            if (it->get()->getSetOfVariables().empty())
            {
                if (initialized)
                    range = rangeOfMultiplication(range, it->get()->getRange());
                else
                {
                    range = it->get()->getRange();
                    initialized = true;
                }
                //it = fr.first->erase(it);
                ++it;
            }
            else
                ++it;
        }
        for (auto it = fr.second->begin(); it != fr.second->end();)
        {
            if (it->get()->getSetOfVariables().empty())
            {
                if (initialized)
                    range = rangeOfDivision(range, it->get()->getRange());
                else
                {
                    range = it->get()->getRange();
                    initialized = true;
                }
               // it = fr.second->erase(it);
                ++it;
            }
            else
                ++it;
        }
        if (initialized)
            return range;
        else
            return FunctionRange(one, one, true, true);
    }
    return FunctionRange(one, one, true, true);
}
FunctionRange getRangeOfConstantMultipliersThatCanBeChanged(const abs_ex & expr)
{
    return getRangeOfConstantMultipliersThatCanBeChanged(expr.get());
}
FunctionRange getRangeOfConstantMultipliersThatCanBeChangedAndTakeThemAway(AbstractExpression * expr)
{
    assert(expr->getId() == FRACTAL);

    if (expr->getId() == FRACTAL)
    {
        auto fr = static_cast<Fractal*>(expr)->getFractal();
        if (fr.first->empty() && fr.second->empty())
        {
            Number coe = static_cast<const Fractal*>(expr)->getCoefficient();
            FunctionRange range = coe.getRange();
            static_cast<Fractal*>(expr)->setCoefficinet(1);
            return range;
        }
        if (isIntegratingConstantAndCanChangeIt(fr.first->back()->getId()))
        {
            auto res = fr.first->back()->getRange();
            fr.first->erase(--fr.first->end());
            return res;
        }
        bool initialized = false;
        Number coe = static_cast<const Fractal*>(expr)->getCoefficient();
        FunctionRange range = coe.getRange();
        initialized =  true;
        static_cast<Fractal*>(expr)->setCoefficinet(1);
        for (auto it = fr.first->begin(); it != fr.first->end();)
        {
            if (it->get()->getSetOfVariables().empty())
            {
                if (initialized)
                    range = rangeOfMultiplication(range, it->get()->getRange());
                else
                {
                    range = it->get()->getRange();
                    initialized = true;
                }
                it = fr.first->erase(it);
            }
            else
                ++it;
        }
        for (auto it = fr.second->begin(); it != fr.second->end();)
        {
            if (it->get()->getSetOfVariables().empty())
            {
                if (initialized)
                    range = rangeOfDivision(range, it->get()->getRange());
                else
                {
                    range = it->get()->getRange();
                    initialized = true;
                }
                it = fr.second->erase(it);
            }
            else
                ++it;
        }
        if (initialized)
            return range;
        else
            return FunctionRange(one, one, true, true);
    }
    return FunctionRange(one, one, true, true);
}
FunctionRange getRangeOfConstantMultipliersThatCanBeChangedAndTakeThemAway(abs_ex & expr)
{
    if (isIntegratingConstantAndCanChangeIt(expr->getId()) || expr->getSetOfVariables().empty())
    {
        auto res = expr->getRange();
        expr = copy(zero);
        return res;
    }
    if (expr->getId() == FRACTAL)
    {
        auto fr = static_cast<Fractal*>(expr.get())->getFractal();
        if (isIntegratingConstantAndCanChangeIt(fr.first->back()->getId()))
        {
            auto res = fr.first->back()->getRange();
            fr.first->erase(--fr.first->end());
            return res;
        }
        bool initialized = false;
        FunctionRange range;
        for (auto it = fr.first->begin(); it != fr.first->end();)
        {
            if (it->get()->getSetOfVariables().empty())
            {
                if (initialized)
                    range = rangeOfMultiplication(range, it->get()->getRange());
                else
                {
                    range = it->get()->getRange();
                    initialized = true;
                }
                it = fr.first->erase(it);
            }
            else
                ++it;
        }
        for (auto it = fr.second->begin(); it != fr.second->end();)
        {
            if (it->get()->getSetOfVariables().empty())
            {
                if (initialized)
                    range = rangeOfDivision(range, it->get()->getRange());
                else
                {
                    range = it->get()->getRange();
                    initialized = true;
                }
                it = fr.second->erase(it);
            }
            else
                ++it;
        }
        if (initialized)
            return range;
        else
            return FunctionRange(one, one, true, true);
    }
    return FunctionRange(one, one, true, true);
}
abs_ex copyWithLiftingIntegrateConstantsIndex(const abs_ex & expr)
{
    abs_ex res = copy(expr);
    auto set = expr->getSetOfVariables();
    for (auto &it : set)
    {
        if (isIntegratingConstant(it))
            setUpExpressionIntoVariable(res, integratingConstantExpr(VariablesDistributor::getVariablesDefinition(it)->getRange()),
                                        it);
    }
    return res;
}
void uniteSameResults(std::list<abs_ex> & list)
{

    for (auto it = list.begin(); it != list.end(); ++it)
    {
        for (auto it1 = next(it); it1 != list.end();)
        {
            //qDebug() << it->get()->toString();
            //qDebug() << it1->get()->toString();
           // auto sub = *it - *it1;
            //auto sub = copyWithLiftingIntegrateConstantsIndex(*it) -
            //        copyWithLiftingIntegrateConstantsIndex(*it1);
            //if (isZero(sub))
            //{
             //   it1 = list.erase(it1);
             //   continue;
            //}
           // AbsExMemoryChecker::enableWritingMemory();
            if (subCompare(*it, *it1))
            {
                it1 = list.erase(it1);
                continue;
            }
           // AbsExMemoryChecker::debugObjectsInMemory();
           // AbsExMemoryChecker::clear();
           //   qDebug() << amountOfIntegratingConstant(21);
           //   qDebug() << amountOfIntegratingConstant(24);
          //  qDebug() << it->get()->toString();
          //  qDebug() << it1->get()->toString();
           // qDebug() << VariablesDistributor::amountOfVariable(1500000000);
           // if (isIntegratingConstantAndCanChangeIt(sub->getId()))
            //{
             //   FunctionRange range1 = getRangeOfConstantAddictivesThatCanBeChangedAndTakeThemAway(*it);
             //   FunctionRange range2 = getRangeOfConstantAddictivesThatCanBeChangedAndTakeThemAway(*it1);
             //   *it = *it + integratingConstantExpr(unification(range1, range2));
             //   it1 = list.erase(it1);
            //}//они не могут быть одновременно нулями, это сверху раскрывается
            if (isZero(*it) || isZero(*it1))
            {
                if (isZero(*it))
                    std::swap(*it, *it1);
                if (it->get()->getId() == FRACTAL && !static_cast<Fractal*>(it->get())->getFractal().first->empty() &&
                        isIntegratingConstantAndCanChangeIt(static_cast<Fractal*>(it->get())->getFractal().first->back()->getId()))
                {
                    VariablesDefinition * def = VariablesDistributor::get().getVariablesDefinition(
                                static_cast<Variable*>(static_cast<Fractal*>(it->get())->getFractal().first->back().get())->getId());
                    auto range = def->getRange();
                    range.addSegmentWithoutNormilizing(zero, zero, true, true);
                    range.normalize();
                    def->setRange(range);
                    it1 = list.erase(it1);
                }
                else
                    ++it1;
                it->get()->simplify();
            }
        //    else if (isIntegratingConstantAndCanChangeIt((*it / *it1)->getId()))
        //    {
         //       FunctionRange range1 = getRangeOfConstantMultipliersThatCanBeChangedAndTakeThemAway(*it);
        //        FunctionRange range2 = getRangeOfConstantMultipliersThatCanBeChangedAndTakeThemAway(*it1);
         //       *it = *it * integratingConstantExpr(unification(range1, range2));
        //        it1 = list.erase(it1);
        //    }
            else if (it->get()->tryToMergeIdenticalBehindConstantExpressions(*it1))
            {
                it1 = list.erase(it1);
                it->get()->simplify();
            }
            else if (it->get()->getSetOfVariables().empty() || it1->get()->getSetOfVariables().empty())
            {
                if (it->get()->getSetOfVariables().empty())
                    std::swap(*it, *it1);
              //  qDebug() << it->get()->makeStringOfExpression();
                auto it_set = it->get()->getSetOfVariables();
                int var = -1;
                for (auto &it : it_set)
                    if (isIntegratingConstantAndCanChangeIt(it))
                    {
                        var = it;
                        break;
                    }
                if (var == -1)
                {
                    ++it1;
                    continue;
                }
                auto system_var = systemVarExpr();
              //  qDebug() << it->get()->makeStringOfExpression();
                setUpExpressionIntoVariable(*it, system_var, var);
           //     qDebug() << it->get()->makeStringOfExpression();
            //    qDebug() << (*it - *it1)->makeStringOfExpression();
                auto res = solveEquation(*it - *it1, system_var->getId());
                std::map<int, abs_ex> funcs;
                funcs.insert({system_var->getId(), getVariableExpr(var)});
                replaceSystemVariablesBackToFunctions(*it, funcs);
                bool found_point = false;
                for (auto &it2 : res)
                {
                    if (it2->getSetOfVariables().empty())
                    {
                        VariablesDistributor::getVariablesDefinition(var)->setRange(
                                    unification(VariablesDistributor::getVariablesDefinition(var)->getRange(),
                                                FunctionRange(it2, it2, true, true)));
                        found_point = true;
                        break;
                    }
                }
                if (found_point)
                    it1 = list.erase(it1);
                else
                    ++it1;
                it->get()->simplify();
            }
            else
                ++it1;
        }
    }
}
void setDifferentIntegratingConstantInDifferentExpressions(std::set<int> & used_constants, std::list<abs_ex> & exprs)
{
    for (auto &it : exprs)
    {
        auto set = it->getSetOfVariables();
        for (auto &it1 : set)
        {
            if (!isIntegratingConstant(it1))
                continue;
            if (has(used_constants, it1))
            {
                setUpExpressionIntoVariable(it, integratingConstantExpr(VariablesDistributor::getVariablesDefinition(it1)->getRange()), it1);
                //it->setSimplified(false);
               // qDebug()<<it->toString();
                //it->simplify();
            }
            else
                used_constants.insert(it1);


        }
    }
}
void simplifyAndDowncast(std::list<abs_ex> & list)
{
    for (auto &it : list)
    {
       // qDebug() << it->toString();
        it->setSimplified(false);
        it->simplify();
        it = it->downcast();
    }
}

std::pair<std::list<DifurResult>, std::vector<QString>> solveDifur(const abs_ex &difur, int x, int y)
{
    std::vector<QString> steps;
    auto res = solveDifurInCommonIntegral(difur, x, y, steps);
    //qDebug() << res.begin()->get()->toString();
    std::list<abs_ex> solved_for_y;
    std::list<abs_ex> solved_for_x;
    for (auto it = res.begin(); it != res.end();)
    {
        std::list<abs_ex> equation_res;
        bool for_y = false;
        if (has(it->get()->getSetOfVariables(), y))
        {
            equation_res = solveEquation(*it, y);
            for_y = true;
        }
        else if (has(it->get()->getSetOfVariables(), x))
            equation_res = solveEquation(*it, x);
        else
        {
            ++it;
            continue;
        }
        if (equation_res.size() > 0)
        {
            it = res.erase(it);
            if (for_y)
                solved_for_y.splice(solved_for_y.begin(), std::move(equation_res));
            else
                solved_for_x.splice(solved_for_x.begin(), std::move(equation_res));
        }
        else
            ++it;
    }
    //for (auto &it : solved_for_x)
    //    qDebug() << it->toString();
   // for (auto &it : solved_for_y)
     //   qDebug() << it->toString();
    std::set<int> used_constants;
    setDifferentIntegratingConstantInDifferentExpressions(used_constants, res);
    setDifferentIntegratingConstantInDifferentExpressions(used_constants, solved_for_x);
    setDifferentIntegratingConstantInDifferentExpressions(used_constants, solved_for_y);

    simplifyAndDowncast(res);
    simplifyAndDowncast(solved_for_x);
    simplifyAndDowncast(solved_for_y);

    uniteSameResults(res);
    uniteSameResults(solved_for_x);
    uniteSameResults(solved_for_y);

    simplifyAndDowncast(res);
    simplifyAndDowncast(solved_for_x);
    simplifyAndDowncast(solved_for_y);

    std::list<DifurResult> result;
    for (auto &it : res)
        result.push_back(DifurResult(std::move(it), DifurResult::COMMON_INTEGRAL));
    for (auto &it : solved_for_x)
        result.push_back(DifurResult(std::move(it), DifurResult::SOLVED_FOR_X));
    for (auto &it : solved_for_y)
        result.push_back(DifurResult(std::move(it), DifurResult::SOLVED_FOR_Y));
    return {std::move(result), std::move(steps)};
}



bool isEquationOfAppropriateForm(const abs_ex & difur, int x, int y)
{
    bool has_wrong_deriv = false;
    difur->doSomethingInDerivativeObject([&has_wrong_deriv, &y, &x](int _y, int _x, int order){
        if (order > 0)
        {
            if (_y != y || _x != x)
                has_wrong_deriv = true;
        }
    });
    if (has_wrong_deriv)
        return false;
    if (!isItFirstOrderExpression(difur, x, y))
        if (difur->hasDifferential())
            return false;
    return true;
}
bool isItFirstOrderExpression(const abs_ex & difur, int x, int y)
{
    bool has_non_first_order_deriv = false;
    difur->doSomethingInDerivativeObject([&has_non_first_order_deriv, x, y](int _y, int _x, int order){
        if (order > 1 && x == _x && y == _y)
            has_non_first_order_deriv = true;
    });
    return !has_non_first_order_deriv;

}
int getOrderOfHighOrderEquation(const abs_ex & difur, int x, int y)
{
    int order = 0;
    difur->doSomethingInDerivativeObject([&order, x, y](int _y, int _x, int _order){
        if (_y == y && _x == x)
            order = std::max(order, _order);
    });
    return order;
}
bool hasDerivativeObject(const abs_ex & difur, int x, int y)
{
    bool has_object = false;
    difur->doSomethingInDerivativeObject([&has_object, x, y](int _y, int _x, int order) {
        if (order > 0 && x == _x && y == _y)
            has_object = true;
    });
    return has_object;
}














DifurResult::DifurResult(abs_ex &&expr, DifurResult::Type type) : result(std::move(expr)), type(type)
{

}

DifurResult::DifurResult(const DifurResult &res) : result(copy(res.result)), type(res.type)
{

}

DifurResult::DifurResult(DifurResult &&res) : result(std::move(res.result)), type(res.type)
{

}

QString DifurResult::toString() const
{
    QString res;
    if (this->type == COMMON_INTEGRAL)
        res = this->result->toString() + "  = 0";
    else if (this->type == SOLVED_FOR_X)
        res = "x = " + this->result->toString();
    else
        res = "y = " + this->result->toString();
    auto set = this->result->getSetOfVariables();
    for (auto &it : set)
        if (isIntegratingConstant(it))
            res += "  and  " + makeIntegratingConstantName(it) + " " + QChar(8712) + "  "
                    + VariablesDistributor::getVariablesDefinition(it)->getRange().toString();
    return res;
}

QString DifurResult::makeRenderString() const
{
    QString res;
    if (this->type == COMMON_INTEGRAL)
        res = this->result->makeRenderString() + "&Symbol(32)=Symbol(32)&0";
    else if (this->type == SOLVED_FOR_X)
        res = "x&Symbol(32)=Symbol(32)&" + this->result->makeRenderString();
    else
        res = "y&Symbol(32)=Symbol(32)&" + this->result->makeRenderString();
    auto set = this->result->getSetOfVariables();
    for (auto &it : set)
        if (isIntegratingConstant(it))
            res += "&Symbol(32)&Symbol(32)&and&Symbol(32)&Symbol(32)&" + makeIntegratingConstantName(it) + "&Symbol(32)&Symbol(8712)&Symbol(32)&"
                    + VariablesDistributor::getVariablesDefinition(it)->getRange().makeRenderString();
    res.replace("<sub>", "_");
    res.replace("</sub>", "");
    return res;
}

QString DifurResult::makeWolframString() const
{
    QString res;
    if (this->type == COMMON_INTEGRAL)
        res = this->result->makeWolframString() + "  = 0";
    else if (this->type == SOLVED_FOR_X)
        res = "x = " + this->result->makeWolframString();
    else
        res = "y = " + this->result->makeWolframString();
    auto set = this->result->getSetOfVariables();
    for (auto &it : set)
        if (isIntegratingConstant(it))
            res += "  and  " + makeIntegratingConstantName(it) + " " + QChar(8712) + "  "
                    + VariablesDistributor::getVariablesDefinition(it)->getRange().toString();
    return res;
}

const abs_ex &DifurResult::expr() const
{
    return this->result;
}

DifurResult::Type DifurResult::getType() const
{
    return this->type;
}

abs_ex DifurResult::toCommonIntegral(int x, int y) const
{
    switch (this->type)
    {
    case SOLVED_FOR_X:
        return getVariableExpr(x) - this->expr();
    case SOLVED_FOR_Y:
        return getVariableExpr(y) - this->expr();
    case COMMON_INTEGRAL:
        return copy(this->expr());
    default:
        assert(false);
    }
    return nullptr;
}

abs_ex &DifurResult::expr()
{
    return this->result;
}
