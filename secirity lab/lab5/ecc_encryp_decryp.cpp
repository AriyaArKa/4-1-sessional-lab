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


// P - Q = P + (-Q)
pair<long long,long long> subtract(
    pair<long long,long long> P,
    pair<long long,long long> Q,
    long long a,
    long long p)
{

    // -Q = (x,-y mod p)

    Q.second =
        mod(-Q.second,p);


    return add(P,Q,a,p);
}


// Print Point
void print(pair<long long, long long> P) {

    if (P.first == -1)
        cout << "O";

    else
        cout << "(" << P.first << ", " << P.second << ")";
}


int main() {
    // y^2 = x^3 + 2x + 3 mod 17

    long long a = 2;
    long long b = 3;
    long long p = 17;

    // Valid Base Point
    pair<long long,long long> G ={3,6};


    // Private key
    long long d = 7;

    // Public key
    //
    // Q = dG

    pair<long long,long long> Q = multiply(G,d,a,p);

    cout << "Base Point G = "; print(G); cout << endl;
    cout << "Private Key d = " << d << endl;
    cout << "Public Key Q = dG = "; print(Q); cout << endl;

    
    // M = 5G

    pair<long long,long long> M = multiply(G,5,a,p);


    cout << "Original Message M = 5G = "; print(M); cout << endl;

    //encryption

    // Random number k
    long long k = 3;


    // C1 = kG
    pair<long long,long long> C1 =multiply(G,k,a,p);

    // kQ
    pair<long long,long long> kQ =multiply(Q,k,a,p);


    // C2 = M + kQ
    pair<long long,long long> C2 =add(M,kQ,a,p);



    cout << "===== ENCRYPTION =====" << endl;
    cout << "Random k = " << k << endl;
    cout << "C1 = kG = "; print(C1); cout << endl;
    cout << "kQ = "; print(kQ); cout << endl;
    cout << "C2 = M + kQ = "; print(C2); cout << endl;
    cout << "Ciphertext c1 = "; print(C1); cout << endl;
    cout << "Ciphertext c2 = "; print(C2); cout << endl;

    //decryption

    // S = dC1
    pair<long long,long long> S =multiply(C1,d,a,p);

    // M = C2 - S
    // M = C2 - dC1
    pair<long long,long long> decryptedM = subtract(C2,S,a,p);

    cout << "dC1 = "; print(S); cout << endl;
    cout << "M = C2 - dC1 = "; print(decryptedM); cout << endl;


    return 0;
}