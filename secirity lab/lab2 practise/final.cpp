#include <iostream>
#include <vector>
using namespace std;

using int64 = long long;


// Greatest Common Divisor
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
// Finds x and y such that:
// ax + by = gcd(a,b)
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


// Finds modular inverse
// Finds d where:
// (e*d) mod phi = 1
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


// Automatically selects e
// Condition:
// gcd(e,phi)=1
int64 autoSelectE(int64 phi)
{
    int64 e = 2;

    while(gcd(e, phi) != 1)
    {
        e++;
    }

    return e;
}


int main()
{
    // Two 10-digit prime numbers
    // int64 p = 1000000007;
    // int64 q = 1000000009;

    // int64 p = 10007;
    // int64 q = 10009;

    // int64 p = 1000003;
    // int64 q = 1000033;

    int64 p = 5;
    int64 q = 11;

    cout<<"Prime p = "<<p<<endl;
    cout<<"Prime q = "<<q<<endl;


    // RSA modulus
    // n = p*q
    int64 n = p * q;


    // Euler Totient
    // phi(n)=(p-1)(q-1)
    int64 phi = (p-1)*(q-1);


    cout<<"\nn = "<<n<<endl;
    cout<<"phi = "<<phi<<endl;



    // e selection option

    int choice;

    cout<<"\nChoose e selection method:"<<endl;
    cout<<"1. Enter e manually"<<endl;
    cout<<"2. Calculate e automatically"<<endl;

    cout<<"Enter choice: ";
    cin>>choice;


    int64 e;


    if(choice == 1)
    {
        cout<<"Enter value of e: ";
        cin>>e;


        // Check e validity

        if(gcd(e,phi)!=1)
        {
            cout<<"Invalid e. gcd(e,phi) must be 1"<<endl;
            return 0;
        }
    }


    else if(choice == 2)
    {
        e = autoSelectE(phi);

        cout<<"Automatically selected e = "
            <<e<<endl;
    }


    else
    {
        cout<<"Invalid choice"<<endl;
        return 0;
    }



    cout<<"\nChosen public exponent e = "
        <<e<<endl;



    // Calculate smallest private key

    int64 d = modInverse(e,phi);


    if(d == -1)
    {
        cout<<"No modular inverse exists"<<endl;
        return 0;
    }


    cout<<"\nSmallest private key d = "
        <<d<<endl;



    // Finding multiple d values
    // All solutions of e*d = 1 (mod phi) are
    // d = d0 + t*phi, for t = 0, 1, 2, ...

    cout<<"\nFirst 3 possible d values:"<<endl;


    int64 d_candidate = d;

    for(int cnt = 0; cnt < 3; cnt++)
    {
        cout<<"d"<<cnt+1
            <<" = "
            <<d_candidate
            <<endl;

        d_candidate += phi;
    }


    return 0;
}