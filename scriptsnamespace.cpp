#include "scriptsnamespace.h"
#include "algebra/variable.h"
#include "algebra/variablesnamedistributor.h"
#include "algebra/variablesdistributor.h"
#include "algebra/algexpr.h"
#include "analitical_geometry/polyhedron.h"
#include "polygonvalue.h"
#include "analitical_geometry/geometry_3d.h"
#include "planevalue.h"
#include "linevalue.h"
#include "polyhedronvalue.h"
#include "algebra/number.h"
//y'' == (6000000-y)/6000000* 10*1- 1*(2*pi / 86400)^2*(6000000-y)
#include "algebra/number.h"
ScriptsNameSpace::ScriptsNameSpace()
{
    for (int i = 0; i < 26; ++i)
    {
        QString name = makeVariablesName(i + 1);
        auto var_expr = AlgExpr(getVariableExpr(i + 1));
        if (var_expr.toString() == "e")
            this->variables.insert({"e", VariableLiteral("e", e(), true)});
        this->variables.insert({var_expr.toString(),  VariableLiteral(var_expr.toString(), var_expr, true)});
    }
   // this->variables.insert({"E", VariableLiteral("E", e(), true)});
    this->variables.insert({"pi", VariableLiteral("pi", pi(), true)});
    this->variables.insert({"Pi", VariableLiteral("Pi", pi(), true)});

    this->variables.insert({"inf", VariableLiteral("inf", inf(), true)});
    this->variables.insert({"minf", VariableLiteral("minf", minusInf(), true)});

    this->variables.insert({"dx", VariableLiteral("dx", D(getVariable("x").getAlgExprValue()), true)});
    this->variables.insert({"dy", VariableLiteral("dy", D(getVariable("y").getAlgExprValue()), true)});

    this->functions.insert({"_ADD", getAddFunction()});
    this->functions.insert({"_SUB", getSubFunction()});
    this->functions.insert({"_MULT", getMultFunction()});
    this->functions.insert({"_DIV", getDivFunction()});
    //!TODO: clean up variable distributor after every script execution
    this->functions.insert({"var", FunctionLiteral("var", 1, [this](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() != VALUE_STRING)
                                    throw QIODevice::tr("Аргумент функции var должен быть строкой");
                                return MathExpression(certainVar(args[0].getStringValue()));
                            })});

    this->functions.insert({"sqrt", FunctionLiteral("sqrt", 1, [this](std::vector<MathExpression>&& args)->MathExpression {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Argument of sqrt() should be object of elementary algebra");
                                this->addCondition(RootCondition(RootCondition::BIGGER_OR_EQUAL_ZERO, args[0].getAlgExprValue().getExpr()));
                                return MathExpression(sqrt(std::move(args[0].getAlgExprValue())));}, true)});

    this->functions.insert({"cbrt", FunctionLiteral("cbrt", 1, [](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Argument of cbrt() should be object of elementary algebra");
                                return MathExpression(cbrt(std::move(args[0].getAlgExprValue())));}, true)});
    this->functions.insert({"pow", FunctionLiteral("pow", 2, [this](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION || args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Arguments of pow() should be object of elementary algebra");
                                if (args[1].getAlgExprValue().getExpr()->getId() == FRACTAL)
                                {
                                    if ( static_cast<Fractal*>(args[1].getAlgExprValue().getExpr().get())->getFractal().second->empty())
                                    {
                                        if (static_cast<Fractal*>(args[1].getAlgExprValue().getExpr().get())->getCoefficient().getDenominator() % 2 == 0)
                                            this->addCondition(RootCondition(RootCondition::BIGGER_OR_EQUAL_ZERO, args[0].getAlgExprValue().getExpr()));
                                    }
                                    else
                                        this->addCondition(RootCondition(RootCondition::BIGGER_OR_EQUAL_ZERO, args[0].getAlgExprValue().getExpr()));
                                }
                                return MathExpression(pow(std::move(args[0].getAlgExprValue()), std::move(args[1].getAlgExprValue())));}, true)
                           });

    this->functions.insert({"sin", FunctionLiteral("sin", 1, [](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Argument of sin() should be object of elementary algebra");
                                return MathExpression(sin(std::move(args[0].getAlgExprValue())));}, true)});

    this->functions.insert({"cos", FunctionLiteral("cos", 1, [](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Argument of cos() should be object of elementary algebra");
                                return MathExpression(cos(std::move(args[0].getAlgExprValue())));}, true)});

    this->functions.insert({"tan", FunctionLiteral("tan", 1, [](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Argument of tan() should be object of elementary algebra");
                                return MathExpression(tan(std::move(args[0].getAlgExprValue())));}, true)});

    this->functions.insert({"cot", FunctionLiteral("cot", 1, [](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Argument of cot() should be object of elementary algebra");
                                return MathExpression(cot(std::move(args[0].getAlgExprValue())));}, true)});

    this->functions.insert({"asin", FunctionLiteral("asin", 1, [this](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Argument of asin() should be object of elementary algebra");
                                this->addCondition(RootCondition(RootCondition::LESS_OR_EQUAL_ZERO, (args[0].getAlgExprValue() - 1).getExpr()));
                                this->addCondition(RootCondition(RootCondition::BIGGER_OR_EQUAL_ZERO, (args[0].getAlgExprValue() + 1).getExpr()));
                                return asin(std::move(args[0].getAlgExprValue()));
                            }, true)});
    this->functions.insert({"acos", FunctionLiteral("acos", 1, [this](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Argument of acot() should be object of elementary algebra");
                                this->addCondition(RootCondition(RootCondition::LESS_OR_EQUAL_ZERO, (args[0].getAlgExprValue() - 1).getExpr()));
                                this->addCondition(RootCondition(RootCondition::BIGGER_OR_EQUAL_ZERO, (args[0].getAlgExprValue() + 1).getExpr()));
                                return acos(std::move(args[0].getAlgExprValue()));
                            }, true)});
    this->functions.insert({"atan", FunctionLiteral("atan", 1, [](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Argument of atan() should be object of elementary algebra");
                                return MathExpression(atan(std::move(args[0].getAlgExprValue())));}, true)});

    this->functions.insert({"acot", FunctionLiteral("acot", 1, [](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Argument of acot() should be object of elementary algebra");
                                return MathExpression(acot(std::move(args[0].getAlgExprValue())));}, true)});

    this->functions.insert({"ln", FunctionLiteral("ln", 1, [this](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Argument of ln() should be object of elementary algebra");
                                this->addCondition(RootCondition(RootCondition::BIGGER_THAN_ZERO, args[0].getAlgExprValue().getExpr()));
                                return MathExpression(ln(std::move(args[0].getAlgExprValue())));
                            }, true)});

    this->functions.insert({"abs", FunctionLiteral("abs", 1, [](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION && args[0].getType() != VALUE_VECTOR)
                                    throw QString("Argument of abs() should be object of elementary algebra or vector");
                                if (args[0].getType() == VALUE_ALGEBRAIC_EXPRESSION)
                                    return MathExpression(abs(std::move(args[0].getAlgExprValue())));
                                return MathExpression(len(std::move(args[0]).getVectorValue()));
                            }, true)});


    this->functions.insert({"log", FunctionLiteral("log", 2, [this](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION ||
                                        args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Arguments of log() should be objects of elementary algebra");
                                this->addCondition(RootCondition(RootCondition::BIGGER_THAN_ZERO, args[1].getAlgExprValue().getExpr()));
                                this->addCondition(RootCondition(RootCondition::BIGGER_THAN_ZERO, args[0].getAlgExprValue().getExpr()));
                                this->addCondition(RootCondition(RootCondition::DONT_EQUAL_ZERO, (args[0].getAlgExprValue() - 1).getExpr()));
                                //в AlgExpr идет сначала аргумент, затем основание, здесь будет наоборот
                                return MathExpression(log(std::move(args[1].getAlgExprValue()), std::move(args[0].getAlgExprValue())));}, true)});

    this->functions.insert({"deg", FunctionLiteral("deg", 1, [](std::vector<MathExpression> && args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Аргументом deg() должно быть число");
                                return args[0].getAlgExprValue() / 180 * pi();
                            }, true)});

    this->functions.insert({"D", FunctionLiteral("D", 1, [](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Argument of D() should be object of elementary algebra");
                                return MathExpression(D(std::move(args[0].getAlgExprValue())));}, true)});

    this->functions.insert({"D", FunctionLiteral("D", 2, [](std::vector<MathExpression>&& args)->MathExpression {
                                if (args[0].getType() == VALUE_ALGEBRAIC_EXPRESSION &&
                                        args[1].getType() == VALUE_ALGEBRAIC_EXPRESSION)
                                {
                                    if (args[1].getAlgExprValue().getExpr()->getId() == NUMBER)
                                        return MathExpression(D(std::move(args[0].getAlgExprValue()), toIntegerNumber(args[1].getAlgExprValue().getExpr())));
                                    return MathExpression(derivative(std::move(args[0].getAlgExprValue()), args[1].getAlgExprValue()));
                                }
                                if (args[0].getType() == VALUE_VECTOR && args[1].getType() == VALUE_ALGEBRAIC_EXPRESSION)
                                {

                                    return MathExpression(derivative(std::move(args[0].getVectorValue()), args[1].getAlgExprValue()));
                                }
                                throw QIODevice::tr("Неправильные аргументы функции D()");
                            }, true)});



    this->functions.insert({"Integrate", FunctionLiteral("Integrate", 2, [](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION ||
                                        args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Arguments of Integrate() should be objects of elementary algebra");
                                auto integr = integral(std::move(args[0].getAlgExprValue()), args[1].getAlgExprValue());
                                if (integr.getExpr() == nullptr)
                                    throw QIODevice::tr("Не удается взять интеграл от ") + args[0].toString();
                                return MathExpression(integr + integratingConstantExpr());
                            }, true)});
    this->functions.insert({"Integrate", FunctionLiteral("Integrate", 1, [](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Argument of Integrate() should be object of elementary algebra");
                                if (!hasDifferentialAsMultiplier(args[0].getAlgExprValue().getExpr()))
                                    throw QIODevice::tr("Вызов функции Integrate с одним аргументом требует дифференциал какой-либо переменной в качестве множителя");
                                auto integr = integral(std::move(args[0].getAlgExprValue()));
                                if (integr.getExpr() == nullptr)
                                    throw QIODevice::tr("Не удается взять интеграл от ") + args[0].toString();
                                return MathExpression(integr + integratingConstantExpr());
                            })});

    this->functions.insert({"Factorize", FunctionLiteral("Factorize", 1, [](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Argument of Factorize() should be object of elementary algebra");
                                return MathExpression(factorize(std::move(args[0].getAlgExprValue())));}, true)});


