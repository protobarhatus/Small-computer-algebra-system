#include "polygonvalue.h"
#include "algebra/algexpr.h"
PolygonValue::PolygonValue()
{

}


PolygonValue::PolygonValue(const std::vector<AlgVector> &pol, const std::vector<QString> & names)
{
    this->polygon = pol;
    this->names = names;
}

PolygonValue::PolygonValue(std::vector<AlgVector> &&pol, std::vector<QString> && names)
{
    this->polygon = std::move(pol);
    this->names = std::move(names);
}

QString PolygonValue::toString() const
{
    QString res = "";
    for (int i = 0; i < polygon.size(); ++i)
    {
        res += "<br>" + names[i] + ": (" + polygon[i].x().toString() + "; " + polygon[i].y().toString() + ")";
    }
    return res;
}

AbstractValueType PolygonValue::getId() const
{
    return VALUE_POLYGON;
}

std::unique_ptr<AbstractValue> PolygonValue::operator+(const std::unique_ptr<AbstractValue> &) const
{
    throw QIODevice::tr("Нельзя применять этот оператор к геометрическому объекту");
    return nullptr;
}

std::unique_ptr<AbstractValue> PolygonValue::operator-(const std::unique_ptr<AbstractValue> &) const
{
    throw QIODevice::tr("Нельзя применять этот оператор к геометрическому объекту");
    return nullptr;
}

std::unique_ptr<AbstractValue> PolygonValue::operator*(const std::unique_ptr<AbstractValue> &) const
{
    throw QIODevice::tr("Нельзя применять этот оператор к геометрическому объекту");
    return nullptr;
}

std::unique_ptr<AbstractValue> PolygonValue::operator/(const std::unique_ptr<AbstractValue> &) const
{
    throw QIODevice::tr("Нельзя применять этот оператор к геометрическому объекту");
    return nullptr;
}

