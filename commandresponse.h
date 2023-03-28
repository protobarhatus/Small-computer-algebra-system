#ifndef COMMANDRESPONSE_H
#define COMMANDRESPONSE_H
#include <QString>
#include <vector>
enum OUTPUT_STRINGS_TYPES{
    OUTPUT_STRING_RENDER_STRING=0,
    OUTPUT_STRING_WOLFRAM_STRING,
    OUTPUT_STRING_TYPES_AMOUNT
};

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
    const std::vector<QString>& getMessage() const;
private:
    Type _type;
    int input_num;
    std::vector<QString> message;
    friend CommandResponse acceptedInputCommandRespond(int input_num, const std::vector<QString> & out_message);
    friend CommandResponse errorCommandRespond(const std::vector<QString> & message);
    friend CommandResponse skippedCommandRespond();
};
CommandResponse acceptedInputCommandRespond(int input_num, const std::vector<QString> & out_message);
CommandResponse errorCommandRespond(const std::vector<QString> & error_message);
CommandResponse skippedCommandRespond();
#endif // COMMANDRESPONSE_H
