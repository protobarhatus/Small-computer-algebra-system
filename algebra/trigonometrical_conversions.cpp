#include "trigonometrical_conversions.h"
#include "sinus.h"
#include "cosinus.h"
#include "tangent.h"
#include "cotangent.h"
#include "number.h"
#include "degree.h"
#include "some_algebra_expression_conversions.h"
abs_ex cosinusToSinus(abs_ex && cosinus_square)
{

    return one - takeDegreeOf(abs_ex(new Sinus(getArgumentOfFunction(std::move(cosinus_square)))), Number(2));
}
abs_ex sinusToCosinus(abs_ex && sinus_square)
{
    return one - takeDegreeOf(abs_ex(new Cosinus(getArgumentOfFunction(std::move(sinus_square)))), Number(2));
}
abs_ex tangentToCosinus(abs_ex && tangent_square)
{
    return one / takeDegreeOf(abs_ex(new Cosinus(getArgumentOfFunction(std::move(tangent_square)))), Number(2)) - one;
}
abs_ex cosinusToTangent(abs_ex && cosinus_square)
{
    return one / (one + takeDegreeOf(abs_ex(new Tangent(getArgumentOfFunction(std::move(cosinus_square)))), Number(2)));
}
abs_ex cotangentToSinus(abs_ex && cotangent_square)
{
    return one / takeDegreeOf(abs_ex(new Sinus(getArgumentOfFunction(std::move(cotangent_square)))), Number(2)) - one;
}
abs_ex sinusToCotangent(abs_ex && sinus_square)
{
    return one / (one + takeDegreeOf(abs_ex(new Cotangent(getArgumentOfFunction(std::move(sinus_square)))), Number(2)));
}
abs_ex tangentToFractal(abs_ex && tangent_degree)
{
    abs_ex argument = getArgumentOfFunction(std::move(tangent_degree));
    return takeDegreeOf(abs_ex(new Sinus(copy(argument))) / abs_ex(new Cosinus(copy(argument))), Degree::getDegreeOfExpression(tangent_degree.get()));
}
abs_ex cotangentToFractal(abs_ex && cotangent_degree)
{
    abs_ex argument = getArgumentOfFunction(std::move(cotangent_degree));
    return takeDegreeOf(abs_ex(new Cosinus(copy(argument))) / abs_ex(new Sinus(copy(argument))), Degree::getDegreeOfExpression(cotangent_degree.get()));
}
abs_ex tangentToCotangent(abs_ex && tangent_degree)
{
    return one / takeDegreeOf(abs_ex(new Cotangent(getArgumentOfFunction(std::move(tangent_degree)))), Degree::getDegreeOfExpression(tangent_degree.get()));
}
abs_ex cotangentToTangent(abs_ex && cotangent_degree)
{
    return one / takeDegreeOf(abs_ex(new Tangent(getArgumentOfFunction(std::move(cotangent_degree)))), Degree::getDegreeOfExpression(cotangent_degree.get()));
}
//Если нет тангенса и котангенса, то: если есть только косинусы и sin^2, то sin^2 -> 1 - cos^2 (SINUS_TO_COSINUS). Иначе cos^2 -> 1-sin^2 (COSINUS_TO_SINUS)
//Если есть синусы и cot^2, то cot^2 -> 1/sin^2 - 1 (COTANGENT_TO_SINUS)
//Если есть котангенсы и sin^2, то sin^2 -> 1/(1+cot^2) (SINUS_TO_COTANGENT)
//Если есть косинусы и tan^2, то tan^2 -> 1/cos^2 - 1 (TANGENT_TO_COSINUS)
//Если есть тангенсы и cos^2, то cos^2 -> 1/(1 + tan^2) (COSINUS_TO_TANGENT)
//Если есть тангенсы и котангенсы, то cot -> 1/tan (COTANGENT_TO_TANGENT)
//Если есть тангенсы, котангенсы и остальные функции, то tan -> sin/cos; cot -> cos/sin (TANGENTS_AND_COTANGENTS_TO_FRACTAL)
//Иначе ничего не делаем (NONE)
//Отдельно проверяется и выполняется для каждого аргумента
//По аргументу:
//имеет ли триг. функцию означает все степени кроме квадрата
//нулевое - имеет ли синус квадрат, первое - имеет ли синус, второе - имеет ли косинус квадрат, третье - имеет ли косинус, четвертое - имеет ли тангенс квадрат
//пятое - имеет ли тангенс,
//шестое - имеет ли котангенс квадрат, седьмое - имеет ли котангенс
std::map<QString, TrigonometricalFunctionsCastType> chooseConversionRules(std::map<QString, std::tuple<bool, bool, bool, bool, bool, bool, bool, bool> > &params)
{

    std::map<QString, TrigonometricalFunctionsCastType> result;
    for (auto &it : params)
    {
        if (std::get<3>(it.second) && !std::get<4>(it.second) && !std::get<5>(it.second) && !std::get<6>(it.second) && !std::get<7>(it.second) && std::get<0>(it.second) &&
                !std::get<1>(it.second))
            result[it.first] = SINUS_TO_COSINUS;
        else if (std::get<2>(it.second) && (std::get<0>(it.second) || std::get<1>(it.second)) && !std::get<4>(it.second) && !std::get<5>(it.second) && !std::get<6>(it.second) && !std::get<7>(it.second))
            result[it.first] = COSINUS_TO_SINUS;
        else if ((std::get<0>(it.second) || std::get<1>(it.second)) && !std::get<2>(it.second) && !std::get<3>(it.second) && !std::get<4>(it.second) && !std::get<5>(it.second) &&
                 std::get<6>(it.second) && !std::get<7>(it.second))
            result[it.first] = COTANGENT_TO_SINUS;
        else if (std::get<0>(it.second) && !std::get<1>(it.second) && !std::get<2>(it.second) && !std::get<3>(it.second) && !std::get<4>(it.second) && !std::get<5>(it.second) &&
                 (std::get<6>(it.second) || std::get<7>(it.second)))
            result[it.first] = SINUS_TO_COTANGENT;
        else if (!std::get<0>(it.second) && !std::get<1>(it.second) && (std::get<2>(it.second) || std::get<3>(it.second)) && std::get<4>(it.second) && !std::get<5>(it.second) &&
                 !std::get<6>(it.second) && !std::get<7>(it.second))
            result[it.first] = TANGENT_TO_COSINUS;
        else if (!std::get<0>(it.second) && !std::get<1>(it.second) && std::get<2>(it.second) && !std::get<3>(it.second) && (std::get<4>(it.second) || std::get<5>(it.second)) &&
                 !std::get<6>(it.second) && !std::get<7>(it.second))
            result[it.first] = COSINUS_TO_TANGENT;
        else if (!std::get<0>(it.second) && !std::get<1>(it.second) && !std::get<2>(it.second) && !std::get<3>(it.second) && (std::get<4>(it.second) || std::get<5>(it.second)) &&
                 (std::get<6>(it.second) || std::get<7>(it.second)))
            result[it.first] = COTANGENT_TO_TANGENT;
        else if ((std::get<0>(it.second) || std::get<1>(it.second) || std::get<2>(it.second) || std::get<3>(it.second)) && (std::get<4>(it.second) || std::get<5>(it.second) ||
                 std::get<7>(it.second)))
            result[it.first] = TANGENTS_AND_COTANGENTS_TO_FRACTAL;
        else
            result[it.first] = NONE;

    }
    return result;
}

