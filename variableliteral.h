#ifndef VARIABLELITERAL_H
#define VARIABLELITERAL_H
#include <QString>
#include "algebra/algexpr.h"
//это не тоже самое, что и просто переменная в AlgExpr-вском смысле, этот литер замещает любое выражение
class VariableLiteral
{
public:
    VariableLiteral(const QString & name, const AlgExpr & expr, bool is_constant = false);
    bool operator==(const VariableLiteral & var) const;
    bool operator!=(const VariableLiteral & var) const;

    QString getName() const;
    AlgExpr getExpr() const;
    bool isConstant() const;
    void setExpr(const AlgExpr & expr);
private:
    bool is_constant;
    QString name;
    AlgExpr expr;
};

#endif // VARIABLELITERAL_H
