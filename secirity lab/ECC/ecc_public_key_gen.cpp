#include <bits/stdc++.h>
using namespace std;

// x mod p
//-5mod17  mod(-5,17)
long long mod(long long x, long long p) {
    x %= p;
    if (x < 0)  //check -5
    {
        x += p;
    }
    return x;
}

// Modular Inverse
// 2^-1 mod 17
// 2 * i = 1(mod p)
// 2k mod17 = 1
long long inverse(long long a, long long p) {
    for (long long i = 1; i < p; i++) {
        if ((a * i) % p == 1)
        {
            return i;
        }
    }
    return -1;
}

// Point Addition
pair<long long, long long> add(
    pair<long long, long long> P,
    pair<long long, long long> Q,
    long long a, long long p) {

    // Point at Infinity
    if (P.first == -1) return Q;
    if (Q.first == -1) return P;

    long long x1 = P.first, y1 = P.second;
    long long x2 = Q.first, y2 = Q.second;

    // P + (-P) = O
    if (x1 == x2 && mod(y1 + y2, p) == 0)
        return {-1, -1};

    long long s;

    // Point Doubling: P + P = 2P
    if (x1 == x2 && y1 == y2) {

        // s = (3x1^2 + a) / 2y1
        long long num = mod(3 * x1 * x1 + a, p);
        long long den = mod(2 * y1, p);

        s = mod(num * inverse(den, p), p);
    }

    // Point Addition: P + Q
    else {

        // s = (y2-y1) / (x2-x1)
        long long num = mod(y2 - y1, p);
        long long den = mod(x2 - x1, p);

        s = mod(num * inverse(den, p), p);
    }

    // x3 = s^2 - x1 - x2
    long long x3 = mod(s * s - x1 - x2, p);

    // y3 = s(x1-x3) - y1
    long long y3 = mod(s * (x1 - x3) - y1, p);

    return {x3, y3};
}


// Scalar Multiplication: kG
pair<long long, long long> multiply(
    pair<long long, long long> G,
    long long k,
    long long a,
    long long p) {

    pair<long long, long long> result = {-1, -1};

    while (k > 0) { 

        if (k % 2 == 1)
            result = add(result, G, a, p);

        G = add(G, G, a, p);

        k = k / 2;
    }

    return result;
}


// Print Point
void print(pair<long long, long long> P) {

    if (P.first == -1)
        cout << "O";

    else
        cout << "(" << P.first << ", " << P.second << ")";
}


int main() {

    // Elliptic Curve:
    // y^2 = x^3 + ax + b (mod p)

    long long a = 2;
    long long b = 3;
    long long p = 17;

    // Base Point
    pair<long long, long long> G = {3, 1};

    // Private Key
    long long d = 7;


    cout << "Curve: y^2 = x^3 + 2x + 3 (mod 17)\n";

    cout << "Base Point G = ";
    print(G);
    cout << "\n\n";


    // Generate 2G to 7G
    for (int i = 2; i <= 7; i++) {

        cout << i << "G = ";
        print(multiply(G, i, a, p));
        cout << endl;
    }


    // Public Key
    pair<long long, long long> Q =
        multiply(G, d, a, p);

    cout << "\nPrivate Key d = " << d << endl;

    cout << "Public Key Q = dG = ";
    print(Q);
    cout << endl;


    return 0;
}