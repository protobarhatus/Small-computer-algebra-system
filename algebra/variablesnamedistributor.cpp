#include "variablesnamedistributor.h"
#include <assert.h>
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
    VariablesNameDistributor::get().used_names.insert(name);
    VariablesNameDistributor::get().names_vars_table.insert({name, id});
}
QString VariablesNameDistributor::getName(int id)
{
    return VariablesNameDistributor::get().vars_table.find(id)->second;
}

QString VariablesNameDistributor::getNextIndexedName(const QString &name)
{
    auto it = VariablesNameDistributor::get().used_names.find(name);
    if (it == VariablesNameDistributor::get().used_names.end())
        return name;

    QString last_num = "0";
    while (it != VariablesNameDistributor::get().used_names.end() && it->contains(name))
    {
        auto num = it->right(it->size() - name.size());
        bool is_num = true;
        for (auto &it : num)
            if (!(it >= '0' && it <= '9'))
                is_num = false;

        if (is_num)
            last_num = num;
        ++it;
    }
    int num = last_num.toInt();
    QString str;
    str.setNum(num + 1);
    return name + str;
}

bool VariablesNameDistributor::hasName(const QString &name)
{
    return VariablesNameDistributor::get().used_names.find(name) != VariablesNameDistributor::get().used_names.end();
}

int VariablesNameDistributor::getId(const QString &name)
{
    assert(VariablesNameDistributor::hasName(name));
    return VariablesNameDistributor::get().names_vars_table.find(name)->second;
}

void VariablesNameDistributor::_clear()
{
    VariablesNameDistributor::get().vars_table.clear();
    VariablesNameDistributor::get().used_names.clear();
    VariablesNameDistributor::get().names_vars_table.clear();
}
