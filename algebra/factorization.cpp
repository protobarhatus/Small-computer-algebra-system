#include "factorization.h"
#include "some_algebra_expression_conversions.h"
#include "primetest.h"
#include <cmath>
#include <map>
#include "mathsets.h"
#include <algorithm>
//большая часть кода взята с http://www.e-maxx-ru.1gb.ru/algo/factorization

template <class T>
bool even(const T & n)
{
    // return n % 2 == 0;
    return (n & 1) == 0;
}

//! ����� ����� �� 2
template <class T>
void bisect(T & n)
{
    // n /= 2;
    n >>= 1;
}

//! �������� ����� �� 2
template <class T>
void redouble(T & n)
{
    // n *= 2;
    n <<= 1;
}

//! ���������� true, ���� n - ������ ������� �������� �����
template <class T>
bool perfect_square(const T & n)
{
    T sq = (T)ceil(sqrt((double)n));
    return sq * sq == n;
}

//! ��������� ������ �� �����, �������� ��� ����
template <class T>
T sq_root(const T & n)
{
    return (T)floor(sqrt((double)n));
}

//! ���������� ���������� ��� � ����� (�.�. ����������� ���������� ���, �������� ����� ����������� ������ �����)
template <class T>
unsigned bits_in_number(T n)
{
    if (n == 0)
        return 1;
    unsigned result = 0;
    while (n)
    {
        bisect(n);
        ++result;
    }
    return result;
}

//! ���������� �������� k-�� ���� ����� (���� ���������� � ����)
template <class T>
bool test_bit(const T & n, unsigned k)
{
    return (n & (T(1) << k)) != 0;
}

//! �������� a *= b (mod n)
template <class T>
void mulmod(T & a, T b, const T & n)
{
    // ������� ������, ������� ������ ��� ������� ����������
    a *= b;
    a %= n;
}

template <>
void mulmod(int & a, int b, const int & n)
{
    a = int((((long long)a) * b) % n);
}

template <>
void mulmod(unsigned & a, unsigned b, const unsigned & n)
{
    a = unsigned((((unsigned long long)a) * b) % n);
}

template <>
void mulmod(unsigned long long & a, unsigned long long b, const unsigned long long & n)
{
    // ������� ������, ���������� �� �������� ���������� ������������ � �����
    if (a >= n)
        a %= n;
    if (b >= n)
        b %= n;
    unsigned long long res = 0;
    while (b)
        if (!even(b))
        {
            res += a;
            while (res >= n)
                res -= n;
            --b;
        }
        else
        {
            redouble(a);
            while (a >= n)
                a -= n;
            bisect(b);
        }
    a = res;
}

template <>
void mulmod(long long & a, long long b, const long long & n)
{
    bool neg = false;
    if (a < 0)
    {
        neg = !neg;
        a = -a;
    }
    if (b < 0)
    {
        neg = !neg;
        b = -b;
    }
    unsigned long long aa = a;
    mulmod<unsigned long long>(aa, (unsigned long long)b, (unsigned long long)n);
    a = (long long)aa * (neg ? -1 : 1);
}


//! ��������� a^k (mod n). ���������� �������� ���������� � �������
template <class T, class T2>
T powmod(T a, T2 k, const T & n)
{
    T res = 1;
    while (k)
        if (!even(k))
        {
            mulmod(res, a, n);
            --k;
        }
        else
        {
            mulmod(a, a, n);
            bisect(k);
        }
    return res;
}

//! ��������� ����� n � ����� q*2^p
template <class T>
void transform_num(T n, T & p, T & q)
{
    T p_res = 0;
    while (even(n))
    {
        ++p_res;
        bisect(n);
    }
    p = p_res;
    q = n;
}

//! �������� �������
template <class T, class T2>
T gcd(const T & a, const T2 & b)
{
    return (a == 0) ? b : gcd(b % a, a);
}

