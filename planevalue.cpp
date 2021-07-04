#include "planevalue.h"

PlaneValue::PlaneValue()
{

}

PlaneValue::PlaneValue(const Plane &plane)
{
    value = plane;
}

PlaneValue::PlaneValue(Plane &&plane)
{
    value = std::move(plane);
}

QString PlaneValue::toString() const
{
    auto a = this->value.a().toString();
    auto b = this->value.b().toString();
    auto c = this->value.c().toString();

    QString res;

    bool has_add = false;
    if (a != "0")
    {
        has_add = true;
        if (a.contains('+') || a.right(a.length() - 1).contains('-'))
            res += "(" + a + ") * x  ";
        else
            res += a + " * x ";
    }
    if (b != "0")
    {
        if (has_add && b[0] != '-')
            res += "+ ";
        has_add = true;
        if (b.contains('+') || b.right(b.length() - 1).contains('-'))
            res += "(" + b + ") * y  ";
        else
            res += b + " * y  ";
    }
    if (c != "0")
    {
        if (has_add && c[0] != '-')
            res += "+ ";
        has_add = true;
        if (c.contains('+') || c.right(b.length() - 1).contains('-'))
            res += "(" + c + ") * z  ";
        else
            res += c + " * z  ";
    }

    auto d = this->value.d().toString();
    if (d[0] != '-')
        res += "+ ";
    res += d + " == 0";
    return res;

}

AbstractValueType PlaneValue::getId() const
{
    return VALUE_PLANE;
}

std::unique_ptr<AbstractValue> PlaneValue::operator+(const std::unique_ptr<AbstractValue> &b) const
{
    throw QIODevice::tr("Нельзя применять оператор \'+\' к геометрическому объекту");
    return nullptr;
}

std::unique_ptr<AbstractValue> PlaneValue::operator-(const std::unique_ptr<AbstractValue> &b) const
{
    throw QIODevice::tr("Нельзя применять оператор \'-\' к геометрическому объекту");
    return nullptr;
}

std::unique_ptr<AbstractValue> PlaneValue::operator*(const std::unique_ptr<AbstractValue> &b) const
{
    throw QIODevice::tr("Нельзя применять оператор \'*\' к геометрическому объекту");
    return nullptr;
}

std::unique_ptr<AbstractValue> PlaneValue::operator/(const std::unique_ptr<AbstractValue> &b) const
{
    throw QIODevice::tr("Нельзя применять оператор \'/\' к геометрическому объекту");
    return nullptr;
}

Plane PlaneValue::getValue() const
{
    return this->value;
}
