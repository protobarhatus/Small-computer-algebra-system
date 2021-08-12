#ifndef PHMODEL_H
#define PHMODEL_H
#include "algebra/algexpr.h"
#include "vector"
#include "phobject.h"

typedef std::unique_ptr<PhObject> PhObjectPtr;



class PhModel
{
public:
    PhModel();

    int addObject(const QString & name, PhObjectPtr && object);

    PhObjectPtr& operator[](int id);
    const PhObjectPtr& operator[](int id) const;

    void completeContinuityCicle();

    PhObjectPtr makeSimpleKinematicObject(const AlgVector & pos, const AlgVector & vel, const AlgVector & acc);

    void setDimensionNumber(int d);
private:

    AlgVector createPositionUknownVector();
    AlgVector createVelocityUknownVector();
    AlgVector createAccelerationUknownVector();

    std::vector<AlgExpr> timeStagesMarks;

    std::vector<PhObjectPtr> objects;
    std::vector<QString> objects_names;

    int dimensions;
};



Variable getTimeArgumentVariable();
#endif // PHMODEL_H
