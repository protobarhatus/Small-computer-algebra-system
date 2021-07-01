#ifndef VARIABLELITERAL_H
#define VARIABLELITERAL_H
#include <QString>
//#include "algebra/algexpr.h"
#include "mathexpression.h"
//это не тоже самое, что и просто переменная в AlgExpr-вском смысле, этот литер замещает любое выражение
class VariableLiteral
{
public:
    VariableLiteral(const QString & name, MathExpression && expr, bool is_constant = false);
    bool operator==(const VariableLiteral & var) const;
    bool operator!=(const VariableLiteral & var) const;

    QString getName() const;
    const MathExpression& getExpr() const;
    bool isConstant() const;
    void setExpr(MathExpression && expr);
private:
    bool is_constant;
    QString name;
    MathExpression expr;
};

#endif // VARIABLELITERAL_H
