#ifndef LINEVALUE_H
#define LINEVALUE_H
#include "abstractvalue.h"
#include "analitical_geometry/line3d.h"

class LineValue : public AbstractValue
{
public:
    LineValue();
    LineValue(const Line3d & val);
    LineValue(Line3d && val);
    virtual QString toString() const override;
    virtual QString makeRenderString() const override;
    virtual QString makeWolframString() const override;
    virtual AbstractValueType getId() const override;

    virtual std::unique_ptr<AbstractValue> operator+(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator-(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator*(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator/(const std::unique_ptr<AbstractValue> & b) const override;

    Line3d getValue() const;

private:
    Line3d value;
};

#endif // LINEVALUE_H
