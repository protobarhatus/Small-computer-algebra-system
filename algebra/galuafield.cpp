#include "galuafield.h"
#include "some_algebra_expression_conversions.h"
void GaluaField::countReverseElements()
{
    this->reverse_elements.clear();
    this->reverse_elements = std::vector<int>(this->_p + 1, 0);
    for (int i = 1; i < this->_p; ++i)
    {
        reverse_elements[i] = xea(this->_p, i).second.second;
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
    get().countReverseElements();
}

int GaluaField::reverse(int element)
{
    return get().reverse_elements[element % get().p()];
}

int GaluaField::p()
{
    return GaluaField::get()._p;
}

GaluaField::GaluaField()
{

}

GfNumber::GfNumber(long long int val)
{
    this->value = val % GaluaField::p();
}

GfNumber::GfNumber(const GfNumber &cop)
{

    this->value = cop.value;
}

GfNumber &GfNumber::operator=(long long val)
{
    this->value = val % p();
    return *this;
}

GfNumber &GfNumber::operator=(const GfNumber &cop)
{
    this->value = cop.value % p();
    return *this;
}

int GfNumber::p() const
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

bool GfNumber::operator==(const GfNumber &right)
{
    return this->value == right.value;
}

bool GfNumber::operator!=(const GfNumber &right)
{
    return !(*this == right);
}

int GfNumber::toInt()
{
    return value;
}

GfNumber operator+(const GfNumber &left, const GfNumber &right)
{
    return (left.value + right.value) % GaluaField::p();
}

GfNumber operator-(const GfNumber &left, const GfNumber &right)
{
    return (left.value % GaluaField::p() - right.value % GaluaField::p() + GaluaField::p()) % GaluaField::p();
}

GfNumber operator*(const GfNumber &left, const GfNumber &right)
{
    return (left.value * right.value) % GaluaField::p();
}

GfNumber operator/(const GfNumber &left, const GfNumber &right)
{
    return (left.value * GaluaField::reverse(right.value))% GaluaField::p();
}
