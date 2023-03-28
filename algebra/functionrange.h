#ifndef FUNCTIONRANGE_H
#define FUNCTIONRANGE_H
#include <list>
#include "functionrangesegment.h"
#include <QString>
class FunctionRange
{
public:
    FunctionRange();
    FunctionRange(const FunctionRange & range);
    FunctionRange(FunctionRange && range);
    FunctionRange(const FunctionRangeSegment & segm);
    FunctionRange(FunctionRangeSegment && segm);
    FunctionRange(const abs_ex & min, const abs_ex & max, bool min_incl, bool max_incl);
    FunctionRange& operator=(FunctionRange && range);
    FunctionRange& operator=(const FunctionRange & range);

    FunctionRange& operator+(const abs_ex & add);
    std::list<FunctionRangeSegment>& getSegments();
    const std::list<FunctionRangeSegment>& getSegments() const;

    FunctionRange& operator+=(const abs_ex & add);
    FunctionRange& operator-=(const abs_ex & add);
    FunctionRange& operator*=(const abs_ex & add);
    FunctionRange& operator/=(const abs_ex & add);

    bool isPoint() const ;
    abs_ex getPoint() const;

    abs_ex getMin() const;
    abs_ex getMax() const;

    static FunctionRange getErrorRange();
    bool isError();
    void addSegmentWithoutNormilizing(const FunctionRangeSegment & segm);
    void addSegmentWithoutNormilizing(FunctionRangeSegment && segm);
    void addSegmentWithoutNormilizing(const abs_ex & min, const abs_ex & max, bool min_incl, bool max_incl);

    friend FunctionRange operator+(const FunctionRange & range, const abs_ex & add);
    friend FunctionRange operator-(const FunctionRange & range, const abs_ex & sub);
    friend FunctionRange operator*(const FunctionRange & range, const abs_ex & mult);
    friend FunctionRange operator/(const FunctionRange & range, const abs_ex & div);

    friend FunctionRange operator+(FunctionRange && range, const abs_ex & add);
    friend FunctionRange operator-(FunctionRange && range, const abs_ex & sub);
    friend FunctionRange operator*(FunctionRange && range, const abs_ex & mult);
    friend FunctionRange operator/(FunctionRange && range, const abs_ex & div);

    friend FunctionRange rangeOfSum(const FunctionRange & first, const FunctionRange & second);
    friend FunctionRange rangeOfSubtraction(const FunctionRange & left, const FunctionRange & right);
    friend FunctionRange rangeOfMultiplication(const FunctionRange & left, const FunctionRange & right);
    friend FunctionRange rangeOfDivision(const FunctionRange & left, const FunctionRange & right);

    FunctionRange subtract(const FunctionRange & second) const;
    void normalize();

    QString toString() const;
    QString makeRenderString() const;
    bool isSymmetricRelativelyZero() const;
    bool isEmpty();
    bool containsZero() const;
private:
    void deleteEmptySegments();
    bool valid = true;

    void sort();
    std::list<FunctionRangeSegment> segments;
};
FunctionRange unification(const FunctionRange & a, const FunctionRange & b);

FunctionRange rangeOfSum(const FunctionRange & first, const FunctionRange & second);
FunctionRange rangeOfSubtraction(const FunctionRange & left, const FunctionRange & right);
FunctionRange rangeOfMultiplication(const FunctionRange & left, const FunctionRange & right);
FunctionRange rangeOfDivision(const FunctionRange & left, const FunctionRange & right);

FunctionRange operator+(const FunctionRange & range, const abs_ex & add);
FunctionRange operator-(const FunctionRange & range, const abs_ex & sub);
FunctionRange operator*(const FunctionRange & range, const abs_ex & mult);
FunctionRange operator/(const FunctionRange & range, const abs_ex & div);

FunctionRange operator+(FunctionRange && range, const abs_ex & add);
FunctionRange operator-(FunctionRange && range, const abs_ex & sub);
FunctionRange operator*(FunctionRange && range, const abs_ex & mult);
FunctionRange operator/(FunctionRange && range, const abs_ex & div);

#endif // FUNCTIONRANGE_H
