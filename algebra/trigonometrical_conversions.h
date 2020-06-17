#ifndef TRIGONOMETRICAL_CONVERSIONS_H
#define TRIGONOMETRICAL_CONVERSIONS_H
#include "abstractexpression.h"
#include <map>
#include <QString>

enum TrigonometricalFunctionsCastType
{
    COSINUS_TO_SINUS,
    SINUS_TO_COSINUS,
    TANGENT_TO_COSINUS,
    COSINUS_TO_TANGENT,
    COTANGENT_TO_SINUS,
    SINUS_TO_COTANGENT,
    TANGENTS_AND_COTANGENTS_TO_FRACTAL,
    TANGENT_TO_COTANGENT,
    COTANGENT_TO_TANGENT,
    NONE
};
enum TrigonometricalFunctionsArgumentsCastType
{
    FROM_TRIPPLE_ARGUMENT,
    FROM_DOUBLE_ARGUMENT,
    FROM_HALF_ARGUMENT,
    ARG_CAST_NONE
};

std::map<QString, TrigonometricalFunctionsCastType> chooseConversionRules(std::map<QString, std::tuple<bool, bool, bool, bool, bool, bool, bool, bool>> & params);
abs_ex cosinusToSinus(abs_ex && cosinus);
abs_ex sinusToCosinus(abs_ex && sinus);
abs_ex tangentToCosinus(abs_ex && tangente);
abs_ex cosinusToTangent(abs_ex && cosinus);
abs_ex cotangentToSinus(abs_ex && cotangent);
abs_ex sinusToCotangent(abs_ex && sinus);
abs_ex tangentToFractal(abs_ex && tangent_degree);
abs_ex cotangentToFractal(abs_ex && cotangent_degree);
abs_ex tangentToCotangent(abs_ex && tangent_degree);
abs_ex cotangentToTangent(abs_ex && cotangent_degree);


std::vector<TrigonometricalFunctionsArgumentsCastType> chooseInstructionsToCastTrigonometryArguments (Number its_degree, bool has_it_odd_deg, QString its_str, std::map<Number, std::pair<std::pair<Number, bool>, QString>, std::function<bool (const Number & a, const Number & b)>> & coefs);
void distributeTrigonometryArgumentsMultipliersRatios(std::vector<std::pair<std::pair<abs_ex, std::pair<Number, bool>>, std::map<Number, std::pair<std::pair<Number, bool>, QString>, std::function<bool (const Number & a, const Number & b)>>>> &
                                                      arguments_multipliers,
                                                      std::vector<std::pair<abs_ex, Number>> & arguments);
std::pair<std::map<QString, TrigonometricalFunctionsArgumentsCastType>, bool> chooseInstructionsToCastTrigonometryArguments(std::vector<std::pair<std::pair<abs_ex, std::pair<Number, bool>>, std::map<Number, std::pair<std::pair<Number, bool>, QString>, std::function<bool (const Number & a, const Number & b)>>>> & arguments_multipliers);
abs_ex convertTrigonometricalFunctionByArgument(abs_ex && func_degree, TrigonometricalFunctionsArgumentsCastType rule);
abs_ex fromHalfArgument(abs_ex && func_degree);
abs_ex fromDoubleArgument(abs_ex && func_degree);
abs_ex fromTrippleArgument(abs_ex && func_degree);

#endif // TRIGONOMETRICAL_CONVERSIONS_H
