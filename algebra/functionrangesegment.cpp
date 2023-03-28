#include "functionrangesegment.h"
#include "some_algebra_expression_conversions.h"
#include "number.h"
#include "abstractexpression.h"
FunctionRangeSegment::FunctionRangeSegment()
{

}

FunctionRangeSegment::FunctionRangeSegment(const abs_ex& imin, const abs_ex& imax, bool is_min_included, bool is_max_included) :
    _min(copy(imin)), _max(copy(imax)), min_included(is_min_included), max_included(is_max_included)
{

}

FunctionRangeSegment::FunctionRangeSegment(const FunctionRangeSegment &segm) :
    _min(copy(segm._min)),_max(copy(segm._max)), min_included(segm.min_included), max_included(segm.max_included)
{

}

FunctionRangeSegment::FunctionRangeSegment(FunctionRangeSegment &&segm) :
    _min(std::move(segm._min)), _max(std::move(segm._max)), min_included(segm.min_included), max_included(segm.max_included)
{

}

FunctionRangeSegment &FunctionRangeSegment::operator=(const FunctionRangeSegment &segm)
{
    _min = copy(segm.min());
    _max = copy(segm.max());
    min_included = segm.min_included;
    max_included = segm.max_included;
    return *this;
}

FunctionRangeSegment &FunctionRangeSegment::operator=(FunctionRangeSegment &&segm)
{
    _min = std::move(segm._min);
    _max = std::move(segm._max);
    min_included =  segm.min_included;
    max_included = segm.max_included;
    return *this;
}

void FunctionRangeSegment::setMin(const abs_ex &min, bool included)
{
    this->_min = copy(min);
    this->min_included = included;
}

void FunctionRangeSegment::setMax(const abs_ex &max, bool included)
{
    this->_max = copy(max);
    this->max_included = included;
}

const abs_ex& FunctionRangeSegment::min() const
{
    return _min;
}

const abs_ex& FunctionRangeSegment::max() const
{
    return _max;
}

abs_ex &FunctionRangeSegment::min()
{
    return _min;
}

abs_ex &FunctionRangeSegment::max()
{
    return _max;
}

bool FunctionRangeSegment::isMinIncluded() const
{
    return min_included;
}

bool FunctionRangeSegment::isMaxIncluded() const
{
    return max_included;
}

FunctionRangeSegment &FunctionRangeSegment::operator*=(const abs_ex &mult)
{
    if (this->_min != nullptr)
        _min = std::move(_min) * mult;
    if (this->_max != nullptr)
        _max = std::move(_max) * mult;
    if (mult->getPositionRelativelyZero() < 0)
        std::swap(_min, _max);
    return *this;
}

FunctionRangeSegment &FunctionRangeSegment::operator/=(const abs_ex &div)
{
    if (isZero(div))
    {
        _min = copy(zero);
        _max = copy(zero);
        min_included = false;
        max_included = false;
        return *this;
    }
    if (this->_min != nullptr)
        _min = std::move(_min) / div;
    if (this->_max != nullptr)
        _max = std::move(_max) / div;
    if (div->getPositionRelativelyZero() < 0)
        std::swap(_min, _max);
    return *this;
}

FunctionRangeSegment &FunctionRangeSegment::operator+=(const abs_ex &add)
{
    if (this->_min != nullptr)
        _min = std::move(_min) + add;
    if (this->_max != nullptr)
        _max = std::move(_max) + add;
    return *this;
}

FunctionRangeSegment &FunctionRangeSegment::operator-=(const abs_ex &sub)
{
    if (this->_min != nullptr)
        _min = std::move(_min) - sub;
    if (this->_max != nullptr)
        _max = std::move(_max) - sub;
    return *this;
}

bool FunctionRangeSegment::isPoint() const
{
    return this->min() != nullptr && this->max() != nullptr && subCompare(min(), max());
}

bool FunctionRangeSegment::canBeLowerThanZero() const
{
    return this->min() == nullptr || this->min()->getPositionRelativelyZero() < 0;
}

bool FunctionRangeSegment::canBeBiggerThanZero() const
{
    return  this->max() == nullptr || (!isZero(this->max()) &&this->max()->getPositionRelativelyZero() > 0);
}

