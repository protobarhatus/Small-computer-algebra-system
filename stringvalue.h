#ifndef STRINGVALUE_H
#define STRINGVALUE_H

#include "abstractvalue.h"
#include <QString>
class StringValue : public AbstractValue
{
public:
    StringValue();
    StringValue(const QString & str);
    StringValue(QString && str);

    virtual QString toString() const override;
    virtual AbstractValueType getId() const override;

    virtual std::unique_ptr<AbstractValue> operator+(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator-(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator*(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator/(const std::unique_ptr<AbstractValue> & b) const override;
    //НЕ ДОБАВЛЯЕТ КАВЫЧКИ
    QString getValue() const;
private:
    QString value;
};

#endif // STRINGVALUE_H
