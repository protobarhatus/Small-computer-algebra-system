#include "polynomials_factorization.h"
#include "mathsets.h"
Matrix<GfNumber> makeRemaindersCoefficientsMatrix(const Polynom &polynom)
{
    int n = polynom.deg();
    int p = GaluaField::p();
    Matrix<GfNumber> matrix(n, n);
    for (int i = 0; i < n; ++i)
    {
        Polynom x(i*p);
        x[i*p] = 1;
        auto remainder = divide(x, polynom).second;
        remainder.extendToDegree(n - 1);
        matrix[i] = remainder.getVectorOfCoefficients();
    }
    return matrix - makeUnitMatrix<GfNumber>(n);
}

std::vector<Polynom > findNullSpace(Matrix<GfNumber> matrix)
{
    int r = 0;
    int n = matrix.size();
    std::vector<int> c(n, -1);
    std::vector<Polynom> result;
    for (int h = 0; h < n; ++h)
    {
        int j = 0;
        while (j < n && !(matrix[h][j] != 0 && c[j] < 0))
            ++j;

        if (j == n)
        {
            std::vector<GfNumber> new_vector(n);
            for (int j = 0; j < n; ++j)
            {
                if (j == h)
                    new_vector[j] = 1;
                else
                {
                    int k = 0;
                    while (k < n && (c[k] <= 0 || c[k] != j))
                        ++k;
                    if (k == n)
                        new_vector[j] = 0;
                    else
                        new_vector[j] = matrix[h][k];
                }
            }
            result.push_back(Polynom(new_vector));
            result.back().cutZeroDegrees();
            ++r;
        }
        else
        {
            GfNumber multiplier = -1/matrix[h][j];
            for (int i = 0; i < n; ++i)
                matrix[i][j] *= multiplier;
            for (int i = 0; i < n; ++i)
            {
                if (i == j)
                    continue;
                GfNumber mult = matrix[h][i];
                for (int k = 0; k < n; ++k)
                    matrix[k][i] += matrix[k][j] * mult;
            }
            c[j] = h;
        }

    }
    return result;
}

std::list<Polynom> factorizeNormalizedPolynomInGaluaField(const Polynom &polynom)
{
    Matrix<GfNumber> remainders_matrix = makeRemaindersCoefficientsMatrix(polynom);
    std::vector<Polynom> null_space = findNullSpace(std::move(remainders_matrix));
    assert(null_space[0].deg() == 0);
    std::list<Polynom> result;
    result.push_back(polynom);
    if (null_space.size() == 1)
    {

        return result;
    }
    if (GaluaField::p() < 100)
    {
        auto it = result.begin();
        while (it != result.end())
        {
            bool added = false;
            for (int i = 1; i < null_space.size(); ++i)
            {
                for (int s = 0; s < GaluaField::p(); ++s)
                {

                    Polynom number(0);
                    number[0] = s;
                    Polynom second = null_space[i] - number;
                    Polynom factor = gcd(*it, second);
                    if (factor.deg() > 0 && factor.deg() < it->deg())
                    {
                        result.push_back(factor);
                        result.push_back(*it / factor);
                        it = result.erase(it);
                        added = true;
                        break;
                    }

                }
                if (added)
                    break;
            }
            if(!added)
                ++it;
        }
    }
    else
    {
        for (auto &it : null_space)
        {
            Polynom one(0);
            one[0] = 1;
            it = pow(it, (GaluaField::p() - 1)/2) - one;
        }
        auto it = result.begin();
        while (it != result.end())
        {
            bool added = false;
            for (auto &it1 : null_space)
            {
                auto factor = gcd(*it, it1);
                if (factor.deg() > 0 && factor.deg() < it->deg())
                {
                    result.push_back(factor);
                    result.push_back(*it / factor);
                    it = result.erase(it);
                    added = true;
                    break;
                }
            }
            if(!added)
                ++it;
        }
    }
    return result;

}
void substractBigCoefficients(const Polynom & pol, Polynom & res)
{
    if (GaluaField::isOverIntegers())
    {
        res = pol;
        return;
    }
    res = Polynom(pol.deg());
    long long int p = GaluaField::p();
    GaluaField::initialize(0);
    for (int i = 0; i <= pol.deg(); ++i)
        if (pol[i].toInt() > p / 2)
            res[i] = pol[i] - p;
        else
            res[i] = pol[i];
    GaluaField::initialize(p);


}

