#include <stdio.h>

#define WORD_SIZES 20
#define MAX_STARS 1
#define MAX_WORDS_PER_SIZE 1

int main(void)
{
    // 1...WORD_SIZES
    int words[WORD_SIZES];
    int stars[WORD_SIZES];
    for (int i = 0; i < WORD_SIZES; ++i)
        stars[i] = words[i] = 0;

    int c, count = 0;
    while ((c = getchar()) != EOF)
    {
        if (c != ' ' && c != '\n' && c != '\t')
            count++;
        else
        {
            if (count > 0 && count <= WORD_SIZES && words[count - 1] < MAX_WORDS_PER_SIZE)
                words[count - 1]++;
            count = 0;
        }
    }

    for (int i = 0; i < WORD_SIZES; ++i)
    {
        stars[i] = words[i] / (float)MAX_WORDS_PER_SIZE * MAX_STARS;
        printf("%02d: ", i + 1);
        for (int star = 0; star < stars[i]; ++star)
            putchar('*');
        putchar('\n');
    }

    putchar('\n');
    for (int j = MAX_STARS; j > 0; j--)
    {
        for (int i = 0; i < WORD_SIZES; ++i)
        {
            putchar(' ');
            if (stars[i] >= j)
                putchar('*');
            else
                putchar(' ');
            putchar(' ');
        }
        putchar('\n');
    }

    for (int i = 0; i < WORD_SIZES; ++i)
        printf("%02d ", i + 1);

    putchar('\n');
    return 0;
}
