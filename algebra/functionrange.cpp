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

FunctionRange::FunctionRange(const abs_ex &min, const abs_ex &max, bool min_incl, bool max_incl)
{
    this->segments.push_back(FunctionRangeSegment(min, max, min_incl, max_incl));
}

FunctionRange &FunctionRange::operator=(FunctionRange &&range)
{
    this->segments = std::move(range.segments);
    this->valid = range.valid;
    return *this;
}

FunctionRange &FunctionRange::operator=(const FunctionRange &range)
{
    this->segments = range.segments;
    this->valid = range.valid;
    return *this;
}

std::list<FunctionRangeSegment> &FunctionRange::getSegments()
{
    return segments;
}

const std::list<FunctionRangeSegment> &FunctionRange::getSegments() const
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

abs_ex FunctionRange::getMin() const
{
    if (this->segments.empty())
        return nullptr;
    return copy(this->segments.begin()->min());
}

abs_ex FunctionRange::getMax() const
{
    if (this->segments.empty())
        return nullptr;
    return copy(this->segments.back().max());
}

FunctionRange FunctionRange::getErrorRange()
{
    FunctionRange res;
    res.valid = false;
    return res;
}

bool FunctionRange::isError()
{
    return !this->valid;
}

void FunctionRange::addSegmentWithoutNormilizing(const FunctionRangeSegment &segm)
{
    this->segments.push_back(segm);
}

void FunctionRange::addSegmentWithoutNormilizing(FunctionRangeSegment &&segm)
{
    this->segments.push_back(std::move(segm));
}

void FunctionRange::addSegmentWithoutNormilizing(const abs_ex &min, const abs_ex &max, bool min_incl, bool max_incl)
{
    this->addSegmentWithoutNormilizing(FunctionRangeSegment(min, max, min_incl, max_incl));
}

FunctionRange FunctionRange::subtract(const FunctionRange &second) const
{
    FunctionRange result = *this;
    auto res_it = result.getSegments().begin();
    auto delete_segm = second.getSegments().begin();
    while (res_it != result.getSegments().end() && delete_segm != second.getSegments().end())
    {
        if (res_it->min() != nullptr && delete_segm->max() != nullptr &&
                bigger(res_it->min(), delete_segm->max()))
        {
            ++delete_segm;
            continue;
        }
        if (res_it->max() != nullptr && lower(res_it->max(), delete_segm->min()))
        {
            ++res_it;
            continue;
        }
        if (delete_segm->min() == nullptr && delete_segm->max() == nullptr)
            return FunctionRange();
        if (delete_segm->min() == nullptr)
        {
            if (res_it->min() == nullptr || lower(res_it->min(), delete_segm->max()))
            {
                if (res_it->max() == nullptr || bigger(res_it->max(), delete_segm->max()))
                {
                    res_it->setMin(delete_segm->max(), !delete_segm->isMaxIncluded());
                    ++delete_segm;
                    continue;
                }
                else if (subCompare(res_it->max(), delete_segm->max()))
                {
                    res_it->setMin(delete_segm->max(), res_it->isMaxIncluded() && !delete_segm->isMaxIncluded());
                    ++res_it;
                    continue;
                }
                else
                {
                    res_it = result.segments.erase(res_it);
                    continue;
                }
            }
            //остался вариант res_it->min() == delete_segm->max()
            res_it->setMin(res_it->min(), res_it->isMinIncluded() && !delete_segm->isMaxIncluded());
            ++delete_segm;
            continue;
        }
        if (delete_segm->max() != nullptr)
        {
            if (res_it->min() == nullptr || lowerOrEquall(res_it->min(), delete_segm->min()))
            {
                if (subCompare(res_it->min(), delete_segm->min()))
                        res_it->setMin(res_it->min(), res_it->isMinIncluded() && !delete_segm->isMinIncluded());
                if (res_it->max() == nullptr || bigger(res_it->max(), delete_segm->min()))
                {

                    auto prev_max = std::move(res_it->max());
                    bool prev_max_included = res_it->isMaxIncluded();
                    res_it->setMax(delete_segm->min(), !delete_segm->isMinIncluded());
                    if (prev_max == nullptr || lower(delete_segm->max(), prev_max))
                    {
                        result.segments.insert(next(res_it), FunctionRangeSegment(delete_segm->max(),
                                                                                  prev_max, !delete_segm->isMaxIncluded(),
                                                                                  prev_max_included));
                    }
                    else if (subCompare(delete_segm->max(), prev_max))
                        result.segments.insert(next(res_it), FunctionRangeSegment(delete_segm->max(),
                                                                                  prev_max, prev_max_included &&
                                                                                  !delete_segm->isMaxIncluded(),
                                                                                  prev_max_included && !delete_segm->isMaxIncluded()));
                    ++res_it;
                    continue;
                }
                //res_it->max() == delete_segm->min()
                res_it->setMax(res_it->max(), res_it->isMaxIncluded() && !delete_segm->isMinIncluded());
                ++res_it;
                continue;
            }
            if (lower(res_it->min(), delete_segm->max()))
            {
                if (lower(res_it->max(), delete_segm->max()))
                {
                    res_it = result.segments.erase(res_it);
                    continue;
                }
                if (subCompare(res_it->max(), delete_segm->max()))
                {
                    res_it->setMin(res_it->max(), res_it->isMaxIncluded() && !delete_segm->isMaxIncluded());
                    ++res_it;
                    continue;
                }
                res_it->setMin(delete_segm->max(), !delete_segm->isMaxIncluded());
                ++delete_segm;
                continue;
            }
            //остается только res_it->min() == delete_segm->max()
            //ситуация когда res_it->min() > delete_segm->max() обрабатывается выше
            res_it->setMin(res_it->min(), res_it->isMinIncluded() && !delete_segm->isMaxIncluded());
            ++res_it;
            continue;
        }
        //delete_segm->max() == nullptr
        if (res_it->min() != nullptr && lower(res_it->min(), delete_segm->min()))
        {
            res_it->setMax(delete_segm->min(), !delete_segm->isMinIncluded());
            ++res_it;
            while (res_it != result.segments.end())
                res_it = result.segments.erase(res_it);
        }
        else if (subCompare(res_it->min(), delete_segm->min()))
        {
            res_it->setMax(res_it->min(), res_it->isMaxIncluded() && !delete_segm->isMinIncluded());
            ++res_it;
            while (res_it != result.segments.end())
                res_it = result.segments.erase(res_it);
        }
        else
        {
            while (res_it != result.segments.end())
                res_it = result.segments.erase(res_it);
        }
    }
    result.deleteEmptySegments();
    result.normalize();
    return result;
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
    this->deleteEmptySegments();
}