QString FunctionRangeSegment::toString() const
{
    if (this->min() == nullptr && this->max() == nullptr)
        return QChar(8477);
    if (this->isPoint())
    {
        return "{ " + this->min()->toString() + " }";
    }
    QString res;
    if (this->isMinIncluded())
        res += '[';
    else
        res += '(';
    if (this->min() == nullptr)
        res += "-" + QString(QChar(8734));
    else
        res += this->min()->toString();
    res += "; ";
    if (this->max() == nullptr)
        res += QChar(8734);
    else
        res += this->max()->toString();
    if (this->isMaxIncluded())
        res += ']';
    else
        res += ')';
    return res;
}

QString FunctionRangeSegment::makeRenderString() const
{
    if (this->min() == nullptr && this->max() == nullptr)
        return "Symbol(8477)";
    if (this->isPoint())
    {
        return "{" + this->min()->makeRenderString() + "}";
    }
    QString res;
    if (this->isMinIncluded())
        res += '[';
    else
        res += "!(";
    if (this->min() == nullptr)
        res += "-" + QString("Symbol(8734)");
    else
        res += this->min()->makeRenderString();
    res += "&Symbol(59)&";
    if (this->max() == nullptr)
        res += "Symbol(8734)";
    else
        res += this->max()->makeRenderString();
    if (this->isMaxIncluded())
        res += ']';
    else
        res += ')';
    return res;
}

bool FunctionRangeSegment::isEmpty()
{
    return !(this->min() == nullptr || this->max() == nullptr) &&
            subCompare(min(), max()) && !(this->isMinIncluded() && this->isMaxIncluded());
}

bool FunctionRangeSegment::containsZero() const
{
    if (isZero(min()))
        return isMinIncluded();
    if (isZero(max()))
        return isMaxIncluded();
    return canBeLowerThanZero() && canBeBiggerThanZero();
}
//нужен для уможнения и деления областей значений
class RangeUnit
{
public:
    RangeUnit(const abs_ex & val, bool min, bool inclusive) : value(copy(val)), minus(min), included(inclusive) {}
    RangeUnit(const RangeUnit& unit)
    {
        this->value = copy(unit.value);
        this->minus = unit.minus;
        this->included = unit.included;
    }
    RangeUnit(RangeUnit && unit)
    {
        this->value = std::move(unit.value);
        this->minus = unit.minus;
        this->included = unit.included;
    }
    abs_ex& getValue() { return value;}
    bool isIncluded() {return included;}

    bool operator<(const RangeUnit & sec) const
    {
        if (sec.value == nullptr && sec.minus)
            return false;
        if (this->value == nullptr && this->minus)
            return true;
        if (this->value == nullptr)
            return false;
        if (sec.value == nullptr)
            return true;
        return (this->value - sec.value)->getPositionRelativelyZero() < 0;
    }
    bool lowerThanZero() const
    {
        if (this->value == nullptr)
            return minus;
        return this->value->getPositionRelativelyZero() < 0;
    }
    friend RangeUnit operator*(const RangeUnit& left, const RangeUnit& right);
 private:
    abs_ex value;
    //полезен только для минус бесконечности
    bool minus;
    bool included;
};
RangeUnit operator*(const RangeUnit& left, const RangeUnit& right)
{
    if (left.value == nullptr)
        return RangeUnit(nullptr, left.minus != right.lowerThanZero(), left.included && right.included);
    if (right.value == nullptr)
        return right * left;
    return RangeUnit(left.value * right.value, false, left.included && right.included);
}