this->functions.insert({"Expand", FunctionLiteral("Expand", 1, [](std::vector<MathExpression>&& args) {
                            if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                throw QString("Argument of Expand() should be object of elementary algebra");
                            return MathExpression(expand(std::move(args[0].getAlgExprValue())));}, true)});

    this->functions.insert({"Vect", FunctionLiteral("VECT", 2, [](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_VECTOR || args[1].getType() != VALUE_VECTOR)
                                    throw QString("Arguments of VECT() should be vectors");
                                return MathExpression(args[0].getVectorValue() * args[1].getVectorValue());}, true)});
    this->functions.insert({"Scalar", FunctionLiteral("scalar", 2, [](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_VECTOR || args[1].getType() != VALUE_VECTOR)
                                    throw QString("Arguments of scalar() should be vectors");
                                return MathExpression(scalar(args[0].getVectorValue(), args[1].getVectorValue()));
                            }, true)});
    this->functions.insert({"Det", FunctionLiteral("det", 1, [](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_MATRIX)
                                    throw "Argument of det() must be matrix";
                                return MathExpression(det(std::move(args[0]).getMatrixValue()));
                            }, true)});
    this->functions.insert({"Inverse", FunctionLiteral("inverse", 1, [](std::vector<MathExpression>&&args) {
                                if (args[0].getType() != VALUE_MATRIX)
                                    throw "Argument of inverse() must be matrix";
                                return MathExpression(inverse(std::move(args[0].getMatrixValue())));
                            }, true)});
    this->functions.insert({"RegularPolygon", FunctionLiteral("RegularPolygon", 2, [this](std::vector<MathExpression>&&args) {
                                if (args[0].getType() != VALUE_STRING || args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("RegularPolygon принимает строку и длину");
                               // if (!isIntegerNumber(args[1].getAlgExprValue())
                               //     throw QIODevice::tr("Второй аргумент RegularPolygon должен быть целым числом и не содержать переменные");
                               // int val = toInt(args[1].getAlgExprValue());

                                auto names = splitPointsNames(args[0].getStringValue());
                                for (auto &it : names)
                                    if (this->hasVariable(it))
                                        throw QIODevice::tr("Переменная с именем ") + it + QIODevice::tr(" уже существует; не получается создать соответствующую точку");

                                auto polygon = getBaseRightPolygonAndCenter(args[1].getAlgExprValue(), names.size());
                                for (int i = 0; i < names.size(); ++i)
                                    this->addVariable(names[i], polygon.first[i]);
                                return MathExpression(std::move(polygon.first), std::move(names), std::move(polygon.second));
                            })});
    this->functions.insert({"Rect", FunctionLiteral("Rect", 3, [this](std::vector<MathExpression> && args) {
                                if (args[0].getType() != VALUE_STRING || args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION ||
                                        args[2].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Rect принимает строку и две длины");
                               // if (!isIntegerNumber(args[1].getAlgExprValue())
                               //     throw QIODevice::tr("Второй аргумент RegularPolygon должен быть целым числом и не содержать переменные");
                               // int val = toInt(args[1].getAlgExprValue());

                                auto names = splitPointsNames(args[0].getStringValue());
                                for (auto &it : names)
                                    if (this->hasVariable(it))
                                        throw QIODevice::tr("Переменная с именем ") + it + QIODevice::tr(" уже существует; не получается создать соответствующую точку");
                                if (names.size() != 4)
                                    throw QIODevice::tr("Прямоугольник должен задаваться 4-мя вершинами");

                                auto polygon = getBaseRectangular(args[1].getAlgExprValue(), args[2].getAlgExprValue());
                                for (int i = 0; i < names.size(); ++i)
                                    this->addVariable(names[i], polygon[i]);
                                return MathExpression(std::move(polygon), std::move(names));
                            }, true)});
    this->functions.insert({"Parallelogramm", FunctionLiteral("Parallelogramm", 4, [this](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() != VALUE_STRING || args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION ||
                                        args[2].getType() != VALUE_ALGEBRAIC_EXPRESSION || args[3].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Parallelogramm принимает строку, две длины и угол");
                                if (args[3].getAlgExprValue() == pi()/2)
                                    return this->callFunctionsAction("Rect", {std::move(args[0]), std::move(args[1]), std::move(args[2])});
                                auto names = splitPointsNames(args[0].getStringValue());
                                if (names.size() != 4)
                                    throw QIODevice::tr("Параллелограм должен задаваться 4-мя вершинами");
                                for (auto &it : names)
                                    if (this->hasVariable(it))
                                        throw QIODevice::tr("Переменная с именем ") + it + QIODevice::tr(" уже существует; не получается создать соответствующую точку");
                                auto polygon = getParallelogram(args[1].getAlgExprValue(), args[2].getAlgExprValue(), args[3].getAlgExprValue());
                                for (int i = 0; i < names.size(); ++i)
                                    this->addVariable(names[i], polygon[i]);
                                return MathExpression(std::move(polygon), std::move(names));
                            }, true)});
    this->functions.insert({"Rhomb", FunctionLiteral("Rhomb", 3, [this](std::vector<MathExpression> && args) {
                                if (args[0].getType() != VALUE_STRING || args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION ||
                                        args[2].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Rhomb принимает строку, длину и угол");
                                return this->callFunctionsAction("Parallelogramm", {std::move(args[0]),
                                            std::move(args[1]), std::move(args[1]), std::move(args[3])});
                            }, true)});
    this->functions.insert({"Center", FunctionLiteral("Center", 1, [](std::vector<MathExpression>&&args) {
                                if (args[0].getType() != VALUE_POLYGON)
                                    throw QIODevice::tr("Center() применима только к многоугольнику");
                                return args[0].getPolygon().getCenter();
                            }, true)});
    this->functions.insert({"RightTriangle", FunctionLiteral("RightTriangle", 3, [this](std::vector<MathExpression>&&args) {
                                if (args[0].getType() != VALUE_STRING || args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION || args[2].getType() != args[1].getType())
                                    throw QIODevice::tr("RightTriangle() принимает имя и два катета в качестве аргумента");
                                auto names = splitPointsNames(args[0].getStringValue());
                                for (auto &it : names)
                                    if (this->hasVariable(it))
                                        throw QIODevice::tr("Переменная с именем ") + it + QIODevice::tr(" уже существует; не получается создать соответствующую точку");
                                auto tr = getRightTriangle(args[1].getAlgExprValue(), args[2].getAlgExprValue());
                                for (int i = 0; i < names.size(); ++i)
                                    this->addVariable(names[i], tr[i]);
                                return MathExpression(std::move(tr), std::move(names), (tr[1] + tr[2])/2);
                            }, true)});
    this->functions.insert({"Pyramid", FunctionLiteral("Piramid", 4, [this](std::vector<MathExpression> && args) {
                                if (args[0].getType() != VALUE_POLYGON)
                                    throw QIODevice::tr("Первый аргумент Piramid() должен быть многоугольником");
                                if (args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Второй аргумент Piramid() должен быть высотой");
                                if (args[2].getType() != VALUE_VECTOR)
                                    throw QIODevice::tr("Третий аргумент Piramid() должен быть точкой основания высоты");
                                if (args[3].getType() != VALUE_STRING)
                                    throw QIODevice::tr("Четвертый аргумент Piramid() должен быть именем вершины");
                                auto base = args[2].getVectorValue();
                                if (base.size() != 2)
                                    throw QIODevice::tr("Основание высоты должно выражаться 2-мерным вектором");
                                if (!isAppropriatePointName(args[3].getStringValue()))
                                    throw QIODevice::tr("Неправильное имя точки для вершины");
                                if (this->hasVariable(args[3].getStringValue()))
                                    throw QIODevice::tr("Переменная с именем вершины уже существует");

                                this->addVariable(args[3].getStringValue(), addDimension(args[2].getVectorValue(), args[1].getAlgExprValue()));

                                return MathExpression(makePiramidOverPolygon(args[0].getPolygon().getPolygon(), args[2].getVectorValue(),
                                    args[1].getAlgExprValue(), args[3].getStringValue() + args[0].getPolygon().name()));
                            }, true)});
    this->functions.insert({"Tetrahedron", FunctionLiteral("Tetrahedron", 2, [this](std::vector<MathExpression> && args) {
                                if (args[0].getType() != VALUE_STRING)
                                    throw QIODevice::tr("Первый аргумент Tetrahedron() должен быть именем");
                                if (args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Второй аргумент Tetrahedron() должен быть длиной");
                                auto names = splitPointsNames(args[0].getStringValue());
                                if (names.size() != 4)
                                    throw QIODevice::tr("Имя должно содержать 4 точки");
                                for (auto &it : names)
                                    if (this->hasVariable(it))
                                        throw QIODevice::tr("Переменная с именем ") + it + QIODevice::tr(" уже существует");
                                auto res = makeTetrahedron(args[1].getAlgExprValue(), args[0].getStringValue());
                                for (auto &it : names)
                                    this->addVariable(it, res[it]);

                                return res;

                            }, true)});
    this->functions.insert({"RegularPyramid", FunctionLiteral("RegularPiramid", 3, [this](std::vector<MathExpression> && args) {
                                if (args[0].getType() != VALUE_STRING)
                                    throw QIODevice::tr("Первый аргумент RegularPiramid() должен быть строкой");
                                if (args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Второй аргумент RegularPiramid() должен быть длиной ребра");
                                if (args[2].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Третий аргумент RegularPiramid() должен быть высотой");
                                auto names = splitPointsNames(args[0].getStringValue());
                                for (auto &it : names)
                                    if (this->hasVariable(it))
                                        throw QIODevice::tr("Переменная с именем ") + it + QIODevice::tr(" уже существует");
                                auto res = makeRightPiramid(args[1].getAlgExprValue(), args[2].getAlgExprValue(), args[0].getStringValue());
                                for (auto &it : names)
                                    this->addVariable(it, res[it]);
                                return res;
                            }, true)});
    this->functions.insert({"EquilateralPyramid", FunctionLiteral("EquilateralPyramid", 2, [this](std::vector<MathExpression> && args) {
                                if (args[0].getType() != VALUE_STRING)
                                    throw QIODevice::tr("Первый аргумент EquilateralPyramid() должен быть строкой");
                                if (args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Второй аргумент EquilateralPyramid() должен быть длиной");
                                auto names = splitPointsNames(args[0].getStringValue());
                                for (auto &it : names)
                                    if (this->hasVariable(it))
                                        throw QIODevice::tr("Переменная с именем ") + it + QIODevice::tr(" уже существует");
                                auto res = makeRightEquilateralPiramid(args[1].getAlgExprValue(), args[0].getStringValue());
                                for (auto &it : names)
                                    this->addVariable(it, res[it]);
                                return res;
                            }, true)});
    this->functions.insert({"RegularPyramidWithGivenSideEdge", FunctionLiteral("PyramidWithGivenSideEdge", 3, [this](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() != VALUE_STRING)
                                    throw QIODevice::tr("Первый аргумент должен быть строкой");
                                if (args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Второй аргумент должен быть длиной");
                                if (args[2].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Второй аргумент должен быть длиной");
                                auto names = splitPointsNames(args[0].getStringValue());
                                for (auto &it : names)
                                    if (this->hasVariable(it))
                                        throw QIODevice::tr("Переменная с именем ") + it + QIODevice::tr(" уже существует");
                                auto res = makeRightPiramidWithGivenBaseEdgeAndSideEdge(args[1].getAlgExprValue(), args[2].getAlgExprValue(), args[0].getStringValue());
                                for (auto &it : names)
                                    this->addVariable(it, res[it]);
                                return res;
                            }, true)});
    this->functions.insert({"Prizm", FunctionLiteral("Prizm", 2, [this](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() != VALUE_POLYGON)
                                    throw QIODevice::tr("Первый аргумент Prizm() должен быть многоугольником");
                                if (args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Второй аргумент Prizm() должен быть высотой");
                                auto names = splitPointsNames(args[0].getPolygon().name());
                                QString name = args[0].getPolygon().name();
                                for (auto &it : names)
                                    name += it + "1";
                                auto res = makePrizmOverPolygon(args[0].getPolygon().getPolygon(), args[1].getAlgExprValue(), name);
                                for (auto &it : names)
                                    this->addVariable(it, res[it]);
                                return res;
                            }, true)});
    this->functions.insert({"RegularPrizm", FunctionLiteral("RegularPrizm",3, [this](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() != VALUE_STRING)
                                    throw QIODevice::tr("Первый аргумент RegularPrizm() должен быть строкой");
                                if (args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Второй аргумент RegularPrizm() должен быть длиной");
                                if (args[2].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Третий аргумент RegularPrizm() должен быть высотой");
                                auto names = splitPointsNames(args[0].getStringValue());
                                for (auto &it : names)
                                    if (this->hasVariable(it))
                                        throw QIODevice::tr("Переменная ") + it + QIODevice::tr(" уже существует");
                                auto res = makeRightPrizm(args[1].getAlgExprValue(), args[2].getAlgExprValue(), args[0].getStringValue());
                                for (auto &it : names)
                                    this->addVariable(it, res[it]);
                                return res;
                            }, true)});
    this->functions.insert({"RectParallelepiped", FunctionLiteral("RectParallelepiped", 4, [this](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() != VALUE_STRING)
                                    throw QIODevice::tr("Первый аргумент должен быть строкой");
                                if (args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Второй аргумент должен быть длиной");
                                if (args[2].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Третий аргумент должен быть шириной");
                                if (args[3].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Четвертый аргумент должен быть высотой");
                                auto names = splitPointsNames(args[0].getStringValue());
                                for (auto &it : names)
                                    if (this->hasVariable(it))
                                        throw QIODevice::tr("Переменная ") + it + QIODevice::tr(" уже существует");
                                auto res = makeRectangularParallelepiped(args[1].getAlgExprValue(), args[2].getAlgExprValue(),
                                    args[3].getAlgExprValue(), args[0].getStringValue());
                                for (auto &it : names)
                                    this->addVariable(it, res[it]);
                                return res;
                            }, true)});




    this->functions.insert({"Dist", FunctionLiteral("Dist", 2, [](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() == VALUE_VECTOR && args[1].getType() == VALUE_VECTOR)
                                    return distance(args[0].getVectorValue(), args[1].getVectorValue());
                                if (args[0].getType() == VALUE_VECTOR && args[1].getType() == VALUE_PLANE)
                                    return distance(args[0].getVectorValue(), args[1].getPlaneValue().getValue());
                                if (args[0].getType() == VALUE_PLANE && args[1].getType() == VALUE_VECTOR)
                                    return distance(args[1].getVectorValue(), args[0].getPlaneValue().getValue());
                                if (args[0].getType() == VALUE_VECTOR && args[1].getType() == VALUE_LINE)
                                    return distance(args[0].getVectorValue(), args[1].getLineValue().getValue());
                                if (args[0].getType() == VALUE_LINE && args[1].getType() == VALUE_VECTOR)
                                    return distance(args[1].getVectorValue(), args[0].getLineValue().getValue());
                                if (args[0].getType() == VALUE_LINE && args[1].getType() == VALUE_LINE)
                                    return distance(args[0].getLineValue().getValue(), args[1].getLineValue().getValue());
                                if (args[0].getType() == VALUE_LINE && args[1].getType() == VALUE_PLANE)
                                    return distance(args[0].getLineValue().getValue(), args[1].getPlaneValue().getValue());
                                if (args[0].getType() == VALUE_PLANE && args[1].getType() == VALUE_LINE)
                                    return distance(args[1].getLineValue().getValue(), args[0].getPlaneValue().getValue());
                                if (args[0].getType() == VALUE_PLANE && args[1].getType() == VALUE_PLANE)
                                    return distance(args[0].getPlaneValue().getValue(), args[1].getPlaneValue().getValue());
                                throw QIODevice::tr("Отсутствует функция Dist() для этих типов");
                            }, true)});
    this->functions.insert({"Angle", FunctionLiteral("Angle", 2, [](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() == VALUE_VECTOR && args[1].getType() == VALUE_VECTOR)
                                    return angle(args[0].getVectorValue(), args[1].getVectorValue());
                                if (args[0].getType() == VALUE_VECTOR && args[1].getType() == VALUE_PLANE)
                                    return angle(args[0].getVectorValue(), args[1].getPlaneValue().getValue());
                                if (args[0].getType() == VALUE_PLANE && args[1].getType() == VALUE_VECTOR)
                                    return angle(args[1].getVectorValue(), args[0].getPlaneValue().getValue());
                                if (args[0].getType() == VALUE_VECTOR && args[1].getType() == VALUE_LINE)
                                    return angle(args[0].getVectorValue(), args[1].getLineValue().getValue());
                                if (args[0].getType() == VALUE_LINE && args[1].getType() == VALUE_VECTOR)
                                    return angle(args[1].getVectorValue(), args[0].getLineValue().getValue());
                                if (args[0].getType() == VALUE_LINE && args[1].getType() == VALUE_LINE)
                                    return angle(args[0].getLineValue().getValue(), args[1].getLineValue().getValue());
                                if (args[0].getType() == VALUE_LINE && args[1].getType() == VALUE_PLANE)
                                    return angle(args[0].getLineValue().getValue(), args[1].getPlaneValue().getValue());
                                if (args[0].getType() == VALUE_PLANE && args[1].getType() == VALUE_LINE)
                                    return angle(args[1].getLineValue().getValue(), args[0].getPlaneValue().getValue());
                                if (args[0].getType() == VALUE_PLANE && args[1].getType() == VALUE_PLANE)
                                    return angle(args[0].getPlaneValue().getValue(), args[1].getPlaneValue().getValue());
                                throw QIODevice::tr("Отсутствует функция Angle() для этих типов");
                            }, true)});
    this->functions.insert({"Projection", FunctionLiteral("Projection", 2, [](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() == VALUE_VECTOR && args[1].getType() == VALUE_LINE)
                                    return projectionToLine(args[0].getVectorValue(), args[1].getLineValue().getValue());
                                if (args[0].getType() == VALUE_VECTOR && args[1].getType() == VALUE_PLANE)
                                    return getPointOfNormalToPlane(args[0].getVectorValue(), args[1].getPlaneValue().getValue());

                                throw QIODevice::tr("Отсутствует функция Projection() для этих типов");
                            }, true)});
    this->functions.insert({"Plane", FunctionLiteral("Plane", 3, [](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() == VALUE_VECTOR && args[1].getType() == VALUE_VECTOR && args[2].getType() == VALUE_VECTOR)
                                    return getPlaneThroughThreePoints(args[0].getVectorValue(), args[1].getVectorValue(), args[2].getVectorValue());
                                throw QIODevice::tr("Функция Plane() принимает три вектора");
                            }, true)});
    this->functions.insert({"Plane", FunctionLiteral("Plane", 4, [](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() == VALUE_ALGEBRAIC_EXPRESSION &&
                                args[1].getType() == VALUE_ALGEBRAIC_EXPRESSION &&
                                args[2].getType() == VALUE_ALGEBRAIC_EXPRESSION &&
                                args[3].getType() == VALUE_ALGEBRAIC_EXPRESSION)
                                    return Plane(args[0].getAlgExprValue(), args[1].getAlgExprValue(),
                                        args[2].getAlgExprValue(), args[3].getAlgExprValue());
                                throw QIODevice::tr("Функция Plane() принимает 4 объекта элементарной алгебры");
                            }, true)});
    this->functions.insert({"Line", FunctionLiteral("Line", 2, [](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() == VALUE_VECTOR && args[1].getType() == VALUE_VECTOR)
                                    return Line3d(args[0].getVectorValue(), args[1].getVectorValue());
                                throw QIODevice::tr("Функция Line() принимает 2 вектора");
                            }, true)});
    this->functions.insert({"PlaneThroughPointPerpendicularToVector", FunctionLiteral("PlaneThroughPointPerpendicularToVector", 2, [](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() == VALUE_VECTOR && args[1].getType() == VALUE_VECTOR)
                                    return getPlaneThroughPointPerpendicularToVector(args[0].getVectorValue(), args[1].getVectorValue());
                                throw QIODevice::tr("Аргументами функции должны быть два вектора");
                            }, true)});
    this->functions.insert({"PlaneThroughTwoPointsAndParallelToVector", FunctionLiteral("PlaneThroughTwoPointsAndParallelToVector", 3, [](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() == VALUE_VECTOR && args[1].getType() == VALUE_VECTOR && args[2].getType() == VALUE_VECTOR)
                                    return getPlaneThroughTwoPointsAndParallelToVector(args[0].getVectorValue(), args[1].getVectorValue(), args[2].getVectorValue());
                                throw QIODevice::tr("Аргументами функции должны быть три вектора");
                            }, true)});
    this->functions.insert({"PlaneThroughTwoPointsAndParallelToLine", FunctionLiteral("PlaneThroughTwoPointsAndParallelToLine", 3, [](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() == VALUE_VECTOR && args[1].getType() == VALUE_VECTOR &&
                                        args[2].getType() == VALUE_LINE)
                                    return getPlaneThroughTwoPointsAndParallelToLine(args[0].getVectorValue(),
                                                args[1].getVectorValue(), args[2].getLineValue().getValue());
                                throw QIODevice::tr("Аргументами функции должны быть два вектора и линия");
                            }, true)});
    this->functions.insert({"PlaneThroughTwoPointsAndPerpendicularToPlane", FunctionLiteral("PlaneThroughTwoPointsAndPerpendicularToPlane", 3, [](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() == VALUE_VECTOR && args[1].getType() == VALUE_VECTOR &&
                                        args[2].getType() == VALUE_PLANE)
                                    return getPlaneThroughTwoPointsAndPerpendicularToPlane(
                                            args[0].getVectorValue(), args[1].getVectorValue(), args[2].getPlaneValue().getValue());
                                throw QIODevice::tr("Аргументами функции должны быть два вектора и плоскость");
                            }, true)});
    this->functions.insert({"PlaneThroughLineParallelToLine", FunctionLiteral("PlaneThroughLineParallelToLine", 2, [](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() == VALUE_LINE && args[1].getType() == VALUE_LINE)
                                    return getPlaneThroughLineParallelToLine(args[0].getLineValue().getValue(),
                                                args[1].getLineValue().getValue());
                                throw QIODevice::tr("Аргументами функции должны быть две линии");
                            }, true)});
    this->functions.insert({"Intersection", FunctionLiteral("Intersection", 2, [](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() == VALUE_LINE && args[0].getType() == VALUE_LINE)
                                    return getIntersection(args[0].getLineValue().getValue(), args[1].getLineValue().getValue());
                                if (args[0].getType() != VALUE_LINE && args[1].getType() == VALUE_PLANE)
                                    return getIntersection(args[0].getLineValue().getValue(), args[1].getPlaneValue().getValue());
                                if (args[0].getType() != VALUE_PLANE && args[1].getType() != VALUE_LINE)
                                    return getIntersection(args[1].getLineValue().getValue(), args[0].getPlaneValue().getValue());
                                if (args[0].getType() != VALUE_PLANE && args[1].getType() != VALUE_PLANE)
                                    return args[0].getPlaneValue().getValue().getIntersection(args[1].getPlaneValue().getValue());
                                throw QIODevice::tr("Отсутствует функция Intersection для этих аргументов");
                            }, true)});
    this->functions.insert({"Mid", FunctionLiteral("Mid", 2, [](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() == VALUE_VECTOR && args[1].getType() == VALUE_VECTOR)
                                    return middle(args[0].getVectorValue(), args[1].getVectorValue());
                                throw QIODevice::tr("Аргументы функции mid() должны быть векторы");
                            }, true)});
    this->functions.insert({"Ratio", FunctionLiteral("Ratio", 4, [](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() == VALUE_VECTOR && args[1].getType() == VALUE_VECTOR
                                    &&args[2].getType() == VALUE_ALGEBRAIC_EXPRESSION && args[3].getType() == VALUE_ALGEBRAIC_EXPRESSION)
                                    return ratio(args[0].getVectorValue(), args[1].getVectorValue(),
                                                    args[2].getAlgExprValue(), args[3].getAlgExprValue());
                                throw QIODevice::tr("Неверный набор аргументов для функции Ratio()");

                            }, true)});
    this->functions.insert({"Section", FunctionLiteral("Section", 2, [this](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() != VALUE_POLYHEDRON)
                                    throw QIODevice::tr("Первый аргумент Section() должен быть многогранником");
                                if (args[1].getType() != VALUE_PLANE)
                                    throw QIODevice::tr("Второй аргумент Section() должен быть сечением");
                                auto res = args[0].getPolyhedron().getValue().section(args[1].getPlaneValue().getValue());
                                int counter = 0;
                                std::vector<QString> names(res.size());
                                for (int i = 0; counter < res.size(); ++i)
                                {
                                    QString str = "S";
                                    str.setNum(i);
                                    if (!this->hasVariable(str))
                                    {
                                        names[counter] = str;
                                        this->addVariable(str, res[counter]);
                                        ++counter;
                                    }
                                }
                                return MathExpression(res, names);
                            }, true)});
    this->functions.insert({"Area", FunctionLiteral("Area", 1, [](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() != VALUE_POLYGON)
                                    throw QIODevice::tr("Area принимает многоугольник");
                                return area(args[0].getPolygon().getPolygon());
                            }, true)});
    this->functions.insert({"Cube", FunctionLiteral("Cube", 2, [this](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() != VALUE_STRING)
                                    throw QIODevice::tr("Первый аргумент Cube() должен быть строкой");
                                if (args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Второй аргумент Cube() должен быть длиной");
                                return this->callFunctionsAction("RectParallelepiped", {args[0], args[1], args[1], args[1]});
                            }, true)});
    this->functions.insert({"Square", FunctionLiteral("Square", 2, [this](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() != VALUE_STRING)
                                    throw QIODevice::tr("Первый аргумент функции Square() должен быть квадратом");
                                if (args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Второй аргумент функции Square() должен быть длиной");
                                return this->callFunctionsAction("RegularPolygon", {args[0], args[1]});
                            }, true)});

    this->functions.insert({"Volume", FunctionLiteral("Volume", 1, [this](std::vector<MathExpression> && args) {
                                if (args[0].getType() != VALUE_POLYHEDRON)
                                    throw QIODevice::tr("Volume() принимает многогранник");
                                return args[0].getPolyhedron().getValue().volume();
                            }, true)});
    this->functions.insert({"Teylor", FunctionLiteral("Teylor", 3, [this](std::vector<MathExpression> && args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Первый аргумент Teylor должен быть выражением");
                                if (args[1].getType() != VALUE_VECTOR)
                                    throw QIODevice::tr("Второй аргумент Teylor должен быть вектором значений ВСЕХ переменных функции");
                                if (args[2].getType() != VALUE_ALGEBRAIC_EXPRESSION &&
                                    args[2].getAlgExprValue().getExpr()->getId() != NUMBER)
                                    throw QIODevice::tr("Третий аргумент Teylor должен быть числом - порядком до которого раскладывать");
                                return teylor(args[0].getAlgExprValue(), args[1].getVectorValue(), toInt(args[2].getAlgExprValue()));
                            }, true)});
    this->functions.insert({"Transpose", FunctionLiteral("Transpose", 1, [this](std::vector<MathExpression> && args) {
                                if (args[0].getType() != VALUE_MATRIX)
                                    throw QIODevice::tr("Аргумент должен быть матрицей");
                                return transpose(args[0].getMatrixValue());
                            })});
    this->functions.insert({"T", FunctionLiteral("T", 1, [this](std::vector<MathExpression> && args) {
                                if (args[0].getType() != VALUE_MATRIX)
                                    throw QIODevice::tr("Аргумент должен быть матрицей");
                                return transpose(args[0].getMatrixValue());
                            })});
    this->functions.insert({"vec", FunctionLiteral("vec", 1, [this](std::vector<MathExpression> && args) {
                                if (args[0].getType() == VALUE_ALGEBRAIC_EXPRESSION)
                                    return AlgVector(args[0].getAlgExprValue());
                                if (args[0].getType() == VALUE_MATRIX)
                                {
                                    const Matrix<AlgExpr>& mat = args[0].getMatrixValue();
                                    if (mat.lines() == 1)
                                        return mat[0];
                                    if (mat.columns() == 1)
                                    {
                                        AlgVector res = AlgVector::create(mat.lines());
                                        for (int i = 0; i < mat.lines(); ++i)
                                            res[i] = mat[i][0];
                                        return res;
                                    }
                                    throw QIODevice::tr("Данная матрица не может быть превращена в вектор");
                                }
                                throw QIODevice::tr("Аргумент должен быть числом либо матрицей");
                            })});
    this->functions.insert({"Identic", FunctionLiteral("Identic", 1, [this](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QIODevice::tr("Identic[] принимает число");
                                AlgExpr arg = args[0].getAlgExprValue();
                                if (arg.getExpr()->getId() != NUMBER || !isIntegerNumber(arg.getExpr()))
                                    throw QIODevice::tr("Аргументом функции Identic должно быть константное целое число");
                                int n = toIntegerNumber(arg.getExpr());
                                return MathExpression(makeUnitMatrix<AlgExpr>(n));
                            })});
    this->functions.insert({"Gramm", FunctionLiteral("Gram", 1, [this](std::vector<MathExpression> && args)->MathExpression {
                                if (args[0].getType() != VALUE_MATRIX)
                                    throw QIODevice::tr("Gramm принимает квадратную матрицу");
                                const Matrix<AlgExpr>& mat = args[0].getMatrixValue();
                                if (mat.lines() != mat.columns())
                                    throw QIODevice::tr("матрица в Gramm должна быть квадратной");
                                return MathExpression(Matrix<AlgExpr>(mat.lines(), mat.columns(), [&mat](int i, int j)->AlgExpr {return scalar(mat[i], mat[j]);}));
                            })});
    this->functions.insert({"ScalarG", FunctionLiteral("Gram", 3, [this](std::vector<MathExpression> && args)->MathExpression {
                            return (transpose(args[0].getMatrixValue()) * args[2].getMatrixValue() * args[1].getMatrixValue())[0][0];
                            })});










}

