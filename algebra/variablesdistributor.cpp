#include "variablesdistributor.h"
#include "variablesnamedistributor.h"
#include "exception.h"
VariablesDistributor::VariablesDistributor()
{
    this->system_var_def = new VariablesDefinition;
    Variable::system_id_counter = this->first_system_num;
    Variable::integrating_constant_id_counter = this->first_integrate_constant;
}

void VariablesDistributor::increaseIntegratingConstant(int id)
{
    VariablesDistributor::get().amount_of_integrating_constants[id - VariablesDistributor::get().first_integrate_constant]++;
}

void VariablesDistributor::decreaseIntegratingConstant(int id)
{
    VariablesDistributor::get().amount_of_integrating_constants[id - VariablesDistributor::get().first_integrate_constant]--;
}
VariablesDistributor& VariablesDistributor::get()
{
    static VariablesDistributor inst;
    return inst;
}
QString makeVariablesName(int id)
{
    if (VariablesDistributor::hasSpecialName(id))
        return VariablesDistributor::getSpecialName(id);
    if (id >= VariablesDistributor::firstIntegrateConstant())
        return makeIntegratingConstantName(id);
    if (id >= VariablesDistributor::firstSystemNum())
    {
        id = id - VariablesDistributor::firstSystemNum();
        QString name = "$system";
        while (id != 0)
        {
            name += QChar(id % 26 + 97);
            id /= 26;
        }
        return name;
    }
    //функция создавалась с расчетом на то, что нумерация идет с 0, но я забыл, что она должна идти с 1. поэтому мне проще сделать так
    id -= 1;
    if (id == 0)
        return "x";
    if (id == 1)
        return "y";
    if (id == 2)
        return "z";
    if (id >= 3 && id <= 25)
        return QChar(id + 94);
    if (id >= 26 && id <= 51)
        return QChar(id + 39);
    QString name = "$";
    while (id != 0)
    {
        name += QChar(id % 26 + 97);
        id /= 26;
    }
    return name;
}
Variable VariablesDistributor::createVariable(VariablesDefinition definition)
{
    if (Variable::id_counter >= get().first_system_num - 1)
        throw Exception();
    ++Variable::id_counter;
    QString name = makeVariablesName(Variable::id_counter);
    VariablesNameDistributor::addVariable(Variable::id_counter, name);
    VariablesDistributor& distr = VariablesDistributor::get();
    distr.variables.push_back(new VariablesDefinition(definition));
    return Variable(Variable::id_counter, distr.variables[Variable::id_counter - 1]);
}

Variable VariablesDistributor::createVariable(VariablesDefinition definition, const QString &name)
{
    if (Variable::id_counter >= get().first_system_num - 1)
        throw Exception();
    ++Variable::id_counter;
    VariablesNameDistributor::addVariable(Variable::id_counter, name);
    VariablesDistributor& distr = VariablesDistributor::get();
    distr.variables.push_back(new VariablesDefinition(definition));
    return Variable(Variable::id_counter, distr.variables[Variable::id_counter - 1]);
}
void VariablesDistributor::deleteVariables()
{
    for (auto &it : this->variables)
        delete it;
    this->variables.clear();
}
void deleteVariables()
{
    VariablesDistributor::get().deleteVariables();
    Variable::id_counter = 0;
}
VariablesDefinition * VariablesDistributor::getVariablesDefinition(int id)
{

    VariablesDistributor & ref = VariablesDistributor::get();
    //if (id > ref.variables.size())
      //  return ref.system_var_def;
    if (id >= ref.first_integrate_constant)
        return VariablesDistributor::get().integrating_constants[id - ref.first_integrate_constant];
    if (id >= ref.first_system_num)
        return ref.system_variables[id - ref.first_system_num];
    return ref.variables[id - 1];
}
Variable getVariable(int id)
{
    if (id >= VariablesDistributor::get().first_system_num)
        return systemVar(id);
    return Variable(id, VariablesDistributor::getVariablesDefinition(id));
}
int VariablesDistributor::firstSystemNum()
{
    return VariablesDistributor::get().first_system_num;
}

VariablesDistributor::~VariablesDistributor()
{
    for (auto &it : this->variables)
        delete it;
}

int VariablesDistributor::firstIntegrateConstant()
{
    return VariablesDistributor::get().first_integrate_constant;
}

int VariablesDistributor::amountOfVariable(int i)
{
    if (i >= Variable::integrating_constant_id_counter)
        return 0;
    return VariablesDistributor::get().amount_of_integrating_constants[i -
            VariablesDistributor::get().first_integrate_constant];
}

void VariablesDistributor::addSpecialName(int index, const QString &name)
{
    VariablesDistributor::get().special_names.insert({index, name});
}

