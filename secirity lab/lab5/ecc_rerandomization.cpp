#include <bits/stdc++.h>
using namespace std;


// x mod p

long long mod(long long x,long long p)
{
    x%=p;

    if(x<0)
        x+=p;

    return x;
}


// Modular Inverse

long long inverse(long long a,long long p)
{
    for(long long i=1;i<p;i++)
    {
        if((a*i)%p==1)
            return i;
    }

    return -1;
}



// Point Addition

pair<long long,long long> add(
    pair<long long,long long> P,
    pair<long long,long long> Q,
    long long a,
    long long p)
{


    // Infinity point

    if(P.first==-1)
        return Q;


    if(Q.first==-1)
        return P;



    long long x1=P.first;
    long long y1=P.second;


    long long x2=Q.first;
    long long y2=Q.second;



    // P + (-P)=O

    if(x1==x2 && mod(y1+y2,p)==0)
        return {-1,-1};



    long long s;



    // Point Doubling

    if(P==Q)
    {

        long long num =
        mod(3*x1*x1+a,p);


        long long den =
        mod(2*y1,p);


        s =
        mod(num*inverse(den,p),p);

    }



    // Normal Addition

    else
    {

        long long num =
        mod(y2-y1,p);


        long long den =
        mod(x2-x1,p);



        s =
        mod(num*inverse(den,p),p);

    }



    long long x3 =
    mod(s*s-x1-x2,p);



    long long y3 =
    mod(s*(x1-x3)-y1,p);



    return {x3,y3};

}



// Scalar Multiplication kG

pair<long long,long long> multiply(
    pair<long long,long long> G,
    long long k,
    long long a,
    long long p)
{

    pair<long long,long long> result =
    {-1,-1};



    while(k>0)
    {

        if(k%2==1)
        {
            result =
            add(result,G,a,p);
        }



        G =
        add(G,G,a,p);



        k=k/2;

    }


    return result;

}



// Point Subtraction

pair<long long,long long> subtract(
    pair<long long,long long> P,
    pair<long long,long long> Q,
    long long a,
    long long p)
{

    // -Q=(x,-y)

    Q.second =
    mod(-Q.second,p);


    return add(P,Q,a,p);

}



// Print Point

void print(pair<long long,long long> P)
{

    if(P.first==-1)
        cout<<"O";

    else
        cout<<"("<<P.first<<","<<P.second<<")";

}



// ================================
// RE-RANDOMIZATION
// ================================
//
// C1' = C1 + rG
//
// C2' = C2 + rQ
//
// ================================


void rerandomize(
    pair<long long,long long> C1,
    pair<long long,long long> C2,
    pair<long long,long long> G,
    pair<long long,long long> Q,
    long long r,
    long long a,
    long long p
)
{
    // rG
    pair<long long,long long> rG =multiply(G,r,a,p);
    // rQ
    pair<long long,long long> rQ =multiply(Q,r,a,p);


    // New ciphertext
    pair<long long,long long> newC1 =add(C1,rG,a,p);
    pair<long long,long long> newC2 =add(C2,rQ,a,p);

    cout<<"\n===== RE-RANDOMIZATION ====="<<endl;

    cout<<"Old C1 = ";
    print(C1);
    cout<<"\nOld C2 = ";
    print(C2);

    cout<<"\n\nNew C1 = ";
    print(newC1);
    cout<<"\nNew C2 = ";
    print(newC2);

    // Verify message after rerandomization
    long long d=7;

    pair<long long,long long> secret =
    multiply(newC1,d,a,p);
    pair<long long,long long> message =
    subtract(newC2,secret,a,p);
    cout<<"\n\nDecrypted After Re-randomization = ";
    print(message);


}



int main()
{

    // Curve

    // y^2=x^3+2x+3 mod17
    long long a=2;
    long long p=17;

    // Generator point
    pair<long long,long long> G ={3,6};


    // Private key
    long long d=7;

    // Public key
    // Q=dG
    pair<long long,long long> Q =multiply(G,d,a,p);

    cout<<"===== KEY GENERATION ====="<<endl;
    cout<<"G = ";
    print(G);

    cout<<"\nPrivate Key d = "<<d;
    cout<<"\nPublic Key Q = ";
    print(Q);

    // =========================
    // MESSAGE
    // =========================

    pair<long long,long long> M =
    multiply(G,5,a,p);

    cout<<"\n\nMessage M = ";
    print(M);

    // =========================
    // ENCRYPTION
    // =========================


    long long k=3;
    pair<long long,long long> C1 =multiply(G,k,a,p);
    pair<long long,long long> C2 =add( M,multiply(Q,k,a,p),a, p);

    cout<<"\n\n===== ENCRYPTION =====";

    cout<<"\nC1 = ";
    print(C1);
    cout<<"\nC2 = ";
    print(C2);

    // =========================
    // RE-RANDOMIZATION
    // =========================

    long long r=4;
    rerandomize(
        C1,
        C2,
        G,
        Q,
        r,
        a,
        p
    );
    return 0;
}