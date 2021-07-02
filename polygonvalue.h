#ifndef POLYGONVALUE_H
#define POLYGONVALUE_H
#include "abstractvalue.h"
#include "vector"
#include "algebra/Vector.h"
#include <QString>
class PolygonValue : public AbstractValue
{
public:
    PolygonValue();

    PolygonValue(const std::vector<AlgVector> & pol, const std::vector<QString>& names);
    PolygonValue(std::vector<AlgVector> && pol, std::vector<QString> && names);

    virtual QString toString() const override;
    virtual AbstractValueType getId() const override;

    virtual std::unique_ptr<AbstractValue> operator+(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator-(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator*(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator/(const std::unique_ptr<AbstractValue> & b) const override;

private:
    std::vector<AlgVector> polygon;
    std::vector<QString> names;
};

#endif // POLYGONVALUE_H
