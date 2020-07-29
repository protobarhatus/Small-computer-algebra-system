#ifndef MATHSETS_H
#define MATHSETS_H
#include <vector>
#include <set>
class MathSets
{
public:

    static std::vector<int> & getPrimesVector();
    static std::set<int> & getPrimesSet();
    static const std::vector<std::vector<long long int>> & getBinomialCoefs();
    static const std::vector<long long int> & getBinomialCoefs(int degree);
    static long long int combinations(int n, int k);
    static int maxDegreeOfBinomialCoefs();
private:
    static void init(MathSets &);
    MathSets();
    MathSets(const MathSets &) = delete;
    MathSets& operator=(const MathSets &) = delete;
    bool has_initialized = false;
    std::vector<int> primes_vec;
    std::set<int> primes_set;
    void makePrimeNumbers(long long int N);
    static MathSets& getInstance();
    std::vector<std::vector<long long int>> binomial_coefficients;
    void makeBinomialCoefficients(int n);
};

#endif // MATHSETS_H
