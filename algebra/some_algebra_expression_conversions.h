#ifndef SOME_ALGEBRA_EXPRESSION_CONVERSIONS_H
#define SOME_ALGEBRA_EXPRESSION_CONVERSIONS_H
#include "abstractexpression.h"
#include <memory>

class Polynomial;
class Fractal;

std::unique_ptr<AbstractExpression> makeAbstractExpression(AlgebraExpression type, AbstractExpression * argument);
abs_ex copy(AbstractExpression * arg);
abs_ex copy(abs_ex & arg);
long long int factorial(long long int N);
long long int gcd(long long int a, long long int b);
long long int lcm(long long int a, long long int b);
long long int root(long long int a, long long int n, bool & succesfully);
long long int power(long long int a, long long int n, bool & succesfully);
std::unique_ptr<AbstractExpression> getDegrCoefficientPtr(long long int degree, const std::vector<int> & members_degrees);
//polynomials must be simplified and reduced
std::unique_ptr<Polynomial> gcd(Polynomial * a, Polynomial * b);
long long int amountOfCombination(long long int from_what, long long int set);
typedef std::vector<int> VariablesCombinations;
void init(VariablesCombinations & combinations, int root);
bool inc(VariablesCombinations & combinations);
std::pair<bool, std::unique_ptr<Fractal>> getPiMember(abs_ex & expr);
bool isPiMemberInTable(Number coe);
#endif // SOME_ALGEBRA_EXPRESSION_CONVERSIONS_H
