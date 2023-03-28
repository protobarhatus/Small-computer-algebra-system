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

PolygonValue::PolygonValue(const std::vector<AlgVector> &pol, const std::vector<QString> &names, const AlgVector &center)
{
    this->polygon = pol;
    this->names = names;
    this->center = center;
    this->has_center_of_outscribed_circle = true;

}

PolygonValue::PolygonValue(std::vector<AlgVector> &&pol, std::vector<QString> &&names, AlgVector &&center)
{
    this->polygon = std::move(pol);
    this->names = std::move(names);
    this->center = std::move(center);
    this->has_center_of_outscribed_circle = true;
}

QString PolygonValue::toString() const
{
    QString res = "";
    for (int i = 0; i < polygon.size(); ++i)
    {
        if (polygon[0].size() == 2)
            res += "<br>" + names[i] + ": (" + polygon[i].x().toString() + "; " + polygon[i].y().toString() + ")";
        else
            res += "<br>" + names[i] + ": (" + polygon[i].x().toString() + "; " + polygon[i].y().toString() + "; " + polygon[i].z().toString() + ")";
    }
    return res;
}

QString PolygonValue::makeRenderString() const
{
    return "String(\"" + toString() + "\")";
}

QString PolygonValue::makeWolframString() const
{
    return toString();
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

MathExpression PolygonValue::getCenter() const
{
    if (!this->has_center_of_outscribed_circle)
        throw QIODevice::tr("Около этого многоугольника нельзя описать окружность");
    return MathExpression(this->center);
}

QString PolygonValue::name() const
{
    QString res;
    for (auto &it : this->names)
        res += it;
    return res;
}

std::vector<AlgVector> PolygonValue::getPolygon() const
{
    return this->polygon;
}

