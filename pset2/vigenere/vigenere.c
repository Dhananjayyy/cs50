//Program to obtain secret key and hash a message
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    //Error Checking
    if (argc != 2)
    {
        printf("Error: Program accepts more than one argument\n");
        return 1;
    }

    //Storing Key and Plaintext Message
    int key[strlen(argv[1])];

    for (int i = 0; i < strlen(argv[1]); i++)
    {
        char sym = argv[1][i];
        if (isupper(sym))
        {
            sym = sym - 'A';
        }
        else if (islower(sym))
        {
            sym = sym - 'a';
        }
        else
        {
            printf("Error: Non alphabetic characters not supported as secret key");
            return 1;
        }
        key[i] = sym;
    }

    string src = get_string("plaintext: ");

    for (int i = 0, counter = 0; i < strlen(src); i++)
    {
        // Remembr sizeof(int) > sizeof(char)

        if (isalpha(src[i]))
        {
            counter = counter % strlen(argv[1]);
            int rkey = key[counter];
            //Lcase
            if (islower(src[i]))
            {
                src[i] = (src[i] + rkey - 'a') % 26 + 'a';
            }
            //Ucase
            else if (isupper(src[i]))
            {
                src[i] = (src[i] + rkey - 'A') % 26 + 'A';
            }

            //Reset Counter to make secret code loop back
            counter++;
        }
    }

    printf("ciphertext: %s\n", src);
    return 0;
}