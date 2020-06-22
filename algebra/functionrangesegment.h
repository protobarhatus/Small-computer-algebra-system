#ifndef FUNCTIONRANGESEGMENT_H
#define FUNCTIONRANGESEGMENT_H
//#include "abstractexpression.h"
#include <memory>
#include <list>
#include <QString>
class AbstractExpression;
typedef std::unique_ptr<AbstractExpression> abs_ex;
class FunctionRangeSegment
{

public:
    FunctionRangeSegment();
    FunctionRangeSegment(const abs_ex& imin, const abs_ex& imax, bool is_min_included, bool is_max_included);
    FunctionRangeSegment(const FunctionRangeSegment & segm);
    FunctionRangeSegment(FunctionRangeSegment && segm);
    FunctionRangeSegment& operator=(const FunctionRangeSegment & segm);
    FunctionRangeSegment& operator=(FunctionRangeSegment && segm);

    void setMin(const abs_ex & min, bool included);
    void setMax(const abs_ex & max, bool included);
    const abs_ex& min() const;
    const abs_ex& max() const;
    abs_ex & min();
    abs_ex & max();
    bool isMinIncluded() const;
    bool isMaxIncluded() const;

    FunctionRangeSegment& operator*=(const abs_ex & mult);
    FunctionRangeSegment& operator/=(const abs_ex & mult);
    FunctionRangeSegment& operator+=(const abs_ex & add);
    FunctionRangeSegment& operator-=(const abs_ex & sub);
    bool isPoint() const;

    bool canBeLowerThanZero() const;
    bool canBeBiggerThanZero() const;

    QString toString() const;
    bool isEmpty();
private:
    abs_ex _min;
    abs_ex _max;
    bool min_included;
    bool max_included;
};

FunctionRangeSegment operator+(const FunctionRangeSegment& left, const FunctionRangeSegment & right);
FunctionRangeSegment operator-(const FunctionRangeSegment& left, const FunctionRangeSegment & right);
FunctionRangeSegment operator*(const FunctionRangeSegment& left, const FunctionRangeSegment& right);
std::list<FunctionRangeSegment> operator/(const FunctionRangeSegment& left, const FunctionRangeSegment& right);
#endif // FUNCTIONRANGESEGMENT_H
