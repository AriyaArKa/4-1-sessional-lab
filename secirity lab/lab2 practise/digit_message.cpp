#include <iostream>
using namespace std;

using int64 = long long;


// GCD using Euclidean Algorithm
int64 gcd(int64 a, int64 b)
{
    while(b != 0)
    {
        int64 t = b;
        b = a % b;
        a = t;
    }

    return a;
}


// Extended Euclidean Algorithm
// Finds x, y such that: ax + by = gcd(a,b)
int64 egcd(int64 a, int64 b, int64 &x, int64 &y)
{
    if(b == 0)
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


// Modular inverse: finds d where (e*d) mod phi = 1
int64 modinv(int64 e, int64 phi)
{
    int64 x, y;
    int64 g = egcd(e, phi, x, y);

    if(g != 1)
        return -1;

    x %= phi;

    if(x < 0)
        x += phi;

    return x;
}


// Overflow-safe modular multiplication: (a*b) % mod
// via add-and-double, so a*b never has to be formed directly.
// (This toolchain has no __int128 to fall back on.)
int64 mulmod(int64 a, int64 b, int64 mod)
{
    int64 result = 0;

    a %= mod;

    while(b > 0)
    {
        if(b % 2 == 1)
            result = (result + a) % mod;

        a = (a + a) % mod;
        b /= 2;
    }

    return result;
}


// Fast modular exponentiation: (base^exp) % mod
int64 mod_pow(int64 base, int64 exp, int64 mod)
{
    int64 result = 1;
    base %= mod;

    while(exp > 0)
    {
        if(exp % 2 == 1)
            result = mulmod(result, base, mod);

        base = mulmod(base, base, mod);
        exp /= 2;
    }

    return result;
}


int main()
{
    int64 p, q;

    cout<<"Enter prime p and q: ";
    cin>>p>>q;

    int64 n = p * q;
    int64 phi = (p - 1) * (q - 1);

    cout<<"n = "<<n<<", phi = "<<phi<<"\n";

    int64 e;

    cout<<"Enter public exponent e (0 for auto): ";
    cin>>e;

    if(e == 0)
    {
        e = 2;

        while(gcd(e, phi) != 1)
            e++;

        cout<<"Auto-selected e = "<<e<<"\n";
    }
    else if(gcd(e, phi) != 1)
    {
        cout<<"e and phi(n) not coprime.\n";
        return 0;
    }

    int64 d = modinv(e, phi);

    if(d == -1)
    {
        cout<<"No modular inverse for e mod phi.\n";
        return 0;
    }

    cout<<"Private exponent d = "<<d<<"\n";

    // First 3 possible d values.
    // All solutions of e*d = 1 (mod phi) are d0 + t*phi.
    cout<<"Candidate d values: ";

    int64 d_candidate = d;

    for(int t = 0; t < 3; t++)
    {
        cout<<d_candidate<<" ";
        d_candidate += phi;
    }

    cout<<"\n";

    int64 m;

    cout<<"Enter plaintext m: ";
    cin>>m;

    int64 c = mod_pow(m, e, n);
    cout<<"Ciphertext c = "<<c<<"\n";

    cout<<"Decrypted: "<<mod_pow(c, d, n)<<"\n";
}
