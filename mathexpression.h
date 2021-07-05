#ifndef MATHEXPRESSION_H
#define MATHEXPRESSION_H
#include "abstractvalue.h"
#include "analitical_geometry/polyhedron.h"
#include <memory>
class AlgExpr;
class ScriptsNameSpace;
#include <algebra/Matrix.h>

AbstractValueType valueTypeFromStringName(const QString & name);

class PolygonValue;
class PlaneValue;
class LineValue;
class MathExpression
{
public:
    MathExpression();
    MathExpression(nullptr_t);
    MathExpression(MathExpression && expr);
    MathExpression(const AlgExpr & expr);
    MathExpression(AlgExpr && expr);
    MathExpression(const Vector<AlgExpr> & expr);
    MathExpression( Vector<AlgExpr> && expr);
    MathExpression(const Matrix<AlgExpr> & expr);
    MathExpression(Matrix<AlgExpr> && expr);
    MathExpression(const std::vector<AlgVector> & vert, const std::vector<QString> & names);
    MathExpression(std::vector<AlgVector> && vert, std::vector<QString> && names);
    MathExpression(const std::vector<AlgVector> & vert, const std::vector<QString> & names, const AlgVector & center);
    MathExpression(std::vector<AlgVector> && vert, std::vector<QString> && names, AlgVector && center);
    MathExpression(const Polyhedron & pol);
    MathExpression(Polyhedron && pol);
    MathExpression(const QString & str);
    MathExpression(QString && str);
    MathExpression(const Line3d & line);
    MathExpression(Line3d && line);
    MathExpression(const Plane & plane);
    MathExpression(Plane && plane);

    MathExpression(std::unique_ptr<AbstractValue> && expr);


    MathExpression(const MathExpression& expr);
    const MathExpression& operator=(const MathExpression & expr);
    const MathExpression& operator=(MathExpression && expr);

    QString toString() const;
    AbstractValueType getType() const;



    friend MathExpression operator+(const MathExpression & left, const MathExpression & right);
    friend MathExpression operator-(const MathExpression & left, const MathExpression & right);
    friend MathExpression operator*(const MathExpression & left, const MathExpression & right);
    friend MathExpression operator/(const MathExpression & left, const MathExpression & right);

    const AlgExpr& getAlgExprValue() const;
    const Vector<AlgExpr>& getVectorValue() const;
    const Matrix<AlgExpr>& getMatrixValue() const;

    const PolygonValue & getPolygon() const;

    const PlaneValue & getPlaneValue() const;
    const LineValue & getLineValue() const;
    //отличия от toString() в том, что это работает только для VALUE_STRING, и при этом НЕ добавляет кавычки для него
    QString getStringValue() const;

    MathExpression executeFunction(const std::vector<MathExpression> & variables) const;
private:

    std::unique_ptr<AbstractValue> value;
};


MathExpression operator+(const MathExpression & left, const MathExpression & right);
MathExpression operator-(const MathExpression & left, const MathExpression & right);
MathExpression operator*(const MathExpression & left, const MathExpression & right);
MathExpression operator/(const MathExpression & left, const MathExpression & right);
#endif // MATHEXPRESSION_H
