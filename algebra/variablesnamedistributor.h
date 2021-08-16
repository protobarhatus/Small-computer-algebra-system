#ifndef VARIABLESNAMEDISTRIBUTOR_H
#define VARIABLESNAMEDISTRIBUTOR_H
#include <map>
#include <QString>
#include <set>
class VariablesNameDistributor
{
public:
    static VariablesNameDistributor& get();
    static void addVariable(int id, QString name);
    static QString getName(int id);

    static QString getNextIndexedName(const QString & name);

    static void _clear();
private:
    VariablesNameDistributor();
    VariablesNameDistributor(const VariablesNameDistributor &) = delete;
    void operator=(const VariablesNameDistributor &) = delete;
    std::map<int, QString> vars_table;
    std::set<QString> used_names;
};

#endif // VARIABLESNAMEDISTRIBUTOR_H
