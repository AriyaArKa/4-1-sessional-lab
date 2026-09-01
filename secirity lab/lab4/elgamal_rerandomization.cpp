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
        int64 t = b;
        b = a % b;
        a = t;
    }

    return a;
}

// ----------------------------------------------------
// (a * b) mod mod
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
    int64 a = 123;

    // beta = alpha^a mod p
    int64 beta = mod_pow(alpha, a, p);

    cout << "Public Key = ("
         << p << ", "
         << alpha << ", "
         << beta << ")\n";

    cout << "Private Key = " << a << "\n\n";


    // =================================================
    // 2. ORIGINAL MESSAGE
    // =================================================

    int64 m = 20;

    // Random value used for original encryption
    int64 r = 7;


    // =================================================
    // 3. NORMAL ELGAMAL ENCRYPTION
    // =================================================

    // C1 = alpha^r mod p
    int64 C1 = mod_pow(alpha, r, p);


    // beta^r mod p
    int64 beta_r = mod_pow(beta, r, p);


    // C2 = m * beta^r mod p
    int64 C2 = mul_mod(m, beta_r, p);


    cout << "Original Message = " << m << "\n";

    cout << "Original Cipher (C1, C2) = ("
         << C1 << ", "
         << C2 << ")\n\n";


    // =================================================
    // 4. RE-RANDOMIZATION
    // =================================================

    // New random value
    int64 r2 = 9;


    // alpha^r2 mod p
    int64 alpha_r2 = mod_pow(alpha, r2, p);


    // beta^r2 mod p
    int64 beta_r2 = mod_pow(beta, r2, p);


    // C1' = C1 * alpha^r2 mod p
    int64 C1_new = mul_mod(C1, alpha_r2, p);


    // C2' = C2 * beta^r2 mod p
    int64 C2_new = mul_mod(C2, beta_r2, p);


    cout << "Re-randomization random r2 = "
         << r2 << "\n";

    cout << "Re-randomized Cipher (C1', C2') = ("
         << C1_new << ", "
         << C2_new << ")\n\n";


    // =================================================
    // 5. DECRYPT RE-RANDOMIZED CIPHERTEXT
    // =================================================

    // s = (C1')^a mod p
    int64 s = mod_pow(C1_new, a, p);


    // s^-1 mod p
    int64 s_inv = modInverse(s, p);


    // m = C2' * s^-1 mod p
    int64 decrypted = mul_mod(C2_new, s_inv, p);


    cout << "Decrypted Message = "
         << decrypted << "\n";

    cout << "Expected Message  = "
         << m << "\n";


    return 0;
}