//! ��������� jacobi(a,b)
template <class T>
T jacobi(T a, T b)
{

#pragma warning (push)
#pragma warning (disable: 4146)

    if (a == 0)
        return 0;
    if (a == 1)
        return 1;

    if (a < 0)
        if ((b & 2) == 0)
            return jacobi(-a, b);
        else
            return -jacobi(-a, b);

    T e, a1;
    transform_num(a, e, a1);

    T s;
    if (even(e) || (b & 7) == 1 || (b & 7) == 7)
        s = 1;
    else
        s = -1;
    if ((b & 3) == 3 && (a1 & 3) == 3)
        s = -s;
    if (a1 == 1)
        return s;
    return s * jacobi(b % a1, a1);

#pragma warning (pop)

}
long long int pollard_p_1(long long int n)
{
    // параметры алгоритма, существенно влияют на производительность и качество поиска
    const long long int b = 13;
    const long long int q[] = { 2, 3, 5, 7, 11, 13 };

    // несколько попыток алгоритма
    long long int a = 5 % n;
    for (int j = 0; j<10; j++)
    {

        // ищем такое a, которое взаимно просто с n
        while (gcd(a, n) != 1)
        {
            mulmod(a, a, n);
            a += 3;
            a %= n;
        }

        // вычисляем a^M
        for (size_t i = 0; i < sizeof q / sizeof q[0]; i++)
        {
            long long int qq = q[i];
            long long int e = (long long int)floor(log((double)b) / log((double)qq));
            long long int aa = powmod(a, powmod(qq, e, n), n);
            if (aa == 0)
                continue;

            // проверяем, не найден ли ответ
            long long int g = gcd(aa - 1, n);
            if (1 < g && g < n)
                return g;
        }

    }

    // если ничего не нашли
    return 1;

}
template <class T>
T pollard_rho (T n, unsigned iterations_count = 100000)
{
    T
        b0 = rand() % n,
        b1 = b0,
        g;
    mulmod (b1, b1, n);
    if (++b1 == n)
        b1 = 0;
    g = gcd (abs (b1 - b0), n);
    for (unsigned count=0; count<iterations_count && (g == 1 || g == n); count++)
    {
        mulmod (b0, b0, n);
        if (++b0 == n)
            b0 = 0;
        mulmod (b1, b1, n);
        ++b1;
        mulmod (b1, b1, n);
        if (++b1 == n)
            b1 = 0;
        g = gcd (abs (b1 - b0), n);
    }
    return g;
}
template <class T>
T pollard_bent(T n, unsigned iterations_count = 19)
{
    T
        b0 = rand() % n,
        b1 = (b0*b0 + 2) % n,
        a = b1;
    for (unsigned iteration = 0, series_len = 1; iteration<iterations_count; iteration++, series_len *= 2)
    {
        T g = gcd(b1 - b0, n);
        for (unsigned len = 0; len<series_len && (g == 1 && g == n); len++)
        {
            b1 = (b1*b1 + 2) % n;
            g = gcd(abs(b1 - b0), n);
        }
        b0 = a;
        a = b1;
        if (g != 1 && g != n)
            return g;
    }
    return 1;
}

