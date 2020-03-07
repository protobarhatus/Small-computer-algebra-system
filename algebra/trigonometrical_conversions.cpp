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

    return one - takeDegreeOf(abs_ex(new Sinus(getArgumentOfTrigonometricalFunction(std::move(cosinus_square)))), Number(2));
}
abs_ex sinusToCosinus(abs_ex && sinus_square)
{
    return one - takeDegreeOf(abs_ex(new Cosinus(getArgumentOfTrigonometricalFunction(std::move(sinus_square)))), Number(2));
}
abs_ex tangentToCosinus(abs_ex && tangent_square)
{
    return one / takeDegreeOf(abs_ex(new Cosinus(getArgumentOfTrigonometricalFunction(std::move(tangent_square)))), Number(2)) - one;
}
abs_ex cosinusToTangent(abs_ex && cosinus_square)
{
    return one / (one + takeDegreeOf(abs_ex(new Tangent(getArgumentOfTrigonometricalFunction(std::move(cosinus_square)))), Number(2)));
}
abs_ex cotangentToSinus(abs_ex && cotangent_square)
{
    return one / takeDegreeOf(abs_ex(new Sinus(getArgumentOfTrigonometricalFunction(std::move(cotangent_square)))), Number(2)) - one;
}
abs_ex sinusToCotangent(abs_ex && sinus_square)
{
    return one / (one + takeDegreeOf(abs_ex(new Cotangent(getArgumentOfTrigonometricalFunction(std::move(sinus_square)))), Number(2)));
}
abs_ex tangentToFractal(abs_ex && tangent_degree)
{
    abs_ex argument = getArgumentOfTrigonometricalFunction(std::move(tangent_degree));
    return takeDegreeOf(abs_ex(new Sinus(copy(argument))) / abs_ex(new Cosinus(copy(argument))), Degree::getDegreeOfExpression(tangent_degree.get()));
}
abs_ex cotangentToFractal(abs_ex && cotangent_degree)
{
    abs_ex argument = getArgumentOfTrigonometricalFunction(std::move(cotangent_degree));
    return takeDegreeOf(abs_ex(new Cosinus(copy(argument))) / abs_ex(new Sinus(copy(argument))), Degree::getDegreeOfExpression(cotangent_degree.get()));
}
abs_ex tangentToCotangent(abs_ex && tangent_degree)
{
    return one / takeDegreeOf(abs_ex(new Cotangent(getArgumentOfTrigonometricalFunction(std::move(tangent_degree)))), Degree::getDegreeOfExpression(tangent_degree.get()));
}
abs_ex cotangentToTangent(abs_ex && cotangent_degree)
{
    return one / takeDegreeOf(abs_ex(new Tangent(getArgumentOfTrigonometricalFunction(std::move(cotangent_degree)))), Degree::getDegreeOfExpression(cotangent_degree.get()));
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