std::pair<abs_ex, bool> min(const std::pair<abs_ex, bool> & a, const std::pair<abs_ex, bool> & b)
{
    if (a.first == nullptr || b.first == nullptr)
        return {nullptr, false};
    return (a.first - b.first)->getPositionRelativelyZero() < 0 ? std::pair<abs_ex, bool>(copy(a.first), a.second) : std::pair<abs_ex, bool>(copy(b.first), b.second);
}
std::pair<abs_ex, bool> max(const std::pair<abs_ex, bool> & a, const std::pair<abs_ex, bool> & b)
{
    if (a.first == nullptr || b.first == nullptr)
        return {nullptr, false};
    return (a.first - b.first)->getPositionRelativelyZero() > 0 ? std::pair<abs_ex, bool>(copy(a.first), a.second) : std::pair<abs_ex, bool>(copy(b.first), b.second);
}
RangeUnit min(const RangeUnit& a, const RangeUnit& b)
{
    return a < b ? a : b;
}
RangeUnit max(const RangeUnit& a, const RangeUnit& b)
{
    return a < b ? b : a;
}
abs_ex add(const abs_ex & a, const abs_ex & b)
{
    if (a == nullptr || b == nullptr)
        return nullptr;
    return a + b;
}
abs_ex sub(const abs_ex & a, const abs_ex & b)
{
    if (a == nullptr || b == nullptr)
        return nullptr;
    return a - b;
}
abs_ex mult(const abs_ex & a, const abs_ex & b)
{
    if (a == nullptr || b == nullptr)
        return nullptr;
    return a * b;
}
abs_ex div(const abs_ex & a, const abs_ex & b)
{
    if (a == nullptr || b == nullptr)
        return nullptr;
    return a / b;
}
FunctionRangeSegment operator+(const FunctionRangeSegment &left, const FunctionRangeSegment &right)
{

    return FunctionRangeSegment(add(left.min(), right.min()), add(left.max(), right.max()), left.isMinIncluded() && right.isMinIncluded(),
                                left.isMaxIncluded() && right.isMaxIncluded());
}

