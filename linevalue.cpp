#include "linevalue.h"

LineValue::LineValue()
{

}

LineValue::LineValue(const Line3d &val)
{
    this->value = val;
}

LineValue::LineValue(Line3d &&val)
{
    this->value = std::move(val);
}

QString LineValue::toString() const
{
    return "R = (" + value.getPointOnLine().x().toString() + "; " + value.getPointOnLine().y().toString() + "; " +
            value.getPointOnLine().z().toString() + ") + " + QChar(945) + " * (" + value.getBaseVector().x().toString() +
            "; " + value.getBaseVector().y().toString() + "; " + value.getBaseVector().z().toString() + ")";
}

AbstractValueType LineValue::getId() const
{
    return VALUE_LINE;
}

std::unique_ptr<AbstractValue> LineValue::operator+(const std::unique_ptr<AbstractValue> &b) const
{
    throw QIODevice::tr("Нельзя применять оператор \'+\' к геометрическому объекту");
    return nullptr;
}
std::unique_ptr<AbstractValue> LineValue::operator-(const std::unique_ptr<AbstractValue> &b) const
{
    throw QIODevice::tr("Нельзя применять оператор \'-\' к геометрическому объекту");
    return nullptr;
}
std::unique_ptr<AbstractValue> LineValue::operator*(const std::unique_ptr<AbstractValue> &b) const
{
    throw QIODevice::tr("Нельзя применять оператор \'*\' к геометрическому объекту");
    return nullptr;
}
std::unique_ptr<AbstractValue> LineValue::operator/(const std::unique_ptr<AbstractValue> &b) const
{
    throw QIODevice::tr("Нельзя применять оператор \'/\' к геометрическому объекту");
    return nullptr;
}

Line3d LineValue::getValue() const
{
    return value;
}