abs_ex convertTrigonometricalFunctionByArgument(abs_ex &&func_degree, TrigonometricalFunctionsArgumentsCastType rule)
{
    switch(rule)
    {
    case FROM_HALF_ARGUMENT:
        return fromHalfArgument(std::move(func_degree));
        break;
    case FROM_DOUBLE_ARGUMENT:
        return fromDoubleArgument(std::move(func_degree));
        break;
    case FROM_TRIPPLE_ARGUMENT:
        return fromTrippleArgument(std::move(func_degree));
        break;
    default:
        return std::move(func_degree);
    }
}

abs_ex fromHalfArgument(abs_ex &&func_degree)
{

    abs_ex  arg = getArgumentOfFunction(func_degree) * two;
    abs_ex degree = Degree::getDegreeOfExpression(func_degree.get()) / two;
    abs_ex res;
    switch(Degree::getArgumentOfDegree(func_degree.get())->getId())
    {
    case SINUS:
        res = (one - cos(arg)) / two;
        break;
    case COSINUS:
        res = (one + cos(arg)) / two;
        break;
    case TANGENT:
        res = (one - cos(arg))/(one + cos(arg));
        break;
    case COTANGENT:
        res = (one + cos(arg)) / (one - cos(arg));
        break;
    default:
        assert(false);
    }
    return takeDegreeOf(std::move(res), std::move(degree));
}

