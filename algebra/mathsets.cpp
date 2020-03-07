#include "mathsets.h"
#include <cmath>
MathSets::MathSets()
{

}
void MathSets::init(MathSets & set)
{

    if (set.has_initialized)
        return;
    set.has_initialized = true;
    //простые числа до 2^20 будут занимать примерно 512 кб и будут строиться всего пару секунд, что вполне удовлетворительно
    set.makePrimeNumbers(pow(2, 20));
}
MathSets& MathSets::getInstance()
{
    static MathSets sets;
    if (!sets.has_initialized)
        sets.init(sets);
    return sets;
}
void MathSets::makePrimeNumbers(long long int N)
{
    this->primes_vec.push_back(2);
    this->primes_set.insert(2);
    long long int root = sqrt(N);
    std::vector<bool> values(N + 1, false);

    for (int i = 3; i <= root; i += 2)
    {
        if (!values[i])
        {
            this->primes_set.insert(i);
            this->primes_vec.push_back(i);
            for (int k = i; k <= N; k += i)
            {
                values[k] = true;
            }
        }
    }
    for (long long int i = root + 1 + root % 2; i <= N; i += 2)
        if (!values[i])
        {
            this->primes_vec.push_back(i);
            this->primes_set.insert(i);
        }



}
std::vector<int> & MathSets::getPrimesVector()
{
    return MathSets::getInstance().primes_vec;
}
std::set<int> & MathSets::getPrimesSet()
{
    return MathSets::getInstance().primes_set;
}
