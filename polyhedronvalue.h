#ifndef POLYHEDRONVALUE_H
#define POLYHEDRONVALUE_H
#include "abstractvalue.h"
#include "analitical_geometry/polyhedron.h"

class PolyhedronValue : public AbstractValue
{
public:
    PolyhedronValue();
    PolyhedronValue(const Polyhedron & val);
    PolyhedronValue(Polyhedron && val);

    virtual QString toString() const override;
    virtual AbstractValueType getId() const override;

    virtual std::unique_ptr<AbstractValue> operator+(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator-(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator*(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator/(const std::unique_ptr<AbstractValue> & b) const override;
private:
    Polyhedron value;
};



#endif // POLYHEDRONVALUE_H
