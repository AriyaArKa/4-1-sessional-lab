#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;


// -------------------------
// Manual XOR
// -------------------------
char manualXOR(char a, char b)
{
    int x = a;
    int y = b;

    int result = 0;
    int place = 1;

    while(x > 0 || y > 0)
    {
        int bit1 = x % 2;
        int bit2 = y % 2;
        if(bit1 != bit2)
        {
            result += place;
        }
        x = x / 2;
        y = y / 2;

        place = place * 2;
    }
    return (char)result;
}



// -------------------------
// Print Binary manually
// -------------------------
void printBinary(char x)
{
    int value = x;
    int bin[8];

    for(int i=7;i>=0;i--)
    {
        bin[i] = value % 2;
        value = value / 2;
    }

    for(int i=0;i<8;i++)
    {
        cout<<bin[i];
    }
}

// -------------------------
// Generate random key string
// -------------------------
string generateKey(int size)
{
    string k="";
    for(int i=0;i<size;i++)
    {
        char c = (rand()%26)+'A';

        k += c;
    }
    return k;
}




int main()
{

    int row,col;
    cout<<"Enter row size: ";
    cin>>row;
    cout<<"Enter column size: ";
    cin>>col;
    string plain[10][10];
    string transpose[10][10];
    string key[10][10];
    string cipher[10][10];
    string decrypt[10][10];
    string original[10][10];
    //-------------------------
    // Input Matrix
    // -------------------------

    cout<<"\nEnter Matrix:\n";
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            cin>>plain[i][j];
        }
    }

    cout<<"\nOriginal Matrix:\n";

    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            cout<<plain[i][j]<<"\t";
        }

        cout<<endl;
    }

    // -------------------------
    // Transpose
    // -------------------------

    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            transpose[j][i]=plain[i][j];
        }
    }


    cout<<"\nTranspose Matrix:\n";


    for(int i=0;i<col;i++)
    {
        for(int j=0;j<row;j++)
        {
            cout<<transpose[i][j]<<"\t";
        }

        cout<<endl;
    }

    // -------------------------
    // Generate Key
    // -------------------------

    cout<<"\nRandom Key Matrix:\n";


    for(int i=0;i<col;i++)
    {
        for(int j=0;j<row;j++)
        {
            key[i][j]=generateKey(transpose[i][j].length());

            cout<<key[i][j]<<"\t";
        }

        cout<<endl;
    }

    // -------------------------
    // Encryption
    // -------------------------

    for(int i=0;i<col;i++)
    {
        for(int j=0;j<row;j++)
        {

            cipher[i][j]="";


            for(int k=0;k<transpose[i][j].length();k++)
            {
                cipher[i][j] += manualXOR(
                    transpose[i][j][k],
                    key[i][j][k]
                );
            }

        }
    }

    cout<<"\nCipher Text Binary:\n";


    for(int i=0;i<col;i++)
    {
        for(int j=0;j<row;j++)
        {

            cout<<"[";


            for(int k=0;k<cipher[i][j].length();k++)
            {
                printBinary(cipher[i][j][k]);
                cout<<" ";
            }


            cout<<"]\t";
        }

        cout<<endl;
    }

    // -------------------------
    // Decryption
    // -------------------------

    for(int i=0;i<col;i++)
    {
        for(int j=0;j<row;j++)
        {

            decrypt[i][j]="";


            for(int k=0;k<cipher[i][j].length();k++)
            {

                decrypt[i][j] += manualXOR(
                    cipher[i][j][k],
                    key[i][j][k]
                );

            }

        }
    }

    cout<<"\nDecrypted Transpose Matrix:\n";

    for(int i=0;i<col;i++)
    {
        for(int j=0;j<row;j++)
        {
            cout<<decrypt[i][j]<<"\t";
        }

        cout<<endl;
    }

    // -------------------------
    // Reverse Transpose
    // -------------------------

    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            original[i][j]=decrypt[j][i];
        }
    }

    cout<<"\nRecovered Original Matrix:\n";

    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            cout<<original[i][j]<<"\t";
        }

        cout<<endl;
    }

    return 0;
}