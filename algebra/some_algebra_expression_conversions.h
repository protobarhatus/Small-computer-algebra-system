#ifndef SOME_ALGEBRA_EXPRESSION_CONVERSIONS_H
#define SOME_ALGEBRA_EXPRESSION_CONVERSIONS_H
#include "abstractexpression.h"
#include <memory>

class Polynomial;
class Fractal;


//std::unique_ptr<AbstractExpression> makeAbstractExpression(AlgebraExpression type, const AbstractExpression * argument);
bool has(const std::set<int> & set, int el);

abs_ex copy(const AbstractExpression * arg);
abs_ex copy(const abs_ex & arg);
abs_ex copyWithLiftingIntegrationConstantsThatCanBeChanged(const abs_ex & expr);
abs_ex copyWithLiftingIntegrationConstantsThatCanBeChanged(const AbstractExpression * expr);
abs_ex licCopy(const AbstractExpression * expr);
abs_ex licCopy(const abs_ex & expr);
long long int factorial(long long int N);
long long int gcd(long long int a, long long int b);
//extended euclidean algorithm; returns {d, {x, y}} that d = x*a+y*b and d = gcd(a, b)
std::pair<long long int, std::pair<long long int, long long int>> xea(long long int a, long long int b);
long long int lcm(long long int a, long long int b);
long long int root(long long int a, long long int n, bool & succesfully);
long long int power(long long int a, long long int n, bool & succesfully);
long long int power(long long int a, long long int n);
bool hasIntersections(const std::set<int> & a, const std::set<int> & b);

std::multiset<int> findSubsetWithSum(const std::multiset<int> & set, int n);
std::unique_ptr<AbstractExpression> getDegrCoefficientPtr(long long int degree, const std::vector<int> & members_degrees);
//polynomials must be simplified and reduced
std::unique_ptr<Polynomial> gcd(Polynomial * a, Polynomial * b);
long long int amountOfCombination(long long int from_what, long long int set);
typedef std::vector<int> VariablesCombinations;
void init(VariablesCombinations & combinations, int root);
bool inc(VariablesCombinations & combinations);
std::pair<bool, std::unique_ptr<Fractal>> getPiMember(abs_ex & expr);
bool isPiMemberInTable(Number coe);
bool isPiMemberOnAxis(Number coe);
abs_ex toAbsEx( AbstractExpression * expr);
void liftAllIntegratingConstants(abs_ex & expr);
QString unicode(int sym);
#endif // SOME_ALGEBRA_EXPRESSION_CONVERSIONS_H
