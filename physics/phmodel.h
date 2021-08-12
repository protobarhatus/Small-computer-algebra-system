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

    void addEquation(AlgExpr && equation);
    void addEquation(const AlgExpr & equation);
    void addEquation(AlgVector && equation);
    void addEquation(const AlgVector & equation);


private:

    AlgExpr findContinuityCicleEnd();
    void solveEquations();

    void recountAllWithNewVariablesValues(const std::map<int, abs_ex> & vars_values);

    std::vector<abs_ex> equations;

    AlgVector createPositionUknownVector();
    AlgVector createVelocityUknownVector();
    AlgVector createAccelerationUknownVector();

    std::vector<AlgExpr> timeStagesMarks;

    std::vector<PhObjectPtr> objects;
    std::vector<QString> objects_names;

    std::set<int> known_variables;

    int dimensions;
};



Variable getTimeArgumentVariable();
#endif // PHMODEL_H
