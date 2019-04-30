#include <stdio.h>
#include <cs50.h>
#include <math.h>

//Greedy Program
int main(void)
{
    float c;
    int cents;
    int coins;

    do
    {
        c = get_float("Change owed: ");
    }
    while (c < 0);

    //Cent Conversion
    cents = (int) round(100 * c);
    coins = 0;

    while (cents >= 25)
    {
        cents = cents - 25;
        coins++;
    }

    while (cents >= 10)
    {
        cents = cents - 10;
        coins++;
    }

    while (cents >= 5)
    {
        cents = cents - 5;
        coins++;
    }

    coins = coins + cents;

    printf("%i\n", coins);
}