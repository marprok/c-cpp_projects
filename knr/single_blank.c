#include <stdio.h>

int main(void)
{
    int c, skip = 0;
    while ((c = getchar()) != EOF)
    {
        if (c != ' ')
        {
            putchar(c);
            skip = 0;
        }
        else if (c == ' ' && !skip)
        {
            putchar(c);
            skip = 1;
        }
    }

    return 0;
}
