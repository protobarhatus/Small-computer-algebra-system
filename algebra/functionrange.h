#ifndef FUNCTIONRANGE_H
#define FUNCTIONRANGE_H
#include <list>
#include "functionrangesegment.h"

class FunctionRange
{
public:
    FunctionRange();
    FunctionRange(const FunctionRange & range);
    FunctionRange(FunctionRange && range);
    FunctionRange(const FunctionRangeSegment & segm);
    FunctionRange(FunctionRangeSegment && segm);

    FunctionRange& operator+(const abs_ex & add);
    std::list<FunctionRangeSegment>& getSegments();

    FunctionRange& operator+=(const abs_ex & add);
    FunctionRange& operator-=(const abs_ex & add);
    FunctionRange& operator*=(const abs_ex & add);
    FunctionRange& operator/=(const abs_ex & add);

    friend FunctionRange operator+(const FunctionRange & range, const abs_ex & add);
    friend FunctionRange operator-(const FunctionRange & range, const abs_ex & sub);
    friend FunctionRange operator*(const FunctionRange & range, const abs_ex & mult);
    friend FunctionRange operator/(const FunctionRange & range, const abs_ex & div);

    friend FunctionRange operator+(FunctionRange && range, const abs_ex & add);
    friend FunctionRange operator-(FunctionRange && range, const abs_ex & sub);
    friend FunctionRange operator*(FunctionRange && range, const abs_ex & mult);
    friend FunctionRange operator/(FunctionRange && range, const abs_ex & div);

    bool isPoint() const ;
    abs_ex getPoint() const;
private:
    void normalize();
    void sort();
    std::list<FunctionRangeSegment> segments;
};

FunctionRange rangeOfSum(const FunctionRange & first, const FunctionRange & second);

FunctionRange operator+(const FunctionRange & range, const abs_ex & add);
FunctionRange operator-(const FunctionRange & range, const abs_ex & sub);
FunctionRange operator*(const FunctionRange & range, const abs_ex & mult);
FunctionRange operator/(const FunctionRange & range, const abs_ex & div);

FunctionRange operator+(FunctionRange && range, const abs_ex & add);
FunctionRange operator-(FunctionRange && range, const abs_ex & sub);
FunctionRange operator*(FunctionRange && range, const abs_ex & mult);
FunctionRange operator/(FunctionRange && range, const abs_ex & div);

#endif // FUNCTIONRANGE_H