std::pair<Polynom, Polynom> henselLinearLiftingAlgorithm(int k, Polynom& p, Polynom g, Polynom h)
{
    GaluaField::setLockChangingReverseElements(true);
    long long int first_coe = p[p.deg()].toInt();
    int P = GaluaField::p();

    GaluaField::initialize(0);
    p = p * first_coe;
    GaluaField::initialize(P);
    g = g * (GfNumber(first_coe)/g[g.deg()]).toInt();
    h = h * (GfNumber(first_coe)/h[h.deg()]).toInt();


    auto xea_res = xea(g, h);
    assert (xea_res.first.deg() == 0);
    Polynom a = std::move(xea_res.second.first) / xea_res.first[0];
    Polynom b = std::move(xea_res.second.second) / xea_res.first[0];
    for (int i = 2; i <= k; ++i)
    {
        GaluaField::initialize(0);
        bool s;
        int P_pow = power(P, i - 1, s);
        Polynom c = (p - g*h)/P_pow;
        GaluaField::initialize(P);
        Polynom cgf = c;

        auto alg_res = solveSomePolynomialEquation(a, b, cgf, g, h);
        GaluaField::initialize(P_pow * P);
        g += P_pow * alg_res.second;
        h += P_pow * alg_res.first;
        g.cutZeroDegrees();
        h.cutZeroDegrees();
    }
    GaluaField::setLockChangingReverseElements(false);
    bool s;
    P = power(P, k, s);
    GaluaField::initialize(P);
    g = g * (GfNumber(first_coe) / g[g.deg()]).toInt();
    h = h * (GfNumber(first_coe) / h[h.deg()]).toInt();
    Polynom g_dash(g.deg()), h_dash(h.deg());
    substractBigCoefficients(g, g_dash);
    substractBigCoefficients(h, h_dash);
    GaluaField::initialize(0);
    p = p / first_coe;
    long long int g_gcd = gcd(gcdOfCoefficients(g_dash), first_coe);
    g_dash = g_dash / g_gcd;
    //если эти полиномы раскладывают p в Z, то все коэффициенты h должны разделиться без остатка. если нет - то
    //это итак неверные полиномы
    h_dash = h_dash / (first_coe / g_gcd);
    return {g_dash, h_dash};

    /*Polynom g_dash(g.deg()), h_dash(h.deg());
    substractBigCoefficients(g, g_dash);
    substractBigCoefficients(h, h_dash);
    GaluaField::initialize(0);

    if (g_dash * h_dash == p)
        return {g_dash, h_dash};

    bool s;
    P = power(P, k, s);
    GaluaField::initialize(P);
    GfNumber g_coe = g[g.deg()];
    g.normalize();
    GfNumber h_coe = h[h.deg()];
    h.normalize();
    g = g * h_coe.toInt();
    h = h * g_coe.toInt();
    substractBigCoefficients(g, g_dash);
    substractBigCoefficients(h, h_dash);
    GaluaField::initialize(0);
    return {g_dash, h_dash};*/
}
long long int chooseModBasis(const Polynom & polynom)
{
    std::vector<int>& primes = MathSets::getPrimesVector();
    for (auto &it : primes)
    {
        if (it > 1000)
            return -1;
        if (it == 2 || it == 3)
            continue;
        if (polynom[polynom.deg()].toInt() % it == 0)
            continue;
        GaluaField::initialize(it);
        auto gcd_with_deriv = gcd(polynom, derivative(polynom));
        if (gcd_with_deriv.deg() == 0)
            return it;
    }
    return -1;
}
std::vector<Polynom> move(std::list<Polynom> & list)
{
    std::vector<Polynom> vec(list.size());
    auto itl = list.begin();
    auto itv = vec.begin();
    while (itl != list.end())
    {
        *itv = std::move(*itl);
        ++itv;
        ++itl;
    }
    return vec;
}
std::list<Polynom> factorize(Polynom polynom)
{
    GaluaField::initialize(0);
    std::list<Polynom> res;
    int gcd_of_coefs = gcdOfCoefficients(polynom);
    if (gcd_of_coefs > 1)
    {
        polynom = polynom / gcd_of_coefs;
        res.push_back(numberPolynom(gcd_of_coefs));
    }
    Polynom deriv = derivative(polynom);
    deriv = deriv / gcdOfCoefficients(deriv);
    Polynom gcd_with_der = gcd(polynom, derivative(polynom));
    if (gcd_with_der.deg() > 0)
    {
        polynom = polynom / gcd_with_der;
        res.splice(res.end(), factorize(gcd_with_der));
    }

    long long int p = chooseModBasis(polynom);
    //long long int p = 13;
    //long long int p = 17;
    if (p == -1)
    {
        GaluaField::initialize(0);
        res.push_back(polynom);
        return res;
    }
    GaluaField::initialize(p);
    Polynom polynom_in_gf = polynom;
    auto facts = factorizeNormalizedPolynomInGaluaField(polynom_in_gf);

    if (facts.size() == 1)
    {
        GaluaField::initialize(0);
        res.push_back(polynom);
        return res;
    }
    int k = ceil(log(2*abs( polynom[polynom.deg()].toInt() * lc(polynom)) * amountOfCombination(polynom.deg() / 2, polynom.deg() / 4)*norma2(polynom)) / log(p));


    std::vector<Polynom> facts_vector = move(facts);
    if (facts_vector.size() > 30)
    {
        GaluaField::initialize(0);
        res.push_back(polynom);
        return res;
    }
    bool s;
    int amount_of_variants = power(2, facts_vector.size(), s);
    int variants = 1;
    while (variants < amount_of_variants)
    {
        GaluaField::initialize(p);
        int deg = 0;
        for (int i = 0; i < facts_vector.size(); ++i)
            if (variants & (1 << i))
                deg += facts_vector[i].deg();
        if (deg > polynom.deg()/2)
        {
            ++variants;
            continue;
        }
        Polynom first_mult = numberPolynom(1);
        for (int i = 0; i < facts_vector.size(); ++i)
            if (variants & (1 << i))
                first_mult *= facts_vector[i];
        Polynom second_mult = polynom_in_gf / first_mult;
        auto hensel_res = henselLinearLiftingAlgorithm(k, polynom, first_mult, second_mult);
        if (hensel_res.first * hensel_res.second == polynom)
        {
            res.splice(res.end(), factorize(hensel_res.first));
            res.splice(res.end(), factorize(hensel_res.second));
            return res;
        }
        ++variants;
    }
    GaluaField::initialize(0);
    res.push_back(polynom);
    return res;

    /*std::multimap<int, Polynom> facts_map;
    for (auto &it : facts)
        facts_map.insert({it.deg(), it});


        std::multiset<int> degrees;
        for (auto &it : facts_map)
            degrees.insert(it.first);
        bool found_factors = false;
        for (int i = 1; i <= polynom.deg()/2;++i)
        {
            auto deg_distribution = findSubsetWithSum(degrees, i);
            if (deg_distribution.empty())
                continue;
            GaluaField::initialize(p);
            Polynom first_mult = numberPolynom(1);

            std::multimap<int, Polynom>::iterator iter = facts_map.find(*deg_distribution.begin());
            first_mult *= iter->second;
            auto set_iter = ++deg_distribution.begin();
            while (set_iter != deg_distribution.end())
            {
                if (*set_iter == *std::prev(set_iter))
                {
                    ++iter;
                }
                else
                    iter = facts_map.find(*set_iter);
                first_mult *= iter->second;
                ++set_iter;
            }
            Polynom second_mult = polynom;
            second_mult = polynom / first_mult;
            auto hensel_res = henselLinearLiftingAlgorithm(k, polynom, first_mult, second_mult);
            //GaluaField::isOverIntegers() сейчас true
            if (hensel_res.first * hensel_res.second == polynom)
            {
                res.splice(res.end(), factorize(hensel_res.first));
                res.splice(res.end(), factorize(hensel_res.second));
                found_factors = true;
            }

        }
        if (!found_factors)
            res.push_back(polynom);
        return res;*/


        /*
         *  while (facts_map.size() != 1)
        {
std::list<Polynom>::iterator first_mult = facts.begin();
        std::pair<Polynom, Polynom> hensel_res;
        while (first_mult != facts.end())
        {
            GaluaField::initialize(p);
            Polynom second_mult = polynom;
            second_mult = second_mult / *first_mult;

            hensel_res = henselLinearLiftingAlgorithm(k, polynom, *first_mult, second_mult);
            //GaluaField::p() сейчас = 0
            if (hensel_res.first * hensel_res.second != polynom)
            {
                ++first_mult;
            }
            else
                break;
        }
        if (first_mult == facts.end())
        {
            res.push_back(polynom);
            return res;
        }
        res.push_back(hensel_res.first);
        facts.erase(first_mult);
        polynom = polynom / res.back();
}

    res.push_back(polynom);
    return res;*/
}
