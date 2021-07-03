#ifndef POLYGONVALUE_H
#define POLYGONVALUE_H
#include "abstractvalue.h"
#include "vector"
#include "algebra/Vector.h"
#include <QString>
#include "mathexpression.h"
class PolygonValue : public AbstractValue
{
public:
    PolygonValue();

    PolygonValue(const std::vector<AlgVector> & pol, const std::vector<QString>& names);
    PolygonValue(std::vector<AlgVector> && pol, std::vector<QString> && names);

    PolygonValue(const std::vector<AlgVector> & pol, const std::vector<QString>& names, const AlgVector & center);
    PolygonValue(std::vector<AlgVector> && pol, std::vector<QString> && names, AlgVector && center);

    virtual QString toString() const override;
    virtual AbstractValueType getId() const override;

    virtual std::unique_ptr<AbstractValue> operator+(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator-(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator*(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator/(const std::unique_ptr<AbstractValue> & b) const override;

    MathExpression getCenter() const;

    QString name() const;

    std::vector<AlgVector> getPolygon() const;

private:
    std::vector<AlgVector> polygon;
    std::vector<QString> names;
    //центр описанной окружности, если есть
    AlgVector center;
    bool has_center_of_outscribed_circle = false;
};

#endif // POLYGONVALUE_H
