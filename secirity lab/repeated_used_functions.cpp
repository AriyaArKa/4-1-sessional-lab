#include <bits/stdc++.h>
using namespace std;

using int64 = long long;


int64 gcd(int64 a, int64 b)
{
    while (b != 0)
    {
        int64 t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int64 mul_mod(int64 a, int64 b, int64 mod)
{
    return (int64)((__int128)a * b % mod);
}

int64 mod_pow(int64 base, int64 exp, int64 mod)
{
    int64 result = 1;
    base %= mod;

    while (exp > 0)
    {
        if (exp & 1)
            result = mul_mod(result, base, mod);

        base = mul_mod(base, base, mod);
        exp >>= 1;
    }

    return result;
}

int64 egcd(int64 a, int64 b, int64 &x, int64 &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }

    int64 x1, y1;
    int64 g = egcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return g;
}

// Modular Inverse
int64 modInverse(int64 e, int64 phi)
{
    int64 x, y;
    int64 g = egcd(e, phi, x, y);

    if (g != 1)
        return -1;

    x %= phi;
    if (x < 0)
        x += phi;

    return x;
}
