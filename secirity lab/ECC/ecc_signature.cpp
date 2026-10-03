#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

// ====================================================
// MOD
// ====================================================

int64 mod(int64 x, int64 p)
{
    x %= p;

    if (x < 0)
        x += p;

    return x;
}

// ====================================================
// MODULAR INVERSE
// ====================================================

int64 inverse(int64 a, int64 p)
{
    for (int64 i = 1; i < p; i++)
    {
        if ((a * i) % p == 1)
            return i;
    }

    return -1;
}

// ====================================================
// POINT ADDITION
// ====================================================

pair<int64, int64> add(
    pair<int64, int64> P,
    pair<int64, int64> Q,
    int64 a,
    int64 p)
{
    // P + O = P
    if (P.first == -1)
        return Q;

    // O + Q = Q
    if (Q.first == -1)
        return P;

    int64 x1 = P.first;
    int64 y1 = P.second;

    int64 x2 = Q.first;
    int64 y2 = Q.second;

    // P + (-P) = O
    if (x1 == x2 && mod(y1 + y2, p) == 0)
        return {-1, -1};

    int64 slope;

    // Point doubling
    if (P == Q)
    {
        int64 numerator =
            mod(3 * x1 * x1 + a, p);

        int64 denominator =
            mod(2 * y1, p);

        slope =
            mod(numerator * inverse(denominator, p), p);
    }

    // Point addition
    else
    {
        int64 numerator =
            mod(y2 - y1, p);

        int64 denominator =
            mod(x2 - x1, p);

        slope =
            mod(numerator * inverse(denominator, p), p);
    }

    int64 x3 =
        mod(slope * slope - x1 - x2, p);

    int64 y3 =
        mod(slope * (x1 - x3) - y1, p);

    return {x3, y3};
}

// ====================================================
// SCALAR MULTIPLICATION
// ====================================================

pair<int64, int64> multiply(
    pair<int64, int64> G,
    int64 k,
    int64 a,
    int64 p)
{
    pair<int64, int64> result = {-1, -1};

    while (k > 0)
    {
        if (k % 2 == 1)
            result = add(result, G, a, p);

        G = add(G, G, a, p);

        k /= 2;
    }

    return result;
}

// ====================================================
// PRINT POINT
// ====================================================

void print(pair<int64, int64> P)
{
    if (P.first == -1)
        cout << "O";
    else
        cout << "(" << P.first << ", "
             << P.second << ")";
}

// ====================================================
// ECDSA
// ====================================================

int main()
{
    // =================================================
    // ECC DOMAIN PARAMETERS
    // y² = x³ + ax + b mod p
    // =================================================

    int64 a = 2;
    int64 b = 3;
    int64 p = 17;

    // Base point
    pair<int64, int64> G = {3, 6};

    // Order of G
    int64 n = 11;

    // =================================================
    // KEY GENERATION
    // =================================================

    // Private key
    int64 d = 7;

    // Public key
    // Q = dG
    pair<int64, int64> Q =
        multiply(G, d, a, p);

    cout << "===== KEY GENERATION =====\n";

    cout << "Base Point G = ";
    print(G);
    cout << endl;

    cout << "Order n = " << n << endl;

    cout << "Private Key d = "
         << d << endl;

    cout << "Public Key Q = dG = ";
    print(Q);
    cout << "\n\n";


    // =================================================
    // MESSAGE
    // =================================================

    // In real systems:
    // h = Hash(message)

    // For lab:
    int64 h = 5;

    cout << "Message Hash h = "
         << h << "\n\n";


    // =================================================
    // SIGNATURE GENERATION
    // =================================================

    // Random nonce
    int64 k = 3;

    // R = kG
    pair<int64, int64> R =
        multiply(G, k, a, p);

    // r = x-coordinate of R mod n
    int64 r = mod(R.first, n);

    // k^-1 mod n
    int64 k_inv =
        inverse(k, n);

    // s = k^-1(h + dr) mod n
    int64 s =
        mod(k_inv * (h + d * r), n);


    cout << "===== SIGNATURE =====\n";

    cout << "Random k = "
         << k << endl;

    cout << "R = kG = ";
    print(R);
    cout << endl;

    cout << "r = " << r << endl;

    cout << "k^-1 mod n = "
         << k_inv << endl;

    cout << "s = " << s << endl;

    cout << "Signature = ("
         << r << ", "
         << s << ")\n\n";


    // =================================================
    // SIGNATURE VERIFICATION
    // =================================================

    // w = s^-1 mod n
    int64 w =
        inverse(s, n);

    // u1 = h*w mod n
    int64 u1 =
        mod(h * w, n);

    // u2 = r*w mod n
    int64 u2 =
        mod(r * w, n);

    // u1G
    pair<int64, int64> u1G =
        multiply(G, u1, a, p);

    // u2Q
    pair<int64, int64> u2Q =
        multiply(Q, u2, a, p);

    // X = u1G + u2Q
    pair<int64, int64> X =
        add(u1G, u2Q, a, p);

    // v = x-coordinate of X mod n
    int64 v =
        mod(X.first, n);


    cout << "===== VERIFICATION =====\n";

    cout << "w = s^-1 mod n = "
         << w << endl;

    cout << "u1 = hw mod n = "
         << u1 << endl;

    cout << "u2 = rw mod n = "
         << u2 << endl;

    cout << "u1G = ";
    print(u1G);
    cout << endl;

    cout << "u2Q = ";
    print(u2Q);
    cout << endl;

    cout << "X = u1G + u2Q = ";
    print(X);
    cout << endl;

    cout << "v = xX mod n = "
         << v << endl;

    cout << "r = "
         << r << endl;


    // =================================================
    // FINAL RESULT
    // =================================================

    if (v == r)
        cout << "\nSignature VALID\n";
    else
        cout << "\nSignature INVALID\n";

    return 0;
}