#include<bits/stdc++.h>

using namespace std;

using int64 = long long;

int64 gcd(int64 a,int64 b)
{
    while(b!=0)
    {
        int64 t = b;
        b = a%b;
        a = t;
    }
    return a;
}
int64 egcd(int64 a,int64 b,int64 &x,int64 &y)
{
    if(b==0)
    {
        x=1;
        y=0;
        return a;
    }
    int64 x1,y1;
    int64 g = egcd(b,a%b,x1,y1);
    x= y1;
    y= x1-(a/b)*y1;

    return g;
}
int64 addmod(int64 a,int64 b,int64 mod)
{
    if(a>=mod-b)
    {
        return a-(mod-b);
    }
    return a+b;
}
int64 mulmod(int64 base,int64 exp,int64 mod)
{
    int64 result = 0;
    base %= mod;
    while(exp>0)
    {
        if(exp&1)
        {
            result = addmod(result,base,mod);
        }
        base = addmod(base,base,mod);
        exp>>=1;
    }
    return result;
}
int64 modpow(int64 base,int64 exp,int64 mod)
{
    int64 result = 1;
    base %= mod;
    while(exp>0)
    {
        if(exp&1)
        {
            result = mulmod(result,base,mod);
        }
        base = mulmod(base,base,mod);
        exp>>=1;

    }
    return result;
}
int64 modinv(int64 e,int64 phi)
{
    int64 x,y;
    int64 g = egcd(e,phi,x,y);

    if(g!=1)
    {
        return -1;
    }
    x%=phi;
    if(x<0)
    {
        x+=phi;
    }
    return x;
}
int64 autoe(int64 phi)
{
    int64 e =2;
    while(gcd(e,phi)!=1)
    {
        e++;
    }
    return e;
}

int main()
{
    // ================= PRODUCT CIPHER PROOF =================

int64 p = 10007;
int64 q = 10009;

int64 n = p * q;
int64 phi = (p - 1) * (q - 1);

int64 e = autoe(phi);
cout << "e : " << e << endl;

int64 d = modinv(e, phi);
cout << "d : " << d << endl;


// ================= MESSAGE 1 =================

int64 m1 = 23;

int64 c1 = modpow(m1, e, n);
int64 m11 = modpow(c1, d, n);

cout << "\nMessage 1" << endl;
cout << "m1  : " << m1 << endl;
cout << "c1  : " << c1 << endl;
cout << "m11 : " << m11 << endl;


// ================= MESSAGE 2 =================

int64 m2 = 38;

int64 c2 = modpow(m2, e, n);
int64 m22 = modpow(c2, d, n);

cout << "\nMessage 2" << endl;
cout << "m2  : " << m2 << endl;
cout << "c2  : " << c2 << endl;
cout << "m22 : " << m22 << endl;


// ========================================================
// PRODUCT PROPERTY
// c1 * c2 = E(m1) * E(m2)
// ========================================================

// Safe modular multiplication
int64 c3 = mulmod(c1, c2, n);

// Decrypt the product
int64 m33 = modpow(c3, d, n);

cout << "\nProduct Cipher" << endl;
cout << "c1 * c2 mod n : " << c3 << endl;
cout << "Decrypt(c1*c2): " << m33 << endl;


// Expected plaintext product
int64 expected = mulmod(m1, m2, n);

cout << "m1 * m2 mod n : " << expected << endl;


// ========================================================
// INDIVIDUAL DECRYPTION
// D(c1) * D(c2)
// ========================================================

int64 dm1 = modpow(c1, d, n);
int64 dm2 = modpow(c2, d, n);

int64 product = mulmod(dm1, dm2, n);

cout << "\nIndividual Decryption Product" << endl;
cout << "D(c1) * D(c2) : " << product << endl;





    return 0;

    
}