bool ScriptsNameSpace::hasFunction(const QString &name) const
{
    return this->functions.find(name) != this->functions.end();
}

bool ScriptsNameSpace::canChangeFunction(const QString &function, int amount_of_vars) const
{
    return this->getFunctionWithThatAmountOfArguments(function, amount_of_vars).isConst();
}

MathExpression ScriptsNameSpace::callFunctionsAction(const QString &function, std::vector<MathExpression> &&args) const
{
   return this->getFunctionWithThatAmountOfArguments(function, args.size()).callAction(std::move(args));
    assert(false);
}

bool ScriptsNameSpace::hasFunctionWithThatAmountOfArguments(const QString &name, int amount_of_vars) const
{
    auto it = this->functions.find(name);
    while (it != this->functions.end() && it->first == name)
    {
        if (it->second.amountOfArguments() == amount_of_vars)
            return true;
        ++it;
    }
    return false;
}

bool ScriptsNameSpace::hasVariable(const QString &name) const
{
   // qDebug() << (this->variables.find(name) != this->variables.end());
  //  qDebug() << name;
   // qDebug() << this->variables.find(name)->first;
    return this->variables.find(name) != this->variables.end();
}

 const MathExpression & ScriptsNameSpace::getVariable(const QString &name) const
{
    return this->variables.find(name)->second.getExpr();
}

