#include "functionrange.h"
#include "some_algebra_expression_conversions.h"
FunctionRange::FunctionRange()
{

}

FunctionRange::FunctionRange(const FunctionRange &range)
{
    this->segments = range.segments;
}

FunctionRange::FunctionRange(FunctionRange &&range)
{
    this->segments =  std::move(range.segments);
}

FunctionRange::FunctionRange(const FunctionRangeSegment &segm)
{
    this->segments.push_back(segm);
}

FunctionRange::FunctionRange(FunctionRangeSegment &&segm)
{
    this->segments.push_back(segm);
}

std::list<FunctionRangeSegment> &FunctionRange::getSegments()
{
    return segments;
}

FunctionRange &FunctionRange::operator+=(const abs_ex &add)
{
    for (auto &it : this->segments)
        it += add;
    return *this;
}
FunctionRange &FunctionRange::operator-=(const abs_ex &add)
{
    for (auto &it : this->segments)
        it -= add;
    return *this;
}
FunctionRange &FunctionRange::operator*=(const abs_ex &add)
{
    for (auto &it : this->segments)
        it *= add;
    if (add->getPositionRelativelyZero() < 0)
        this->sort();
    return *this;
}
FunctionRange &FunctionRange::operator/=(const abs_ex &add)
{
    for (auto &it : this->segments)
        it /= add;
    if (add->getPositionRelativelyZero() < 0)
        this->sort();
    return *this;
}

bool FunctionRange::isPoint() const
{
    return this->segments.size() == 1 && this->segments.begin()->isPoint();
}

abs_ex FunctionRange::getPoint() const
{
    assert(this->isPoint());
    return copy(this->segments.begin()->min());
}

void FunctionRange::normalize()
{
    enum Type {
        OPEN_INCLUSIVE,
        CLOSE_EXLUSIVE,
        OPEN_EXCLUSIVE,
        CLOSE_INCLUSIVE

    };
    std::list<std::pair<abs_ex, Type>> events;
    for (auto &it : this->segments)
    {
        events.push_back({std::move(it.min()), it.isMinIncluded() ? OPEN_INCLUSIVE : OPEN_EXCLUSIVE});
        events.push_back({std::move(it.max()), it.isMaxIncluded() ? CLOSE_INCLUSIVE : CLOSE_EXLUSIVE});
    }
    events.sort([](const std::pair<abs_ex, Type> & a, const std::pair<abs_ex, Type> & b)->bool {
        if (b.first == nullptr && (b.second == OPEN_EXCLUSIVE || b.second == OPEN_INCLUSIVE))
            return false;
        if (a.first == nullptr && (a.second == OPEN_EXCLUSIVE || a.second == OPEN_INCLUSIVE))
            return true;
        if (a.first == nullptr && (a.second == CLOSE_EXLUSIVE || a.second == CLOSE_INCLUSIVE))
            return false;
        if (b.first == nullptr && (b.second == CLOSE_EXLUSIVE || b.second == CLOSE_INCLUSIVE))
            return true;


        if (subCompare(a.first, b.first))
            return a.second < b.second;
        return (a.first - b.first)->getPositionRelativelyZero() < 0;
    });
    abs_ex begin = nullptr;
    bool begin_inclusive = false;
    abs_ex end = nullptr;
    bool end_inclusive = false;

    this->segments.clear();
    int active_lines = 0;
    for (auto &it : events)
    {
        if (it.second == OPEN_INCLUSIVE || it.second == OPEN_EXCLUSIVE)
        {
            if (active_lines == 0)
            {
                begin = std::move(it.first);
                begin_inclusive = it.second == OPEN_INCLUSIVE;
            }
            ++active_lines;
        }
        else
        {
            if (active_lines == 0)
                assert(false);
            if (active_lines == 1)
            {
                end = std::move(it.first);
                end_inclusive = it.second == CLOSE_INCLUSIVE;
                this->segments.push_back(FunctionRangeSegment(begin, end, begin_inclusive, end_inclusive));
            }
            --active_lines;
        }
    }
    if (active_lines > 0)
        assert(false);

}

void FunctionRange::sort()
{
    //предполагаем, что пересечений нет
    std::sort(segments.begin(), segments.end(), [](const FunctionRangeSegment & a, const FunctionRangeSegment & b)->bool {
        if (a.min() == nullptr)
            return true;
        if (b.min() == nullptr)
            return false;
        return (a.min() - b.min())->getPositionRelativelyZero() < 0;
    });
}

FunctionRange operator+(const FunctionRange &range, const abs_ex &add)
{
    FunctionRange result(range);
    for (auto &it : result.getSegments())
        it += add;
    return result;
}

FunctionRange operator-(const FunctionRange &range, const abs_ex &sub)
{
    FunctionRange result(range);
    for (auto &it : result.getSegments())
        it -= sub;
    return result;
}

FunctionRange operator*(const FunctionRange &range, const abs_ex &mult)
{
    FunctionRange result(range);
    for (auto &it : result.getSegments())
        it *= mult;
    if (mult->getPositionRelativelyZero() < 0)
        result.sort();
    return result;
}

FunctionRange operator/(const FunctionRange &range, const abs_ex &div)
{
    FunctionRange result(range);
    for (auto &it : result.getSegments())
        it /= div;
    if (div->getPositionRelativelyZero() < 0)
        result.sort();
    return result;
}

FunctionRange operator+(FunctionRange &&range, const abs_ex &add)
{
    for (auto &it : range.getSegments())
        it += add;
    return std::move(range);
}
FunctionRange operator-(FunctionRange &&range, const abs_ex &add)
{
    for (auto &it : range.getSegments())
        it -= add;
    return std::move(range);
}
FunctionRange operator*(FunctionRange &&range, const abs_ex &add)
{
    for (auto &it : range.getSegments())
        it *= add;
    if (add->getPositionRelativelyZero() < 0)
        range.sort();
    return std::move(range);
}
FunctionRange operator/(FunctionRange &&range, const abs_ex &add)
{
    for (auto &it : range.getSegments())
        it /= add;
    if (add->getPositionRelativelyZero() < 0)
        range.sort();
    return std::move(range);
}

FunctionRange rangeOfSum(const FunctionRange &first, const FunctionRange &second)
{
    if (first.isPoint())
        return second + first.getPoint();
    if (second.isPoint())
        return first + second.getPoint();

}
