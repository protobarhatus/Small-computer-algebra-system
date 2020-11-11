#include "mainapplicationspace.h"
#include "algebra/algexpr.h"
MainApplicationSpace::MainApplicationSpace()
{

}

template <typename T>
std::vector<T> toStdVector(const QVector<T> & vec)
{
    std::vector<T> res(vec.size());
    for (int i = 0; i < vec.size(); ++i)
        res[i] = vec[i];
    return res;
}
void MainApplicationSpace::launch()
{
    for (int i = 0; i < 26; ++i)
        var();
    this->window = new MainWindow;
    current_script = -1;
    this->window->setMainApplicationSpace(this);
    //this->scripts.push_back(Script());

    window->show();

}

void MainApplicationSpace::executeCurrentScript(const QStringList &commands)
{
    auto comms = toStdVector(commands.toVector());
    this->window->outResult(scripts[current_script].execute(comms), comms);

}

void MainApplicationSpace::addScript()
{
    this->scripts.push_back(Script());
    this->current_script = this->scripts.size() - 1;
}

void MainApplicationSpace::changeScript(int new_index)
{
    this->current_script = new_index;
}

MainWindow *MainApplicationSpace::getWindow() const
{
    return this->window;
}
