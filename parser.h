#ifndef PARSER_H
#define PARSER_H
//#include "algebra/algexpr.h"
#include "mathexpression.h"
#include "QString"
#include "scriptsnamespace.h"
enum RequestType {
    REQUEST_TYPE_EXPRESSION,
    REQUEST_TYPE_EQUATION,
    REQUEST_TYPE_ASSIGNATION,
    REQUEST_TYPE_WRONG_REQUEST
};
RequestType defineTypeOfRequest(const QString & request);

std::pair<QString, std::vector<QString>> textParseFunction(const QString & func);
QString deleteOuterBreakets(const QString & expr);
bool isVariable(const QString & expr, const ScriptsNameSpace & scripts_space);
MathExpression parseAndComplete(QString  expr, const ScriptsNameSpace & scripts_space);
QString deleteSpaces(const QString & expr);
bool isAppropriateVarialbeSignature(const QString & str);

#endif // PARSER_H
