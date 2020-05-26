#ifndef POLYNOMIALS_FACTORIZATION_H
#define POLYNOMIALS_FACTORIZATION_H
#include "polynom.h"
#include "Matrix.h"


//действия этих функций происходят в конечном поле галуа, его размерность определяется в
//инициировании разложения на множители
Matrix<GfNumber> makeRemaindersCoefficientsMatrix(const Polynom & polynom);
std::vector<Polynom> findNullSpace(Matrix<GfNumber> matrix);
//полином должен быть без степеней, то есть gcd(p(x), p'(x)) = 1
std::list<Polynom> factorizeNormalizedPolynomInGaluaField(const Polynom & polynom);
//на вход: p в Z[x], g в GF(p), h в GF(p) p = g*h в GF(p), k, на выход: gk, hk такие, что p = gk * hk в GF(p^k)
std::pair<Polynom, Polynom> henselLinearLiftingAlgorithm(int k, Polynom &p,
                                                         Polynom  g,  Polynom  h);
std::list<Polynom> factorize(Polynom  polynom);
#endif // POLYNOMIALS_FACTORIZATION_H