FunctionRangeSegment operator-(const FunctionRangeSegment &left, const FunctionRangeSegment &right)
{
    return FunctionRangeSegment(sub(left.min(), right.max()), sub(left.max(), right.min()), left.isMinIncluded() && right.isMaxIncluded(),
                                (!left.isMaxIncluded() || right.isMinIncluded()) && left.max() != nullptr && left.min() != nullptr);
}
bool Xor(bool a, bool b)
{
    return (a || b) && !(a && b);
}
FunctionRangeSegment operator*(const FunctionRangeSegment &left, const FunctionRangeSegment &right)
{
   /* if (left.min() == nullptr && left.max() == nullptr)
        return FunctionRangeSegment(nullptr, nullptr, false, false);
    if (right.min() == nullptr && right.max() == nullptr)
        return FunctionRangeSegment(nullptr, nullptr, false, false);
    if (right.min() == nullptr && left.min() != nullptr)
        return right * left;
    FunctionRangeSegment result;
    if (left.min() == nullptr)
    {
        if (right.canBeBiggerThanZero())
            result.setMin(nullptr, false);
        else if (left.canBeBiggerThanZero())
            result.setMin(left.max() * right.min(), left.isMaxIncluded() && right.isMinIncluded());
        else
            result.setMin(left.max() * right.max(), left.isMaxIncluded() && right.isMaxIncluded());

        if (right.canBeLowerThanZero())
            result.setMax(nullptr, false);
        else if (left.canBeBiggerThanZero())
            result.setMax(left.max() * right.max(), left.isMaxIncluded() && right.isMaxIncluded());
        else
            result.setMax(left.max() * right.min(), left.isMaxIncluded() && right.isMinIncluded());
        return result;

    }
    if (right.max() == nullptr && left.max() != nullptr)
        return right * left;
    if (left.max() == nullptr)
    {
        if (right.canBeLowerThanZero())
            result.setMin(nullptr, false);
        else if (left.canBeLowerThanZero())
            result.setMin(left.min() * right.max(), left.isMinIncluded() && right.isMaxIncluded());
        else
            result.setMin(left.min() * right.min(), left.isMinIncluded() && right.isMinIncluded());

        if (right.canBeBiggerThanZero())
            result.setMax(nullptr, false);
        else if (left.canBeLowerThanZero())
            result.setMax(left.min() * right.min(), left.isMinIncluded() && right.isMinIncluded());
        else
            result.setMax(left.min() * right.max(), left.isMinIncluded() && right.isMinIncluded());
        return result;
    }
    std::pair<abs_ex, bool> minmin = {left.min() * right.min(), left.isMinIncluded() && right.isMinIncluded()};
    std::pair<abs_ex, bool> minmax = {left.min() * right.max(), left.isMinIncluded() && right.isMaxIncluded()};
    std::pair<abs_ex, bool> maxmin = {left.max() * right.min(), left.isMaxIncluded() && right.isMinIncluded()};
    std::pair<abs_ex, bool> maxmax = {left.max() * right.max(), left.isMaxIncluded() && right.isMaxIncluded()};

    auto min_bord = min(min(minmin, minmax), min(maxmin, maxmax));
    auto max_bord = max(max(minmin, minmax), max(maxmin, maxmax));
    return FunctionRangeSegment(min_bord.first, max_bord.first, min_bord.second, max_bord.second);*/

    RangeUnit left_min (left.min(), true, left.isMinIncluded());
    RangeUnit left_max (left.max(), false, left.isMaxIncluded());
    RangeUnit right_min(right.min(), true, right.isMinIncluded());
    RangeUnit right_max(right.max(), false, right.isMaxIncluded());

    RangeUnit minmin = left_min * right_min;
    RangeUnit minmax = left_min * right_max;
    RangeUnit maxmin = left_max * right_min;
    RangeUnit maxmax = left_max * right_max;

    RangeUnit min_res = min(min(minmin, minmax), min(maxmin, maxmax));
    RangeUnit max_res = max(max(minmin, minmax), max(maxmin, maxmax));
    return FunctionRangeSegment(std::move(min_res.getValue()), std::move(max_res.getValue()), min_res.isIncluded(),
                                max_res.isIncluded());
}
std::list<FunctionRangeSegment> toList(FunctionRangeSegment && segm)
{
    std::list<FunctionRangeSegment> res;
    res.push_back(segm);
    return res;
}
std::list<FunctionRangeSegment> toList(FunctionRangeSegment && segm1, FunctionRangeSegment && segm2)
{
    std::list<FunctionRangeSegment> res;
    res.push_back(segm1);
    res.push_back(segm2);
    return res;
}
bool canBeZero(const FunctionRangeSegment & segm)
{
    return (isZero(segm.min()) && segm.isMinIncluded()) || (isZero(segm.max()) && segm.isMaxIncluded()) ||
            (segm.canBeLowerThanZero() && segm.canBeBiggerThanZero());
}
std::list<FunctionRangeSegment> operator/(const FunctionRangeSegment &left, const FunctionRangeSegment &right)
{
    std::list<FunctionRangeSegment> res;
    if (left.min() == nullptr && left.max() == nullptr)
        return toList(FunctionRangeSegment(nullptr, nullptr, false, false));
    if (right.isPoint() && isZero(right.max()))
        return res;
    if (left.isPoint() && isZero(left.min()))
        return toList(FunctionRangeSegment(zero, zero, true, true));
    if (right.min() == nullptr && right.max() == nullptr && canBeZero(left))
        return toList(FunctionRangeSegment(nullptr, nullptr, false, false));
        return toList(FunctionRangeSegment(nullptr, 0, false, false), FunctionRangeSegment(0, nullptr, false, false));
    bool right_lower = right.canBeLowerThanZero();
    bool right_bigger = right.canBeBiggerThanZero();
    auto div_on_positive = [](const FunctionRangeSegment & left, const FunctionRangeSegment &right)->FunctionRangeSegment {
        FunctionRangeSegment result;
        if (left.canBeLowerThanZero())
        {
            if (isZero(right.min()))
                result.setMin(nullptr, false);
            else
                result.setMin(left.min() / right.min(), left.isMinIncluded() && right.isMinIncluded());
        }
        else
        {
            if (right.max() == nullptr)
                result.setMin(copy(zero), false);
            else
                result.setMin(left.min() / right.max(), left.isMinIncluded() && right.isMaxIncluded());
        }

        if (left.canBeBiggerThanZero())
        {
            if (isZero(right.min()))
                result.setMax(nullptr, false);
            else
                result.setMax(left.max() / right.min(), left.isMaxIncluded() && right.isMinIncluded());
        }
        else
        {
            if (right.max() == nullptr)
                result.setMax(copy(zero), false);
            else
                result.setMax(left.max() / right.max(), left.isMaxIncluded() && right.isMaxIncluded());
        }
        return result;
    };

    auto div_on_negative = [&div_on_positive](const FunctionRangeSegment & left, const FunctionRangeSegment &right)->FunctionRangeSegment
    {
        FunctionRangeSegment div = right;
        div *= minus_one;
        auto res= (div_on_positive(left, div));
        res *= minus_one;
        return res;
    };
    if (right_lower && right_bigger)
        return toList(div_on_positive(left, FunctionRangeSegment(zero, right.max(), true, right.isMaxIncluded())),
                      div_on_negative(left, FunctionRangeSegment(right.min(), zero, right.isMinIncluded(), true)));
    if (right_bigger)
        return toList(div_on_positive(left, right));
    return toList(div_on_negative(left, right));
}