bool ScriptsNameSpace::canChangeVariable(const QString &name) const
{
    return !this->variables.find(name)->second.isConstant();
}

void ScriptsNameSpace::changeVariable(const QString &name, MathExpression &&new_value)
{
    this->variables.find(name)->second.setExpr(std::move(new_value));
}

void ScriptsNameSpace::addVariable(const QString &name, MathExpression &&value)
{
    this->variables.insert({name, VariableLiteral(name, std::move(value))});
}

void ScriptsNameSpace::addFunction(const QString &name, int amount_of_vars, const std::function<MathExpression (std::vector<MathExpression> &&)> & action)
{
    this->functions.insert({name, FunctionLiteral(name, amount_of_vars, action)});
}

void ScriptsNameSpace::changeFunction(const QString &function, int amount_of_vars, const std::function<MathExpression (std::vector<MathExpression> &&)> &new_action)
{
    auto it = this->functions.find(function);
    while (it != this->functions.end() && it->first == function)
    {
        if (it->second.amountOfArguments() ==amount_of_vars)
            it->second.setAction(new_action);
        ++it;
    }
}

void ScriptsNameSpace::addCondition(const RootCondition &cond)
{
    this->conditions.push_back(cond);
}

const std::vector<RootCondition> &ScriptsNameSpace::getConditions() const
{
    return conditions;
}

