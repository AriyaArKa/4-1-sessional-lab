#include <bits/stdc++.h>

using namespace std;

// x mod p
long long mod(long long x, long long p) {
  x %= p;

  if (x < 0)
    x += p;

  return x;
}

// Modular Inverse
long long inverse(long long a, long long p) {
  for (long long i = 1; i < p; i++) {
    if ((a * i) % p == 1)
      return i;
  }

  return -1;
}


// Point Addition
pair < long long, long long > add(
  pair < long long, long long > P,
  pair < long long, long long > Q,
  long long a,
  long long p) {

  // Infinity point
  if (P.first == -1)
    return Q;
  if (Q.first == -1)
    return P;


  long long x1 = P.first;
  long long y1 = P.second;

  long long x2 = Q.first;
  long long y2 = Q.second;


  // P + (-P)=O
  if (x1 == x2 && mod(y1 + y2, p) == 0)
    return {-1,-1};

  long long s;

  // Point Doubling
  if (P == Q) {

    long long num =mod(3 * x1 * x1 + a, p);
    long long den =mod(2 * y1, p);
    s =mod(num * inverse(den, p), p);
  }

  // Normal Addition
  else {

    long long num =mod(y2 - y1, p);
    long long den =mod(x2 - x1, p);
    s =mod(num * inverse(den, p), p);
  }

  long long x3 =mod(s * s - x1 - x2, p);
  long long y3 =mod(s * (x1 - x3) - y1, p);
  return {x3,y3};
}

// Scalar Multiplication kG

pair < long long, long long > multiply(pair < long long, long long > G,long long k,long long a,long long p) 
{
  pair < long long, long long > result = {-1, -1};
  while (k > 0) {
    if (k % 2 == 1) {
      result =add(result, G, a, p);
    }
    G =add(G, G, a, p);
    k = k / 2;

  }

  return result;
}

// P-Q = P+(-Q)
pair < long long, long long > subtract(
  pair < long long, long long > P,
  pair < long long, long long > Q,
  long long a,
  long long p) {

  Q.second =mod(-Q.second, p);

  return add(P, Q, a, p);
}

// Print Point
void print(pair < long long, long long > P) {

  if (P.first == -1)
    cout << "O";
  else
    cout << "(" << P.first << "," << P.second << ")";

}

// Homomorphic Addition
void homomorphic(
  pair < long long, long long > C1_1,
  pair < long long, long long > C2_1,

  pair < long long, long long > C1_2,
  pair < long long, long long > C2_2,

  pair < long long, long long > M1,
  pair < long long, long long > M2,

  long long d,
  long long a,
  long long p)
{


  // New C1

  pair < long long, long long > newC1 =
      add(C1_1,C1_2,a,p);



  // New C2

  pair < long long, long long > newC2 =
      add(C2_1,C2_2,a,p);



  cout << "===== HOMOMORPHIC ADDITION =====" << endl;


  cout << "New C1 = ";
  print(newC1);
  cout << endl;


  cout << "New C2 = ";
  print(newC2);
  cout << endl;



  // Decryption
  pair < long long, long long > secret =multiply(newC1,d,a,p);

  pair < long long, long long > message =subtract(newC2,secret,a,p);

  cout << "Recovered M1+M2 = ";
  print(message);
  cout << endl;


  pair<long long,long long> expected =add(M1,M2,a,p);

  cout << "Expected M1+M2 = ";
  print(expected);
  cout << endl;

  if(message.first == expected.first && message.second == expected.second)
  {
      cout << "MATCHED : Homomorphic Property Verified"<< endl;
  }

  else
  {
      cout << "NOT MATCHED : Error"<< endl;
  }

}

int main() {

  // Curve:
  // y^2 = x^3 + 2x + 3 mod 17

  long long a = 2;
  long long p = 17;

  // Generator Point

  pair < long long, long long > G = {3,6};

  // Private Key

  long long d = 7;

  // Public Key

  // Q=dG

  pair < long long, long long > Q =multiply(G, d, a, p);

  cout << "Base Point G = "; print(G); cout << endl;
  cout << "Private Key d = " << d << endl;
  cout << "Public Key Q = "; print(Q); cout << endl;

  // ==========================
  // MESSAGE 1
  // ==========================

  pair < long long, long long > M1 =multiply(G, 5, a, p);
  cout << "M1 = "; print(M1); cout << endl;

  // ==========================
  // MESSAGE 2
  // ==========================

  pair < long long, long long > M2 =multiply(G, 9, a, p);
  cout << "M2 = "; print(M2); cout << endl;

  // ==========================
  // ENCRYPT M1
  // ==========================

  long long k1 = 3;

  pair < long long, long long > C1_1 =multiply(G, k1, a, p);
  pair < long long, long long > C2_1 =add(M1,multiply(Q, k1, a, p),a,p);

  cout << "Encryption M1" << endl;
  cout << "C1 = "; print(C1_1); cout << endl;
  cout << "C2 = "; print(C2_1); cout << endl;

  // ==========================
  // ENCRYPT M2
  // ==========================

  long long k2 = 5;

  pair < long long, long long > C1_2 =multiply(G, k2, a, p);
  pair < long long, long long > C2_2 =add(M2,multiply(Q, k2, a, p),a,p);

  cout << "Encryption M2" << endl;
  cout << "C1 = "; print(C1_2); cout << endl;
  cout << "C2 = "; print(C2_2); cout << endl;

  // ==========================
  // HOMOMORPHISM
  // ==========================
homomorphic(
    C1_1,
    C2_1,

    C1_2,
    C2_2,

    M1,
    M2,

    d,
    a,
    p
);
  return 0;
}