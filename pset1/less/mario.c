#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;

    //Input must be non negative and greater than 23
    do
    {
        height = get_int("Height: ");
    }
    while (height < 0 || height > 23);

    //Making the actual pyramid
    for (int i = 0; i < height; i++)
    {
        //Making Spaces, Spaces = Total Height - Current Level
        for (int spaces = 0; spaces < (height - i - 1); spaces++)
        {
            printf(" ");
        }

        //Making Hashes which is just equal to the Current Level
        for (int hash = 0; hash < (i + 2); hash++)
        {
            printf("#");
        }

        printf("\n");
    }
}