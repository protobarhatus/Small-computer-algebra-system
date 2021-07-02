#include "stringvalue.h"
#include <QIODevice>
StringValue::StringValue()
{

}

StringValue::StringValue(const QString &str)
{
    this->value = str;
}

StringValue::StringValue(QString &&str)
{
    this->value = std::move(str);
}

QString StringValue::toString() const
{
    return '"' + this->value + '"';
}

AbstractValueType StringValue::getId() const
{
    return VALUE_STRING;
}

std::unique_ptr<AbstractValue> StringValue::operator+(const std::unique_ptr<AbstractValue> &b) const
{
    if (b->getId() == VALUE_STRING)
        return std::unique_ptr<AbstractValue> (new StringValue(this->value + static_cast<StringValue*>(b.get())->value));
    throw QString("Addition of incomparable types");
}

std::unique_ptr<AbstractValue> StringValue::operator-(const std::unique_ptr<AbstractValue> &) const
{
    throw QIODevice::tr("Нельзя применять оператор \'-\' со строкой");
    return nullptr;
}

std::unique_ptr<AbstractValue> StringValue::operator*(const std::unique_ptr<AbstractValue> &) const
{
    throw QIODevice::tr("Нельзя применять оператор \'*\' со строкой");
    return nullptr;
}

std::unique_ptr<AbstractValue> StringValue::operator/(const std::unique_ptr<AbstractValue> &) const
{
    throw QIODevice::tr("Нельзя применять оператор \'/\' со строкой");
    return nullptr;
}

QString StringValue::getValue() const
{
    return this->value;
}
