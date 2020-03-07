#include "primetest.h"
#include "some_algebra_expression_conversions.h"
#include "mathsets.h"
#include <cmath>
//код здесь скопирован с http://www.e-maxx-ru.1gb.ru/algo/bpsw
long long int powmod(long long int a, long long int b, long long int m) {
    long long int res = 1;
    while (b)
        if (b & 1)
            res = (res * 1ll * a) % m, --b;
        else
            a = (a * 1ll * a) % m, b >>= 1;
    return res;
}

bool miller_rabin(long long int n) {
    long long int b = 2;
    for (long long int g; (g = gcd(n, b)) != 1; ++b)
        if (n > g)
            return false;
    long long int p = 0, q = n - 1;
    while ((q & 1) == 0)
        ++p, q >>= 1;
    long long int rem = powmod(b, q, n);
    if (rem == 1 || rem == n - 1)
        return true;
    for (long long int i = 1; i<p; ++i) {
        rem = (rem * 1ll * rem) % n;
        if (rem == n - 1)  return true;
    }
    return false;
}

long long int jacobi(long long int a, long long int b)
{
    if (a == 0)  return 0;
    if (a == 1)  return 1;
    if (a < 0)
        if ((b & 2) == 0)
            return jacobi(-a, b);
        else
            return -jacobi(-a, b);
    long long int a1 = a, e = 0;
    while ((a1 & 1) == 0)
        a1 >>= 1, ++e;
    long long int s;
    if ((e & 1) == 0 || (b & 7) == 1 || (b & 7) == 7)
        s = 1;
    else
        s = -1;
    if ((b & 3) == 3 && (a1 & 3) == 3)
        s = -s;
    if (a1 == 1)
        return s;
    return s * jacobi(b % a1, a1);
}

bool bpsw(long long int n) {
    if ((long long int)sqrt(n + 0.0) * (long long int)sqrt(n + 0.0) == n)  return false;
    long long int dd = 5;
    for (;;) {
        long long int g = gcd(n, abs(dd));
        if (1<g && g<n)  return false;
        if (jacobi(dd, n) == -1)  break;
        dd = dd<0 ? -dd + 2 : -dd - 2;
    }
    long long int p = 1, q = (p*p - dd) / 4;
    long long int d = n + 1, s = 0;
    while ((d & 1) == 0)
        ++s, d >>= 1;
    long long u = 1, v = p, u2m = 1, v2m = p, qm = q, qm2 = q * 2, qkd = q;
    for (long long int mask = 2; mask <= d; mask <<= 1) {
        u2m = (u2m * v2m) % n;
        v2m = (v2m * v2m) % n;
        while (v2m < qm2)   v2m += n;
        v2m -= qm2;
        qm = (qm * qm) % n;
        qm2 = qm * 2;
        if (d & mask) {
            long long t1 = (u2m * v) % n, t2 = (v2m * u) % n,
                t3 = (v2m * v) % n, t4 = (((u2m * u) % n) * dd) % n;
            u = t1 + t2;
            if (u & 1)  u += n;
            u = (u >> 1) % n;
            v = t3 + t4;
            if (v & 1)  v += n;
            v = (v >> 1) % n;
            qkd = (qkd * qm) % n;
        }
    }
    if (u == 0 || v == 0)  return true;
    long long qkd2 = qkd * 2;
    for (long long int r = 1; r<s; ++r) {
        v = (v * v) % n - qkd2;
        if (v < 0)  v += n;
        if (v < 0)  v += n;
        if (v >= n)  v -= n;
        if (v >= n)  v -= n;
        if (v == 0)  return true;
        if (r < s - 1) {
            qkd = (qkd * 1ll * qkd) % n;
            qkd2 = qkd * 2;
        }
    }
    return false;
}

bool prime(long long int n) { // эту функцию нужно вызывать для проверки на простоту
    std::vector<int> & primes = MathSets::getPrimesVector();
    if (n <= primes.back() && n > primes[100])
    {
        std::set<int> & s_primes  = MathSets::getPrimesSet();
        return s_primes.find(n) != s_primes.end();
    }
    for (auto it : primes)
            if (n == it)
                return true;
            else if (n % it == 0)
                return false;
    if (primes.back() * primes.back() >= n)
        return true;
    if (!miller_rabin(n))
        return false;
    return bpsw(n);
}
