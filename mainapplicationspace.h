#ifndef MAINAPPLICATIONSPACE_H
#define MAINAPPLICATIONSPACE_H
#include "mainwindow.h"
#include "script.h"
class MainApplicationSpace
{
public:
    MainApplicationSpace();
    void launch();

    void executeCurrentScript(const QStringList & commands);
    void addScript();
    void changeScript(int new_index);
    MainWindow * getWindow() const;
private:

    MainWindow * window;
    std::vector<Script> scripts;
    int current_script;
};

#endif // MAINAPPLICATIONSPACE_H
