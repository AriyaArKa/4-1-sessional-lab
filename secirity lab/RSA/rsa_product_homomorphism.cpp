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

int64 autoSelectE(int64 phi)
{
    int64 e = 2;

    while (gcd(e, phi) != 1)
        e++;

    return e;
}

int main()
{
    int64 p = 10007;
    int64 q = 10009;

    cout << "Prime p = " << p << endl;
    cout << "Prime q = " << q << endl;

    
    int64 n = p * q;
    int64 phi = (p - 1) * (q - 1);

    cout << "\nn = " << n << endl;
    cout << "phi = " << phi << endl;

    // Public exponent
    int64 e = autoSelectE(phi);
    cout << "Automatically selected e = " << e << endl;

    int64 d = modInverse(e, phi);

    cout << "\nPublic Key (n, e): (" << n << ", " << e << ")" << endl;
    cout << "Private Key (d): " << d << endl;

    // Messages
    int64 m1 = 65;
    int64 m2 = 77;

    // Encryption
    int64 c1 = mod_pow(m1, e, n);
    int64 c2 = mod_pow(m2, e, n);

    cout << "\nMessage 1 = " << m1 << endl;
    cout << "Message 2 = " << m2 << endl;

    cout << "Encrypted m1 = " << c1 << endl;
    cout << "Encrypted m2 = " << c2 << endl;

    int64 cprod = mul_mod(c1, c2, n);

    cout << "\nCombined Ciphertext = " << cprod << endl;

    // Decrypt the product
    int64 decrypted = mod_pow(cprod, d, n);

    cout << "Decrypted Product = " << decrypted << endl;
    cout << "Expected Value    = " << (m1 * m2) % n << endl;

    if (decrypted == (m1 * m2) % n)
        cout << "\nEqual" << endl;
    else
        cout << "\nnot equal" << endl;

    return 0;
}