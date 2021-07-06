#include "polyhedrondefinitioncomponent.h"
#include "polyhedron.h"
#include "geometry_3d.h"
PolyhedronDefinitionComponent::PolyhedronDefinitionComponent(Polyhedron * pol)
{
    this->parent = pol;
}

PolyhedronDefinitionComponent::~PolyhedronDefinitionComponent()
{

}

PrizmDefinition::PrizmDefinition(const std::vector<QString> &lower_base, const std::vector<QString> &upper_base,
                                 Polyhedron *parent) : PolyhedronDefinitionComponent(parent)
{
    this->lower_base = lower_base;
    this->upper_base = upper_base;
}

AlgExpr PrizmDefinition::volume()
{
    std::vector<AlgVector> base(this->lower_base.size());
    for (int i = 0; i < this->lower_base.size(); ++i)
        base[i] = (*this->parent)[this->lower_base[i]];
    AlgExpr height = distance((*this->parent)[upper_base[0]], getPlaneThroughThreePoints(base[0], base[1], base[2]));
    return area(base) * height;
}

std::unique_ptr<PolyhedronDefinitionComponent> PrizmDefinition::copy() const
{
    return std::unique_ptr<PolyhedronDefinitionComponent>(new PrizmDefinition(*this));
}

PyramidDefinition::PyramidDefinition(const std::vector<QString> &base, const QString &top_vertex, Polyhedron *parent) :
    PolyhedronDefinitionComponent(parent)
{
    this->base = base;
    this->top_vertex = top_vertex;
}

AlgExpr PyramidDefinition::volume()
{
    std::vector<AlgVector> base(this->base.size());
    for (int i = 0; i < this->base.size(); ++i)
        base[i] = (*this->parent)[this->base[i]];
    AlgExpr height = distance((*this->parent)[top_vertex], getPlaneThroughThreePoints(base[0], base[1], base[2]));
    return area(base) * height / 3;
}

std::unique_ptr<PolyhedronDefinitionComponent> PyramidDefinition::copy() const
{
    return std::make_unique<PyramidDefinition>(*this);
}