abs_ex fromDoubleArgument(abs_ex &&func_degree)
{
    abs_ex  arg = getArgumentOfFunction(func_degree) / two;
    abs_ex degree = Degree::getDegreeOfExpression(func_degree.get());
    abs_ex res;
    switch (Degree::getArgumentOfDegree(func_degree.get())->getId())
    {
    case SINUS:
        res = two*sin(arg)*cos(arg);
        break;
    case COSINUS:
        res = takeDegreeOf(cos(arg), two) - takeDegreeOf(sin(arg), two);
        break;
    case TANGENT:
        res = two*tan(arg) / (one - takeDegreeOf(tan(arg), two));
        break;
    case COTANGENT:
        res = (takeDegreeOf(cot(arg), two) - one) / (two * cot(arg));
        break;
    default:
        assert(false);
    }
    return takeDegreeOf(std::move(res), std::move(degree));
}

abs_ex fromTrippleArgument(abs_ex &&func_degree)
{
    abs_ex  arg = getArgumentOfFunction(func_degree) / absEx(3);
    abs_ex degree = Degree::getDegreeOfExpression(func_degree.get());
    abs_ex res;
    switch (Degree::getArgumentOfDegree(func_degree.get())->getId())
    {
    case SINUS:
        res = sin(arg) * (absEx(3) - two*two*takeDegreeOf(sin(arg), two));
        break;
    case COSINUS:
        res = cos(arg) * (absEx(4)*takeDegreeOf(cos(arg), two) - absEx(3));
        break;
    case TANGENT:
        res = (absEx(3) * tan(arg) - takeDegreeOf(tan(arg), absEx(3)))/(one - absEx(3)*takeDegreeOf(tan(arg), 2));
        break;
    case COTANGENT:
        res = (absEx(3)*cot(arg) - takeDegreeOf(cot(arg), absEx(3)))/(one - absEx(3)*takeDegreeOf(cot(arg), 2));
        break;
    default:
        assert(false);
    }
    return takeDegreeOf(std::move(res), std::move(degree));
}


