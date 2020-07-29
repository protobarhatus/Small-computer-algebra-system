#ifndef COMMANDRESPONSE_H
#define COMMANDRESPONSE_H
#include <QString>

class CommandResponse
{
public:
    CommandResponse();
    CommandResponse(const CommandResponse & resp);
    CommandResponse& operator=(const CommandResponse & resp);
    enum Type{
        ACCEPTED_INPUT,
        ERROR,
        SKIPPED
    };
    Type type() const;
    int inputNumber() const;
    QString getMessage() const;
private:
    Type _type;
    int input_num;
    QString message;
    friend CommandResponse acceptedInputCommandRespond(int input_num, const QString & out_message);
    friend CommandResponse errorCommandRespond(const QString & message);
    friend CommandResponse skippedCommandRespond();
};
CommandResponse acceptedInputCommandRespond(int input_num, const QString & out_message);
CommandResponse errorCommandRespond(const QString & error_message);
CommandResponse skippedCommandRespond();
#endif // COMMANDRESPONSE_H
