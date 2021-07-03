#include "polyhedronvalue.h"

PolyhedronValue::PolyhedronValue()
{

}

PolyhedronValue::PolyhedronValue(const Polyhedron &val)
{
    this->value = val;
}

PolyhedronValue::PolyhedronValue(Polyhedron &&val)
{
    this->value = std::move(val);
}

QString PolyhedronValue::toString() const
{
    QString str;
    for (auto &it : this->value.getBasePoints())
    {
        str += "<br>" + it.first + ": " + "(" + it.second->x().toString() + "; " + it.second->y().toString() + "; " + it.second->z().toString() + ")";

    }
    return str;
}

AbstractValueType PolyhedronValue::getId() const
{
    return VALUE_POLYHEDRON;
}

std::unique_ptr<AbstractValue> PolyhedronValue::operator+(const std::unique_ptr<AbstractValue> &) const
{
    throw QIODevice::tr("Нельзя применять оператор \'+\' к геометрическому объекту");
    return nullptr;
}
std::unique_ptr<AbstractValue> PolyhedronValue::operator-(const std::unique_ptr<AbstractValue> &) const
{
    throw QIODevice::tr("Нельзя применять оператор \'-\' к геометрическому объекту");
    return nullptr;
}
std::unique_ptr<AbstractValue> PolyhedronValue::operator*(const std::unique_ptr<AbstractValue> &) const
{
    throw QIODevice::tr("Нельзя применять оператор \'*\' к геометрическому объекту");
    return nullptr;
}
std::unique_ptr<AbstractValue> PolyhedronValue::operator/(const std::unique_ptr<AbstractValue> &) const
{
    throw QIODevice::tr("Нельзя применять оператор \'/\' к геометрическому объекту");
    return nullptr;
}
