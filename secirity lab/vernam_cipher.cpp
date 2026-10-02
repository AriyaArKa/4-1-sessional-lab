#include <iostream>
#include <string>
using namespace std;

// Manual XOR
char XOR(char a, char b)
{
    char result = 0;
    int power = 1;

    for (int i = 0; i < 8; i++)
    {
        int x = a % 2;
        int y = b % 2;

        if (x != y)
            result += power;

        a = a / 2;
        b = b / 2;

        power = power * 2;
    }

    return result;
}

// Print binary
void binary(char x)
{
    int b[8];

    for (int i = 7; i >= 0; i--)
    {
        b[i] = x % 2;
        x = x / 2;
    }

    for (int i = 0; i < 8; i++)
        cout << b[i];
}

int main()
{
    int n;

    cout << "Enter matrix size: ";
    cin >> n;

    string a[10][10];
    string t[10][10];
    string key[10][10];
    string enc[10][10];
    string dec[10][10];

    // -------------------------------
    // Input matrix
    // -------------------------------
    cout << "\nEnter matrix:\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> a[i][j];
        }
    }

    // -------------------------------
    // Display original
    // -------------------------------
    cout << "\nOriginal Matrix:\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << a[i][j] << "\t";

        cout << endl;
    }

    // -------------------------------
    // Transpose
    // -------------------------------
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            t[j][i] = a[i][j];
    }

    cout << "\nTransposed Matrix:\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << t[i][j] << "\t";

        cout << endl;
    }

    // -------------------------------
    // Binary
    // -------------------------------
    cout << "\nTransposed Matrix in Binary:\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << "[";

            for (int k = 0; k < t[i][j].length(); k++)
            {
                binary(t[i][j][k]);
                cout << " ";
            }

            cout << "]\t";
        }

        cout << endl;
    }

    // -------------------------------
    // Input key
    // -------------------------------
    cout << "\nEnter key matrix:\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> key[i][j];
        }
    }

    // -------------------------------
    // Encryption
    // -------------------------------
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            enc[i][j] = "";

            for (int k = 0; k < t[i][j].length(); k++)
            {
                enc[i][j] += XOR(
                    t[i][j][k],
                    key[i][j][k]
                );
            }
        }
    }

    // -------------------------------
    // Encrypted binary
    // -------------------------------
    cout << "\nEncrypted Matrix in Binary:\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << "[";

            for (int k = 0; k < enc[i][j].length(); k++)
            {
                binary(enc[i][j][k]);
                cout << " ";
            }

            cout << "]\t";
        }

        cout << endl;
    }

    // -------------------------------
    // Decryption
    // -------------------------------
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            dec[i][j] = "";

            for (int k = 0; k < enc[i][j].length(); k++)
            {
                dec[i][j] += XOR(
                    enc[i][j][k],
                    key[i][j][k]
                );
            }
        }
    }

    // -------------------------------
    // Decrypted matrix
    // -------------------------------
    cout << "\nDecrypted Matrix:\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << dec[i][j] << "\t";

        cout << endl;
    }

    return 0;
}
