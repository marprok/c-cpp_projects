#include <stdio.h>

int main(void)
{
    int c;
    while ((c = getchar()) != EOF)
    {
        switch (c)
        {
        case '\t':
            putchar('\\');
            putchar('t');
            break;
        case '\b':
            putchar('\\');
            putchar('b');
            break;
        case '\\':
            putchar('\\');
            putchar('\\');
            break;
        default:
            putchar(c);
        }
    }

    return 0;
}
