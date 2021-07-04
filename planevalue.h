#ifndef PLANEVALUE_H
#define PLANEVALUE_H
#include "abstractvalue.h"
#include "analitical_geometry/plane.h"

class PlaneValue : public AbstractValue
{
public:
    PlaneValue();
    PlaneValue(const Plane & plane);
    PlaneValue(Plane && plane);

    virtual QString toString() const override;
    virtual AbstractValueType getId() const override;

    virtual std::unique_ptr<AbstractValue> operator+(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator-(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator*(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator/(const std::unique_ptr<AbstractValue> & b) const override;

    Plane getValue() const;

private:
    Plane value;
};

#endif // PLANEVALUE_H
