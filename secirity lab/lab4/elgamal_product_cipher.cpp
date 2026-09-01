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

    cout << "Private Key = "
         << a << "\n\n";


    // =================================================
    // 2. TWO MESSAGES
    // =================================================

    int64 m1 = 5;
    int64 m2 = 9;

    // Separate random values
    int64 r1 = 7;
    int64 r2 = 11;


    // =================================================
    // 3. ENCRYPT FIRST MESSAGE
    // =================================================

    // C11 = alpha^r1 mod p
    int64 C11 = mod_pow(alpha, r1, p);

    // beta^r1 mod p
    int64 beta_r1 = mod_pow(beta, r1, p);

    // C12 = m1 * beta^r1 mod p
    int64 C12 = mul_mod(m1, beta_r1, p);

    cout << "Cipher of m1 = ("
         << C11 << ", "
         << C12 << ")\n";


    // =================================================
    // 4. ENCRYPT SECOND MESSAGE
    // =================================================

    // C21 = alpha^r2 mod p
    int64 C21 = mod_pow(alpha, r2, p);

    // beta^r2 mod p
    int64 beta_r2 = mod_pow(beta, r2, p);

    // C22 = m2 * beta^r2 mod p
    int64 C22 = mul_mod(m2, beta_r2, p);

    cout << "Cipher of m2 = ("
         << C21 << ", "
         << C22 << ")\n\n";


    // =================================================
    // 5. COMBINE / MULTIPLY CIPHERTEXTS
    // =================================================

    // C1' = C11 * C21 mod p
    int64 C1_combined = mul_mod(C11, C21, p);

    // C2' = C12 * C22 mod p
    int64 C2_combined = mul_mod(C12, C22, p);

    cout << "Combined Cipher = ("
         << C1_combined << ", "
         << C2_combined << ")\n\n";


    // =================================================
    // 6. DECRYPT COMBINED CIPHERTEXT
    // =================================================

    // s = (C1')^a mod p
    int64 s = mod_pow(C1_combined, a, p);

    // s^-1 mod p
    int64 s_inv = modInverse(s, p);

    // m' = C2' * s^-1 mod p
    int64 decrypted = mul_mod(C2_combined, s_inv, p);


    // =================================================
    // 7. EXPECTED RESULT
    // =================================================

    int64 expected = mul_mod(m1, m2, p);


    cout << "Decrypted Product = "
         << decrypted << "\n";

    cout << "Expected m1 * m2  = "
         << expected << "\n";


    return 0;
}