std::vector<TrigonometricalFunctionsArgumentsCastType> chooseInstructionsToCastTrigonometryArguments(Number its_degree, bool has_it_odd_deg, QString its_str, std::map<Number, std::pair<std::pair<Number, bool>, QString>, std::function<bool (const Number &, const Number &)> > &coefs)
{
    auto comp = [](const Number & a, const Number & b)->bool { return a.less(b);};
    std::vector<TrigonometricalFunctionsArgumentsCastType> result(1 + coefs.size(), ARG_CAST_NONE);
    if (coefs.size() == 0)
        return result;
    coefs.insert({1, {{its_degree, has_it_odd_deg}, its_str}});
    //иначе получается скорее переусложнение выражения, и вообще такие строить не надо
    if (coefs.size() > 5)
        return result;
    //нормируем коэффициенты так, чтобы первый был единичным, убирая все, кроме 1, 2, 3, 4, 6
    Number norma_coeff = Number(1) / coefs.begin()->first;
    std::map<Number, std::pair<std::pair<Number, bool>, QString>, std::function<bool (const Number & a, const Number & b)>> coes(comp);
    for (auto &it : coefs)
    {
        Number new_coe = it.first * norma_coeff;
        if (new_coe.isInteger() && new_coe.getNumerator() <= 6 && new_coe.getNumerator() != 5)
            coes.insert({new_coe, it.second});
    }
    if (coes.size() < 2)
        return result;
    //степени при соответственно 1, 2, 3, 4, 6 коэффициентах. [0] и [5] не учитываются и остаются для удобства. Нулевая степень означает отсутствие данного коэффициента
    std::vector<std::pair<int, bool>> degrs(7, {0, 0});
    for (auto &it : coes)
        degrs[it.first.getNumerator()] = {it.second.first.first.getNumerator(), it.second.first.second};
    std::vector<TrigonometricalFunctionsArgumentsCastType> preresult(7, ARG_CAST_NONE);
    //необходимо выбрать аргумент, к которому приводить
    int cast_to_arg;
    if (degrs[1].first % 2 == 1 || degrs[1].second)
        cast_to_arg = 1;
    else if (degrs[1].first != 0 && !degrs[1].second && degrs[2].first != 0)
        cast_to_arg = 2;
    else if (degrs[2].first != 0 && !degrs[1].second && ((degrs[4].first != 0 && degrs[2].first <= 2) || degrs[6].first == 1))
        cast_to_arg = 2;
    else if (degrs[1].first != 0 && !degrs[1].second && degrs[4].first != 0)
        cast_to_arg = 2;
    else if (degrs[1].first != 0 && degrs[3].first != 0)
        cast_to_arg = 1;
    else if (degrs[1].first == 2 && !degrs[1].second && (degrs[4].first != 0 || degrs[6].first != 0))
        cast_to_arg = 2;

    std::map<Number, std::pair<std::pair<Number, bool>, QString>, std::function<bool (const Number & a, const Number & b)>>::iterator map_it = coefs.begin();
    int degrs_ind = 0;
    int result_ind = 1;
    while (degrs_ind < degrs.size())
    {
        while (degrs_ind < degrs.size() && degrs[degrs_ind].first == 0)
            ++degrs_ind;
        auto set_to_result = [&result](int & result_index, Number num, TrigonometricalFunctionsArgumentsCastType conv)->void
        {
            if (num == 1)
                result[0] = conv;
            else
            {
                result[result_index] = conv;
                ++result_index;
            }
        };
        if (degrs_ind < degrs.size())
        {
            if (cast_to_arg == 2 * degrs_ind)
                set_to_result(result_ind, map_it->first, FROM_HALF_ARGUMENT);
            else if (cast_to_arg * 2 == degrs_ind)
                set_to_result(result_ind, map_it->first, FROM_DOUBLE_ARGUMENT);
            else if (cast_to_arg * 3 == degrs_ind)
                set_to_result(result_ind, map_it->first, FROM_TRIPPLE_ARGUMENT);

            ++degrs_ind;
            ++map_it;
        }

    }
    return result;
}
//все степени здесь целые
//первое выражение - аргумент функции, второй - степень функции и то, есть у этой функции нечетная степень, затем в словаре аргументы, которые представляют собой тот
//аргумент, умноженный на какое-либо число.
//ключ словаря - то самое число,  число в значении словаря - степени триг. функции с этим самым аргументом и то, есть ли у этой функции нечетная степень,
//а строка - строковое представление функции с этим аргументом
void distributeTrigonometryArgumentsMultipliersRatios(std::vector<std::pair<std::pair<abs_ex, std::pair<Number, bool> >, std::map<Number, std::pair<std::pair<Number, bool>, QString>, std::function<bool (const Number &, const Number &)> > > > &arguments_multipliers,
                                                      std::vector<std::pair<abs_ex, Number> > &arguments)
{
     auto comp = [](const Number & a, const Number & b)->bool { return a.less(b);};
    for (auto &it1 : arguments)
    {
      //  qDebug() << it1.first->makeStringOfExpression();
        bool found_match = false;
        for (auto &it2 : arguments_multipliers)
        {
            auto div_res = it1.first / it2.first.first;
            if (div_res->getId() == NUMBER && !static_cast<Number*>(div_res.get())->isOne())
            {
              //  qDebug() << it1.first->makeStringOfExpression();
               // qDebug() << it2.first.first->makeStringOfExpression();
              //  qDebug() << div_res->makeStringOfExpression();
                found_match = true;
                auto fres = it2.second.find(*static_cast<Number*>(div_res.get()));
                if (fres == it2.second.end())
                    it2.second.insert({*static_cast<Number*>(div_res.get()), {{it1.second, it1.second.getNumerator() % 2}, it1.first->makeStringOfExpression()}});
                else
                    fres->second.first = {max(fres->second.first.first, it1.second), fres->second.first.second || (it1.second.getNumerator() % 2)};
            }
            else if (div_res->getId() == NUMBER)
            {
                found_match = true;
                 /*auto fres = it2.second.find(*static_cast<Number*>(div_res.get()));
                 if (fres != it2.second.end())
                     fres->second.first.second = fres->second.first.second || (it1.second.getNumerator() % 2);*/
                it2.first.second.second = it2.first.second.second || (it1.second.getNumerator() % 2);
            }
        }
        if (!found_match)
        {
            std::pair<abs_ex, std::pair<Number, bool>> p = {copy(it1.first), std::pair<Number, bool>{it1.second, it1.second.getNumerator() % 2}};
            arguments_multipliers.push_back({std::move(p), std::map<Number, std::pair<std::pair<Number, bool>, QString>, std::function<bool (const Number & a, const Number & b)>>(comp)});
        }
    }
}
//По аргументу: все степени здесь целые
//первое выражение - аргумент функции, второй - степень функции и то, есть у этой функции нечетная степень, затем в словаре аргументы, которые представляют собой тот
//аргумент, умноженный на какое-либо число.
//ключ словаря - то самое число,  число в значении словаря - степени триг. функции с этим самым аргументом и то, есть ли у этой функции нечетная степень,
//а строка - строковое представление функции с этим аргументом
std::pair<std::map<QString, TrigonometricalFunctionsArgumentsCastType>, bool> chooseInstructionsToCastTrigonometryArguments(std::vector<std::pair<std::pair<abs_ex, std::pair<Number, bool> >, std::map<Number, std::pair<std::pair<Number, bool>, QString>, std::function<bool (const Number &, const Number &)> > > > &arguments_multipliers)
{
    std::map<QString, TrigonometricalFunctionsArgumentsCastType> instructions;
    bool need_to_convert = false;
    //qDebug() << arguments_multipliers.size();
   // if (arguments_multipliers.size() > 0)
    //    qDebug() << arguments_multipliers.begin()->first.second.makeStringOfExpression();
    for (auto &it : arguments_multipliers)
    {
        auto vec_res = chooseInstructionsToCastTrigonometryArguments(it.first.second.first, it.first.second.second, it.first.first->makeStringOfExpression(), it.second);
        instructions.insert({it.first.first->makeStringOfExpression(), vec_res[0]});
        if (vec_res[0] != ARG_CAST_NONE)
            need_to_convert = true;
        int res_ind = 1;
        for (auto &it1 : it.second)
        {
            if (it1.second.second == it.first.first->makeStringOfExpression())
                continue;
            instructions.insert({it1.second.second, vec_res[res_ind]});
            if (vec_res[res_ind] != ARG_CAST_NONE)
                need_to_convert = true;
            ++res_ind;
        }
    }
    return {instructions, need_to_convert};
}
