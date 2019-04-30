#include <cs50.h>
#include <stdio.h>
#include <string.h>

void repeat(char a, int times);

int main(void)
{
    int height;
    do
    {
        height = get_int("Enter non negative height: ");
    }
    while (height > 23 || height < 0);

    for (int i = 0; i < height; i++)
    {
        repeat(' ', height - i - 1);
        repeat('#', i + 1);
        repeat(' ', 2);
        repeat('#', i + 1);
        printf("\n");
    }
}

void repeat(char a, int times)
{
    for (int i = 0; i < times; i++)
    {
        printf("%c", a);
    }
}