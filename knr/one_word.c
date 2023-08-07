#include <stdio.h>

int main(void)
{
    int c, in_word = 0, print_nl = 0;
    while ((c = getchar()) != EOF)
    {
        in_word = c != ' ' && c != '\n' && c != '\t';
        if (in_word)
        {
            putchar(c);
            print_nl = 1;
        }
        else if (print_nl)
        {
            putchar('\n');
            print_nl = 0;
        }
    }

    return 0;
}
