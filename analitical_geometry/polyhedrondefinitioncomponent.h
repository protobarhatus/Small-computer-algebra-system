#ifndef POLYHEDRONDEFINITIONCOMPONENT_H
#define POLYHEDRONDEFINITIONCOMPONENT_H
#include <vector>
#include <QString>
#include "algebra/algexpr.h"
class Polyhedron;
class PolyhedronDefinitionComponent
{
public:
    PolyhedronDefinitionComponent(Polyhedron * pol);
    virtual ~PolyhedronDefinitionComponent();
    virtual AlgExpr volume() = 0;
    virtual std::unique_ptr<PolyhedronDefinitionComponent> copy() const = 0;
protected:
    Polyhedron * parent;
};

class PrizmDefinition : public PolyhedronDefinitionComponent
{
public:
    PrizmDefinition(const std::vector<QString> & lower_base, const std::vector<QString> & upper_base, Polyhedron * parent);
    AlgExpr volume() override;
    std::unique_ptr<PolyhedronDefinitionComponent> copy() const override;
private:
    std::vector<QString> lower_base;
    std::vector<QString> upper_base;
};

class PyramidDefinition : public PolyhedronDefinitionComponent
{
public:
    PyramidDefinition(const std::vector<QString> & base, const QString & top_vertex, Polyhedron * parent);
    AlgExpr volume() override;
    std::unique_ptr<PolyhedronDefinitionComponent> copy() const override;
private:
    std::vector<QString> base;
    QString top_vertex;
};

#endif // POLYHEDRONDEFINITIONCOMPONENT_H
