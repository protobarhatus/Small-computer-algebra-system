#include "variablesnamedistributor.h"

VariablesNameDistributor::VariablesNameDistributor()
{

}
VariablesNameDistributor& VariablesNameDistributor::get()
{
    static VariablesNameDistributor distr;
    return distr;
}
void VariablesNameDistributor::addVariable(int id, QString name)
{
    VariablesNameDistributor::get().vars_table.insert(std::pair<int, QString>(id, name));
}
QString VariablesNameDistributor::getName(int id)
{
    return VariablesNameDistributor::get().vars_table.find(id)->second;
}
