//Program to obtain secret key and hash a message
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    //Error Checking
    if (argc != 2)
    {
        printf("Error: Program accepts only two arguments\n");
        return 1;
    }

    //Storing Key and Plaintext Message
    int key = atoi(argv[1]);
    string src = get_string("plaintext: ");

    for (int i = 0; i < strlen(src); i++)
    {
        //Since sizeof(int) > sizeof(char)
        int k = src[i];

        //Lcase
        if (islower(src[i]))
        {
           k = (k - 'a' + key) % 26 + 'a';
        }
        //Ucase
        else if (isupper(src[i]))
        {
            k = (k - 'A' + key) % 26 + 'A';
        }
        src[i] = k;
    }

    printf("ciphertext: %s\n", src);
    return 0;
}