const FunctionLiteral &ScriptsNameSpace::getFunctionLiteral(const QString &name) const
{
    auto it = this->functions.find(name);
    assert(it != this->functions.end());
    return it->second;
}

const FunctionLiteral &ScriptsNameSpace::getFunctionWithThatAmountOfArguments(const QString &function, int amount_of_vars) const
{
    auto it = this->functions.find(function);
    while (it != this->functions.end() && it->first == function)
    {
        if (it->second.amountOfArguments() ==amount_of_vars)
            return it->second;
        ++it;
    }
}



FunctionLiteral getAddFunction()
{
    return FunctionLiteral("_ADD", 2, [](std::vector<MathExpression>&& args)->MathExpression {
        return args[0] + args[1];
    },true);
}

FunctionLiteral getSubFunction()
{
    return FunctionLiteral("_SUB", 2, [](std::vector<MathExpression>&& args)->MathExpression {
            return args[0] - args[1];
        },true);
}

FunctionLiteral getMultFunction()
{
    return FunctionLiteral("_MULT", 2, [](std::vector<MathExpression>&& args)->MathExpression {
            return args[0] * args[1];
        },true);
}

FunctionLiteral getDivFunction()
{
    return FunctionLiteral("_DIV", 2, [](std::vector<MathExpression>&& args)->MathExpression {
            return args[0] / args[1];
        },true);
}


/*y''-a*y'+4*y == x^2+x


M = {{1; 2; 3}; {2; 3; 4}; {5; 6*sqrt(2); 7}}

M + {{5; a; b}; {1; 1; 1}; {3; 4; 5}}

Inverse[M]

Det[M]
rect = RegularPolygon["ABCDEF", 3]
pyramid = Pyramid[rect, 3, Center[rect], "H"]
ABC
ABH
Angle[ABC, ABH]*/

