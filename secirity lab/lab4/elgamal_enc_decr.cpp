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
    return (a * b) % mod;
}

// Returns base^exp mod mod
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

// Returns value^-1 mod mod
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
    // Key generation
    int64 p = 467;
    int64 alpha = 2;
    int64 a = 123;       // private key

    int64 beta = mod_pow(alpha, a, p);

    cout << "Public Key = (" 
         << p << ", "
         << alpha << ", "
         << beta << ")\n";

    cout << "Private Key = " << a << "\n";

    // Message
    int64 m = 20;

    // Temporary random number
    int64 r = 7;

    // Encryption
    int64 C1 = mod_pow(alpha, r, p);

    int64 beta_r = mod_pow(beta, r, p);
    int64 C2 = mul_mod(m, beta_r, p);

    cout << "\nCiphertext = ("
         << C1 << ", "
         << C2 << ")\n";

    // Decryption
    int64 s = mod_pow(C1, a, p);

    int64 s_inv = modInverse(s, p);

    int64 decrypted = mul_mod(C2, s_inv, p);

    cout << "Decrypted Message = "
         << decrypted << "\n";

    return 0;
}


