#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

FILE * sample;
bool eof_flag = false;

/* prototypes */
char * read_words(void);

int main (void)
{
    sample = fopen ("sample.txt", "r");

    char *string;

    while (!eof_flag)
        printf ("%s ", read_words());

    fclose (sample);

    return 0;
}

char * read_words(void)
{
    int     ch;
    char    *word, *word_temp;
    bool    in_word = false;

    word_temp = word;
    while ((ch = getc(sample)) != EOF)
    {
        if (isalpha(ch))
        {
            if (!in_word)
                in_word = true;
            *word_temp = ch;
            word_temp++;
        } else
            in_word = false;
    }
    *word_temp = '\0';
    eof_flag = true;

    return word;
}