long long int pollard_monte_carlo(long long int n, unsigned m = 100)
{
    long long int b = rand() % (m - 2) + 2;

    static std::vector<long long int> primes;
    static long long int m_max;
    if (primes.empty())
        primes.push_back(3);
    if (m_max < m)
    {
        m_max = m;
        for (long long int prime = 5; prime <= m; ++++prime)
        {
            bool is_prime = true;
            for (std::vector<long long int>::iterator iter = primes.begin();	iter != primes.end(); ++iter)
            {
                long long int div = *iter;
                if (div*div > prime)
                    break;
                if (prime % div == 0)
                {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
                primes.push_back(prime);
        }
    }

    long long int g = 1;
    for (size_t i = 0; i<primes.size() && g == 1; i++)
    {
        long long int cur = primes[i];
        while (cur <= n)
            cur *= primes[i];
        cur /= primes[i];
        b = powmod(b, cur, n);
        g = gcd(abs(b - 1), n);
        if (g == n)
            g = 1;
    }

    return g;
}
template <class T>
T ferma(const T & n)
{
    long long int
        x = sq_root(n),
        y = 0,
        r = x * x - y * y - n;
    for (;;)
        if (r == 0)
            return x != y ? x - y : x + y;
        else
            if (r > 0)
            {
                r -= y + y + 1;
                ++y;
            }
            else
            {
                r += x + x + 1;
                ++x;
            }
}
long long int primitiveFindDivinder(long long int N, const std::vector<int> & primes)
{
    long long int root = sqrt(N) + 1;
    for (int i = 0; primes[i] <= root; ++i)
    {
        if (N % primes[i] == 0)
            return primes[i];
    }
    return N;
}
std::map<long long int, int> primitiveFactorization(long long int N, const std::vector<int> & primes)
{
    std::map<long long int, int> multiples;
    while (N != 1)
    {
        long long int factor = primitiveFindDivinder(N, primes);
        auto it = multiples.find(factor);
        if (it == multiples.end())
            multiples.insert(std::pair<long long int, int>(factor, 1));
        else
            ++it->second;
        N /= factor;
    }

    //std::cout << std::endl;
    return multiples;
}
std::map<long long int, int> factorize(long long int N)
{
    std::map<long long int, int> multiples;
    std::vector<int> & primes = MathSets::getPrimesVector();
    auto merge = [&multiples](const std::map<long long int, int> & sec) mutable {
        for (auto &it : sec)
        {
            multiples[it.first] += it.second;
        }
    };
    if (N < 2)
        return multiples;
    while (N != 1 && !prime(N))
    {
        if (N <= ((long long int)(primes.back())) * primes.back())
        {
            merge(primitiveFactorization(N, primes));
            return multiples;
        }
        long long int div = 1;
        for (int i = 0; i < 1000; ++i)
            if (N % primes[i] == 0)
                div = primes[i];
        // сначала идут быстрые алгоритмы Полларда
        if (div == 1)
            div = pollard_monte_carlo(N);
        if (div == 1)
            div = pollard_rho(N);
        if (div == 1)
            div = pollard_p_1(N);
        if (div == 1)
            div = pollard_bent(N);
        // придётся запускать 100%-ый алгоритм Ферма
        if (div == 1)
            div = ferma(N);
        N /= div;
        merge(factorize(div));
    }
    if (N != 1)
        ++multiples[N];
    return multiples;
}
std::vector<long long int> v_factorize(long long int N, bool have_to_sort)
{
    std::vector<long long int> multiples;
    std::vector<int> & primes = MathSets::getPrimesVector();
    if (N < 2)
        return multiples;
    bool is_useless_to_find_primes_primitive = false;
    while (N != 1 && !prime(N))
    {
        long long int div = 1;
        if (!is_useless_to_find_primes_primitive)
            for (auto &it : primes)
                if (N % it == 0)
                    div = it;
        if (div == 1)
            is_useless_to_find_primes_primitive = true;
        else
        {
            multiples.push_back(div);
            N /= div;
            continue;
        }
    div = pollard_monte_carlo(N);
    if (div == 1)
        div = pollard_rho(N);
    if (div == 1)
        div = pollard_p_1(N);
    if (div == 1)
        div = pollard_bent(N);
    // придётся запускать 100%-ый алгоритм Ферма
    if (div == 1)
        div = ferma(N);
    N /= div;
    auto div_factors = v_factorize(div, false);
    multiples.insert(multiples.end(), div_factors.begin(), div_factors.end());
    }
    if (N != 1)
        multiples.push_back(N);
    if (have_to_sort)
        std::sort(multiples.begin(), multiples.end());
    return multiples;
}
