#include <iostream>
using namespace std;

// Encrypt the text
void encrypt(char text[], int key)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        // Uppercase letter
        if (text[i] >= 'A' && text[i] <= 'Z')
        {
            text[i] = (text[i] - 'A' + key) % 26 + 'A';
        }

        // Lowercase letter
        else if (text[i] >= 'a' && text[i] <= 'z')
        {
            text[i] = (text[i] - 'a' + key) % 26 + 'a';
        }

        // Space, number, symbol → unchanged
    }
}

// Decrypt the text
void decrypt(char text[], int key)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        // Uppercase letter
        if (text[i] >= 'A' && text[i] <= 'Z')
        {
            text[i] = (text[i] - 'A' - key + 26) % 26 + 'A';
        }

        // Lowercase letter
        else if (text[i] >= 'a' && text[i] <= 'z')
        {
            text[i] = (text[i] - 'a' - key + 26) % 26 + 'a';
        }
    }
}

int main()
{
    char text[1000];
    int key;

    cout << "Enter text: ";
    cin.getline(text, 1000);

    cout << "Enter key: ";
    cin >> key;

    // Keep key between 0 and 25
    key = key % 26;

    // Encryption
    encrypt(text, key);
    cout << "\nEncrypted: " << text << endl;

    // Decryption
    decrypt(text, key);
    cout << "Decrypted: " << text << endl;

    return 0;
}