bool VariablesDistributor::hasSpecialName(int id)
{
    return VariablesDistributor::get().special_names.find(id) != VariablesDistributor::get().special_names.end();
}

QString VariablesDistributor::getSpecialName(int id)
{
    return VariablesDistributor::get().special_names.find(id)->second;
}
Variable systemVar(int num)
{
    //return Variable(num + VariablesDistributor::firstSystemNum(), makeVariablesName(num + VariablesDistributor::firstSystemNum()));
    return Variable(num, makeVariablesName(num));
}

Variable systemVar()
{
    ++Variable::system_id_counter;

    VariablesDistributor::get().system_variables.push_back(new VariablesDefinition(*VariablesDistributor::get().system_var_def));
    return Variable(Variable::system_id_counter - 1, makeVariablesName(Variable::system_id_counter - 1));
}

Variable integratingConstant()
{
    VariablesDistributor& ref = VariablesDistributor::get();
    ref.integrating_constants.push_back(new VariablesDefinition(FunctionRange(nullptr, nullptr, false, false)));
    VariablesDistributor::get().amount_of_integrating_constants.push_back(0);
    ++Variable::integrating_constant_id_counter;
    return Variable(Variable::integrating_constant_id_counter - 1,
                    makeIntegratingConstantName(Variable::integrating_constant_id_counter - 1));
}

Variable systemVar(int min, int max)
{
    ++Variable::system_id_counter;
    VariablesDefinition * new_def = new VariablesDefinition(FunctionRange(FunctionRangeSegment(numToAbs(min),
                                                                                               numToAbs(max),
                                                                                               true, true)));
    VariablesDistributor::get().system_variables.push_back(new_def);

    Variable new_var = Variable(Variable::system_id_counter - 1, makeVariablesName(Variable::system_id_counter - 1));
    return new_var;
}

abs_ex integratingConstantExpr()
{
    return abs_ex(new Variable(integratingConstant()));
}

abs_ex systemVarExpr()
{
    return abs_ex(new Variable(systemVar()));
}

abs_ex getVariableExpr(int id)
{
    return abs_ex (new Variable(getVariable(id)));
}

bool isIntegratingConstant(int index)
{
    return index >= VariablesDistributor::get().first_integrate_constant;
}

Variable integratingConstant(const FunctionRange &range)
{
    VariablesDistributor::get().integrating_constants.push_back(new VariablesDefinition(range));
    VariablesDistributor::get().amount_of_integrating_constants.push_back(0);
    ++Variable::integrating_constant_id_counter;
    return Variable(Variable::integrating_constant_id_counter - 1,
                    makeIntegratingConstantName(Variable::integrating_constant_id_counter - 1),
                    VariablesDistributor::get().integrating_constants.back());
}

QString makeIntegratingConstantName(int id)
{
    if (VariablesDistributor::hasSpecialName(id))
        return VariablesDistributor::getSpecialName(id);
    id -= VariablesDistributor::get().first_integrate_constant;
    if (id == 0)
        return "C";
    QString num;
    num.setNum(id);
    return "C"+num;
}

abs_ex integratingConstantExpr(const FunctionRange &range)
{
    return abs_ex(new Variable(integratingConstant(range)));
}
Variable systemVar(const FunctionRange &range)
{
    ++Variable::system_id_counter;
    VariablesDefinition * new_def = new VariablesDefinition(range);
    VariablesDistributor::get().system_variables.push_back(new_def);

    Variable new_var = Variable(Variable::system_id_counter - 1, makeVariablesName(Variable::system_id_counter - 1));
    return new_var;
}

Variable systemVar(const abs_ex &min, const abs_ex &max, bool min_included, bool max_included)
{
    return systemVar(FunctionRange(min, max, min_included, max_included));
}

abs_ex systemVarExpr(const abs_ex &min, const abs_ex &max, bool min_included, bool max_included)
{
    return abs_ex(new Variable(systemVar(min, max, min_included, max_included)));
}

abs_ex integratingConstantExpr(int index)
{
    return abs_ex(new Variable(index));
}

abs_ex integratingConstantExpr(int index, const FunctionRange &range)
{
    VariablesDistributor::get().integrating_constants[index - VariablesDistributor::get().first_integrate_constant]->setRange(range);
    return abs_ex(new Variable(index));
}

bool isIntegratingConstantAndCanChangeIt(int id)
{
    return isIntegratingConstant(id) &&
            VariablesDistributor::get().amount_of_integrating_constants[id - VariablesDistributor::get().first_integrate_constant] == 1;
}

int amountOfIntegratingConstant(int index)
{
    return VariablesDistributor::amountOfVariable(index + 1500000000);
}


abs_ex systemVarExpr(const FunctionRange &range)
{
    return abs_ex(new Variable(systemVar(range)));
}
