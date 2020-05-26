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
long long int gcdOfCoefficients(const Polynom & polynom)
{
    long long int gcf = polynom[0].toInt();
    for (int i = 1; i < polynom.size(); ++i)
        gcf = gcd(gcf, polynom[i].toInt());
    return gcf;
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

    int p = 13;
    GaluaField::initialize(p);
    Polynom polynom_in_gf = polynom;
    auto facts = factorizeNormalizedPolynomInGaluaField(polynom_in_gf);

    if (facts.size() == 1)
    {
        GaluaField::initialize(0);
        res.push_back(polynom);
        return res;
    }
    int k = ceil(log(2*abs(lc(polynom) * polynom[polynom.deg()].toInt()) * amountOfCombination(polynom.deg() / 2, polynom.deg() / 4)) / log(p));
    while (facts.size() != 1)
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
    return res;
}
