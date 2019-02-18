#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

FILE * sample;
bool eof_flag = false;

/* prototypes */
void open_file();
long int count_char(FILE *);
/* char * read_words(void); */

int main (void)
{
    open_file();

    return 0;
}

void open_file()
{
    sample = fopen ("sample.txt", "r");

    printf ("%ld\n", count_char(sample));

    fclose (sample);
}

long int count_char(FILE *f)
{
    long count = 0;

    while (getc(f) != EOF)
    {
        ++count;
    }

    return count;
}

/* char * read_words(void) */
/* { */
/*     int     ch; */
/*     char    temp; */
/*     char    *word, *word_temp; */
/*     bool    in_word = false; */

/*     word_temp = word = &temp; */
/*     while ((ch = getc(sample)) != EOF) */
/*     { */
/*         if (isalpha(ch)) */
/*         { */
/*             if (!in_word) */
/*                 in_word = true; */
/*             *word_temp = ch; */
/*             word_temp++; */
/*         } else */
/*             in_word = false; */
/*     } */
/*     *word_temp = '\0'; */
/*     eof_flag = true; */

/*     return word; */
/* } */
