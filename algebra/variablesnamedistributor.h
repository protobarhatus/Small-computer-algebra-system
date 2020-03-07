#ifndef VARIABLESNAMEDISTRIBUTOR_H
#define VARIABLESNAMEDISTRIBUTOR_H
#include <map>
#include <QString>
class VariablesNameDistributor
{
public:
    static VariablesNameDistributor& get();
    static void addVariable(int id, QString name);
    static QString getName(int id);
private:
    VariablesNameDistributor();
    VariablesNameDistributor(const VariablesNameDistributor &) = delete;
    void operator=(const VariablesNameDistributor &) = delete;
    std::map<int, QString> vars_table;
};

#endif // VARIABLESNAMEDISTRIBUTOR_H
