#include <stdio.h>

int main(void)
{
    int c, blanks = 0, tabs = 0, newlines = 0;
    while ((c = getchar()) != EOF)
    {
        switch(c)
        {
            case ' ':
                blanks++;
                break;
            case '\t':
                tabs++;
                break;
            case '\n':
                newlines++;
                break;
            default:
                break;
        }
    }

    printf("\nblanks: %d tabs %d newlines %d\n", blanks, tabs, newlines);
    return 0;
}
