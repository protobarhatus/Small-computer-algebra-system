#ifndef SOLVING_DIFFERENTIAL_EQUATIONS_H
#define SOLVING_DIFFERENTIAL_EQUATIONS_H
#include "abstractexpression.h"
#include "list"
class DifurResult
{
public:
    enum Type {
        COMMON_INTEGRAL,
        SOLVED_FOR_X,
        SOLVED_FOR_Y
    };
    DifurResult (abs_ex && expr, Type type);
    DifurResult (const DifurResult & res);
    DifurResult (DifurResult && res);
    QString toString() const;
    QString makeRenderString() const;
    QString makeWolframString() const;
    const abs_ex &expr() const;
    Type getType() const;
    abs_ex toCommonIntegral(int x, int y) const;
    abs_ex & expr();
private:
    abs_ex result;
    Type type;
};
FunctionRange getRangeOfConstantAddictivesThatCanBeChangedAndTakeThemAway(abs_ex & expr);
FunctionRange getRangeOfConstantMultipliersThatCanBeChangedAndTakeThemAway(abs_ex & expr);
FunctionRange getRangeOfConstantMultipliersThatCanBeChangedAndTakeThemAway(AbstractExpression * expr);
FunctionRange getRangeOfConstantAddictivesThatCanBeChangedAndTakeThemAway(AbstractExpression * expr);
FunctionRange getRangeOfConstantMultipliersThatCanBeChanged(const abs_ex & expr);
FunctionRange getRangeOfConstantMultipliersThatCanBeChanged(const AbstractExpression * expr);
std::pair<std::list<DifurResult>, std::vector<QString>> solveDifur(const abs_ex & difur, int x, int y);
bool hasDerivativeObject(const abs_ex & difur, int x, int y);

#endif // SOLVING_DIFFERENTIAL_EQUATIONS_H
