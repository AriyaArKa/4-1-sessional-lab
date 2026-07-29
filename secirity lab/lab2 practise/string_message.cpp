#include <iostream>
#include <vector>
#include <string>

using namespace std;

using int64 = long long;


// ================= GCD =================

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


// ================= Extended GCD =================

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


// ================= Modular Inverse =================

int64 modInverse(int64 e, int64 phi)
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


// ================= Select e =================

int64 autoSelectE(int64 phi)
{
    int64 e = 2;

    while(gcd(e, phi) != 1)
        e++;

    return e;
}


// ================= Modular Multiplication =================
// Overflow-safe (a*b) % mod via add-and-double

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


// ================= Modular Power =================

int64 modPow(int64 base, int64 exp, int64 mod)
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


// ================= TEXT TO NUMBER =================
// Cast through unsigned char first so extended
// characters (codes 128-255) stay non-negative.

vector<int64> textToNumbers(const string &text)
{
    vector<int64> numbers;

    for(unsigned char c : text)
        numbers.push_back((int64)c);

    return numbers;
}


// ================= NUMBER TO TEXT =================

string numbersToText(const vector<int64> &numbers)
{
    string text;

    for(int64 n : numbers)
        text += (char)(unsigned char)n;

    return text;
}


int main()
{
    int64 p, q;

    cout<<"Enter prime p and q: ";
    cin>>p>>q;

    int64 n = p * q;
    int64 phi = (p - 1) * (q - 1);

    cout<<"n = "<<n<<", phi = "<<phi<<"\n";

    if(n <= 255)
    {
        cout<<"Warning: n must be > 255 to safely encode "
              "every character; pick larger primes.\n";
    }

    int64 e;

    cout<<"Enter public exponent e (0 for auto): ";
    cin>>e;

    if(e == 0)
    {
        e = autoSelectE(phi);
        cout<<"Auto-selected e = "<<e<<"\n";
    }
    else if(gcd(e, phi) != 1)
    {
        cout<<"e and phi(n) not coprime.\n";
        return 0;
    }

    int64 d = modInverse(e, phi);

    if(d == -1)
    {
        cout<<"No modular inverse for e mod phi.\n";
        return 0;
    }

    cout<<"Public Key:  (e,n) = ("<<e<<","<<n<<")\n";
    cout<<"Private Key: d = "<<d<<"\n";

    cin.ignore();

    string message;

    cout<<"\nEnter message: ";
    getline(cin, message);

    // Convert text into numbers
    vector<int64> plainNumbers = textToNumbers(message);

    cout<<"\nASCII values:\n";
    for(int64 x : plainNumbers)
        cout<<x<<" ";
    cout<<"\n";

    // ================= ENCRYPTION =================

    vector<int64> cipher;

    for(int64 m : plainNumbers)
        cipher.push_back(modPow(m, e, n));

    cout<<"\nEncrypted values:\n";
    for(int64 c : cipher)
        cout<<c<<" ";
    cout<<"\n";

    // ================= DECRYPTION =================

    vector<int64> decrypted;

    for(int64 c : cipher)
        decrypted.push_back(modPow(c, d, n));

    string original = numbersToText(decrypted);

    cout<<"\nDecrypted message:\n"<<original<<endl;

    return 0;
}
