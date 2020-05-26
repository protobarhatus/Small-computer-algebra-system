#include "galuafield.h"
#include "some_algebra_expression_conversions.h"
void GaluaField::countReverseElements()
{
    if (this->lock_changing_reverse)
        return;
    if (this->reverse_elements.size() == this->_p + 1)
        return;
    this->reverse_elements.clear();
    this->reverse_elements = std::vector<int>(this->_p + 1, 0);
    for (int i = 1; i < this->_p; ++i)
    {
        reverse_elements[i] = xea(this->_p, i).second.second % this->_p;
        if (reverse_elements[i] < 0)
            reverse_elements[i] += this->_p;
    }
}

GaluaField &GaluaField::get()
{
    static GaluaField field;
    return field;
}

void GaluaField::initialize(int p_value)
{
    get()._p = p_value;
    if (p_value == 0)
    {
        get()._p = std::numeric_limits<long long int>::max();
        get().over_integer = true;
    }
    else
    {
        get().countReverseElements();
        get().over_integer = false;
    }
}

int GaluaField::reverse(int element)
{
    return get().reverse_elements[element % get().p()];
}

long long int GaluaField::p()
{
    return GaluaField::get()._p;
}

bool GaluaField::isOverIntegers()
{
    return GaluaField::get().over_integer;
}

void GaluaField::setLockChangingReverseElements(bool enabled)
{
    GaluaField::get().lock_changing_reverse = enabled;
}

GaluaField::GaluaField()
{
    this->over_integer = true;
}

GfNumber::GfNumber(long long int val)
{
    if (GaluaField::isOverIntegers())
    {
        this->value = val;
        return;
    }
    this->value = val % GaluaField::p();
    if (this->value < 0)
        this->value += GaluaField::p();
}

GfNumber::GfNumber(const GfNumber &cop)
{

    this->value = cop.value;
    if (!GaluaField::isOverIntegers())
    {

        this->value %= GaluaField::p();
        if (this->value < 0)
            this->value += GaluaField::p();
    }
}

GfNumber &GfNumber::operator=(long long val)
{
    if (GaluaField::isOverIntegers())
    {
        this->value = val;
        return *this;
    }
    this->value = val % GaluaField::p();
    if (this->value < 0)
        this->value += GaluaField::p();
    return *this;
}

GfNumber &GfNumber::operator=(const GfNumber &cop)
{
    this->value = cop.value % p();
    return *this;
}

long long int GfNumber::p() const
{
    return GaluaField::p();
}

GfNumber &GfNumber::operator+=(const GfNumber &left)
{
    *this = *this + left;
    return *this;
}

GfNumber &GfNumber::operator-=(const GfNumber &left)
{
    *this = *this - left;
    return *this;
}

GfNumber &GfNumber::operator*=(const GfNumber &right)
{
    *this = *this * right;
    return *this;
}

GfNumber &GfNumber::operator/=(const GfNumber &right)
{
    *this = *this * GaluaField::reverse(right.value);
    return *this;
}

bool GfNumber::operator==(const GfNumber &right) const
{
    return this->value == right.value;
}

bool GfNumber::operator!=(const GfNumber &right) const
{
    return !(*this == right);
}

long long int GfNumber::toInt() const
{
    return value;
}

GfNumber operator+(const GfNumber &left, const GfNumber &right)
{
    return (left.value + right.value) % GaluaField::p();
}

GfNumber operator-(const GfNumber &left, const GfNumber &right)
{
    return (left.value % GaluaField::p() - right.value % GaluaField::p() + (GaluaField::isOverIntegers() ? 0 : GaluaField::p())) % GaluaField::p();
}

GfNumber operator*(const GfNumber &left, const GfNumber &right)
{
    return (left.value * right.value) % GaluaField::p();
}

GfNumber operator/(const GfNumber &left, const GfNumber &right)
{
    if (GaluaField::isOverIntegers())
        return left.value / right.value;
    return (left.value * GaluaField::reverse(right.value))% GaluaField::p();
}

GfNumber operator/(int left, const GfNumber &right)
{
    return GfNumber(left)/right;
}

GfNumber operator/(const GfNumber &left, int right)
{
    return left / GfNumber(right);
}
