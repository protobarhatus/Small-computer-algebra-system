#include "scriptsnamespace.h"
#include "algebra/variable.h"
#include "algebra/variablesnamedistributor.h"
#include "algebra/variablesdistributor.h"
#include "algebra/algexpr.h"
#include "analitical_geometry/polyhedron.h"
#include "polygonvalue.h"
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
    this->variables.insert({"E", VariableLiteral("E", e(), true)});
    this->variables.insert({"pi", VariableLiteral("pi", pi(), true)});
    this->variables.insert({"Pi", VariableLiteral("Pi", pi(), true)});

    this->variables.insert({"dx", VariableLiteral("dx", D(getVariable("x").getAlgExprValue()), true)});
    this->variables.insert({"dy", VariableLiteral("dy", D(getVariable("y").getAlgExprValue()), true)});

    this->functions.insert({"_ADD", getAddFunction()});
    this->functions.insert({"_SUB", getSubFunction()});
    this->functions.insert({"_MULT", getMultFunction()});
    this->functions.insert({"_DIV", getDivFunction()});

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

    this->functions.insert({"D", FunctionLiteral("D", 1, [](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Argument of D() should be object of elementary algebra");
                                return MathExpression(D(std::move(args[0].getAlgExprValue())));}, true)});

    this->functions.insert({"D", FunctionLiteral("D", 2, [](std::vector<MathExpression>&& args) {
                                if (args[0].getType() != VALUE_ALGEBRAIC_EXPRESSION ||
                                        args[1].getType() != VALUE_ALGEBRAIC_EXPRESSION)
                                    throw QString("Arguments of D() should be objects of elementary algebra");
                                return MathExpression(derivative(std::move(args[0].getAlgExprValue()), args[1].getAlgExprValue()));}, true)});

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
