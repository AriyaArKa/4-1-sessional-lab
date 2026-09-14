#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int64 gcd(int64 a, int64 b) {
  while (b != 0) {
    int64 t = b;
    b = a % b;
    a = t;
  }

  return a;
}

int64 mul_mod(int64 a, int64 b, int64 mod) { return (a * b) % mod; }

int64 mod_pow(int64 base, int64 exp, int64 mod) {
  int64 result = 1;
  base %= mod;

  while (exp > 0) {
    if (exp & 1) result = mul_mod(result, base, mod);

    base = mul_mod(base, base, mod);
    exp >>= 1;
  }

  return result;
}

int64 egcd(int64 a, int64 b, int64 &x, int64 &y) {
  if (b == 0) {
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

int64 modInverse(int64 value, int64 mod) {
  int64 x, y;

  int64 g = egcd(value, mod, x, y);

  if (g != 1) return -1;

  x %= mod;

  if (x < 0) x += mod;

  return x;
}

int main() {
  int64 p = 467;
  int64 alpha = 2;
  int64 a = 123;
  int64 beta = mod_pow(alpha, a, p);
  cout << "Public Key :" << p << endl;
  cout << "alpha: " << alpha << endl;
  cout << "beta: " << beta << endl;
  cout << "Private Key = " << a << "\n";
  int64 m = 20;
  cout << "m: " << m << endl;
  int64 r = 7;
  int64 C1 = mod_pow(alpha, r, p);
  cout << "c1: " << C1 << endl;
  int64 beta_r = mod_pow(beta, r, p);
  cout << "beta_r: " << beta_r << endl;
  int64 C2 = mul_mod(m, beta_r, p);
  cout << "C2: " << C2 << endl;
  int64 s = mod_pow(C1, a, p);
  cout << "s: " << s << endl;

  int64 s_inv = modInverse(s, p);
  cout << "s_inv: " << s_inv << endl;
  int64 decrypted = mul_mod(C2, s_inv, p);
  cout << "decrypted: " << decrypted << endl;

  return 0;
}
