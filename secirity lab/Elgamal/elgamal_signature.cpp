#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

// ----------------------------------------------------
// GCD
// ----------------------------------------------------
int64 gcd(int64 a, int64 b)
{
    while (b != 0)
    {
        int64 temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// ----------------------------------------------------
// (a * b) mod m
// ----------------------------------------------------
int64 mul_mod(int64 a, int64 b, int64 mod)
{
    return (a * b) % mod;
}

// ----------------------------------------------------
// base^exp mod mod
//
// Memory:
// mod_pow(BASE, EXPONENT, MOD)
// ----------------------------------------------------
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

// ----------------------------------------------------
// Extended Euclidean Algorithm
// ----------------------------------------------------
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

// ----------------------------------------------------
// value^-1 mod mod
//
// Memory:
// modInverse(VALUE, MOD)
// ----------------------------------------------------
int64 modInverse(int64 value, int64 mod)
{
    int64 x, y;

    int64 g = egcd(value, mod, x, y);

    if (g != 1)
        return -1;

    x %= mod;

    if (x < 0)
        x += mod;

    return x;
}

int main()
{
    // =================================================
    // 1. KEY GENERATION
    // =================================================

    int64 p = 467;
    int64 alpha = 2;

    // Private key
    int64 a = 127;

    // beta = alpha^a mod p
    int64 beta = mod_pow(alpha, a, p);

    cout << "Public Key  = ("
         << p << ", "
         << alpha << ", "
         << beta << ")\n";

    cout << "Private Key = " << a << "\n\n";


    // =================================================
    // 2. MESSAGE
    // =================================================

    int64 M = 17;

    cout << "message as integer: "
         << M << "\n";


    // =================================================
    // 3. SIGNATURE GENERATION
    // =================================================

    // Temporary random number
    int64 r = 5;

    // For signature:
    // gcd(r, p-1) must be 1
    // because r inverse mod (p-1) is required.

    if (gcd(r, p - 1) != 1)
    {
        cout << "Invalid r. Choose another r.\n";
        return 0;
    }


    // y1 = alpha^r mod p
    int64 y1 = mod_pow(alpha, r, p);


    // r^-1 mod (p-1)
    int64 r_inv = modInverse(r, p - 1);


    // y2 = r^-1 (M - a*y1) mod (p-1)

    int64 temp = (M - mul_mod(a, y1, p - 1)) % (p - 1);

    if (temp < 0)
        temp += (p - 1);

    int64 y2 = mul_mod(r_inv, temp, p - 1);


    cout << "\nSignature (y1, y2) = ("
         << y1 << ", "
         << y2 << ")\n";


    // =================================================
    // 4. SIGNATURE VERIFICATION
    // =================================================

    // Left = alpha^M mod p
    int64 left = mod_pow(alpha, M, p);


    // Right = beta^y1 * y1^y2 mod p

    int64 part1 = mod_pow(beta, y1, p);

    int64 part2 = mod_pow(y1, y2, p);

    int64 right = mul_mod(part1, part2, p);


    cout << "\nLeft  = " << left << "\n";
    cout << "Right = " << right << "\n";


    if (left == right)
        cout << "\nSignature VALID\n";
    else
        cout << "\nSignature INVALID\n";


    return 0;
}
