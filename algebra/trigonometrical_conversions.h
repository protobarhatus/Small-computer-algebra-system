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
#endif // TRIGONOMETRICAL_CONVERSIONS_H
