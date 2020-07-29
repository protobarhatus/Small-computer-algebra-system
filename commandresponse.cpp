#include "commandresponse.h"

CommandResponse::CommandResponse()
{

}

CommandResponse::CommandResponse(const CommandResponse &resp) : _type(resp._type), input_num(resp.input_num), message(resp.message)
{

}

CommandResponse &CommandResponse::operator=(const CommandResponse &resp)
{
    this->_type = resp._type;
    this->input_num = resp.input_num;
    this->message = resp.message;
    return *this;
}

int CommandResponse::inputNumber() const
{
    return this->input_num;
}

QString CommandResponse::getMessage() const
{
    return this->message;
}

CommandResponse::Type CommandResponse::type() const
{
    return _type;
}

CommandResponse acceptedInputCommandRespond(int input_num, const QString & out_message)
{
    CommandResponse resp;
    resp._type = CommandResponse::ACCEPTED_INPUT;
    resp.input_num = input_num;
    resp.message = out_message;
    return resp;
}

CommandResponse errorCommandRespond(const QString &error_message)
{
    CommandResponse resp;
    resp._type = CommandResponse::ERROR;
    resp.message = error_message;
    return resp;
}

CommandResponse skippedCommandRespond()
{
    CommandResponse resp;
    resp._type = CommandResponse::SKIPPED;
    return resp;
}
