#ifndef SCRIPT_H
#define SCRIPT_H
#include <vector>
#include <QString>
#include "commandresponse.h"
QChar systemUnreadableSymbol();
class Script
{
public:
    Script();
    Script(const Script & script);
    Script(Script && script);
    Script& operator=(const Script & script);
    Script& operator=(Script && script);

    void setCommands(const std::vector<QString> & commands);
    std::vector<CommandResponse> execute(const std::vector<QString> & commands);
private:
    std::vector<QString> commands;
};

#endif // SCRIPT_H
