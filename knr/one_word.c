#include <stdio.h>

int main(void)
{
    int c, in_word = 0;
    while ((c = getchar()) != EOF)
    {
        if (c != ' ' && c != '\n' && c != '\t')
        {
            putchar(c);
            in_word = 1;
        }
        else if (in_word)
        {
            putchar('\n');
            in_word = 0;
        }
    }

    return 0;
}