QString FunctionRange::toString() const
{
    QString res;
    for (auto it = this->segments.begin(); it != this->segments.end(); ++it)
    {
        res += it->toString();
        if (next(it) != this->segments.end())
            res += " U ";
    }
    return res;
}

bool FunctionRange::isSymmetricRelativelyZero() const
{
    if (this->segments.size() == 1)
    {
        if (this->segments.back().min() == nullptr)
            return this->segments.back().max() == nullptr;
        if (this->segments.back().max() == nullptr)
            return false;
        return ( subCompare(this->segments.back().min(), -this->segments.back().max())) &&
                this->segments.back().isMinIncluded() == this->segments.back().isMaxIncluded();
    }
    if (this->segments.empty())
        return true;
    for (auto it = this->segments.begin(), it1 = --this->segments.end(); it != next(it1); ++it, --it1)
    {
        if (it->min() == nullptr)
            return it1->max() == nullptr;
        if (it->max() == nullptr)
            return false;
        if (it->isMinIncluded() != it1->isMaxIncluded())
            return false;
        if (it->isMaxIncluded() != it1->isMinIncluded())
            return false;
        if (!subCompare(it->min(), -it1->max()))
            return false;
        if (it1 != it)
            if (!subCompare(it->max(), -it1->min()))
                return false;
    }
    return true;
}

bool FunctionRange::isEmpty()
{
    return this->segments.empty();
}

bool FunctionRange::containsZero() const
{
    for (auto &it : this->segments)
        if (it.containsZero())
            return true;
    return false;
}

void FunctionRange::deleteEmptySegments()
{
    for (auto it = this->segments.begin(); it != this->segments.end();)
    {
        if (it->isEmpty())
            it = this->segments.erase(it);
        else
            ++it;
    }
}

void FunctionRange::sort()
{
    //предполагаем, что пересечений нет
    segments.sort([](const FunctionRangeSegment & a, const FunctionRangeSegment & b)->bool {
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
    FunctionRange result;
    for (auto &it : first.segments)
    {
        for (auto &it1 : second.segments)
        {
            result.segments.push_back(it + it1);
        }
    }
    result.normalize();
    return result;
}

FunctionRange rangeOfSubtraction(const FunctionRange &left, const FunctionRange &right)
{
    if (right.isPoint())
        return left - right.getPoint();
    FunctionRange result;
    for (auto &it : left.segments)
    {
        for (auto &it1 : right.segments)
        {
            result.segments.push_back(it - it1);
        }
    }
    result.normalize();
    return result;
}

FunctionRange rangeOfMultiplication(const FunctionRange &left, const FunctionRange &right)
{
    if (left.isPoint())
        return right * left.getPoint();
    if (right.isPoint())
        return left * right.getPoint();
    FunctionRange result;
    for (auto &it : left.segments)
    {
        for (auto &it1 : right.segments)
        {
            result.segments.push_back(it * it1);
        }
    }
    result.normalize();
    return result;
}

FunctionRange rangeOfDivision(const FunctionRange &left, const FunctionRange &right)
{
    if (right.isPoint())
        return left / right.getPoint();
    FunctionRange result;
    for (auto &it : left.segments)
    {
        for (auto &it1 : right.segments)
        {
            result.segments.splice(result.segments.end(), it / it1);
        }
    }
    result.normalize();
    return result;
}

FunctionRange unification(const FunctionRange &a, const FunctionRange &b)
{
    FunctionRange result;
    for (auto &it : a.getSegments())
    {
        result.addSegmentWithoutNormilizing(it);
    }
    for (auto &it : b.getSegments())
        result.addSegmentWithoutNormilizing(it);
    result.normalize();
    return result;
}
