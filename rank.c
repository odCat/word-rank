#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

FILE * sample;
bool eof_flag = false;

struct word
{
    char *          str;
    unsigned short  rank;
};

/* prototypes */
void open_file();
long int count_char(FILE *);
void read_file(FILE *, char *data);
void print_data(char const *data);        // for tests
int count_words(char *data);

int main (void)
{
    open_file();

    return 0;
}

void open_file()
{
    sample = fopen ("sample.txt", "r");

    long c = count_char(sample);
    char data[c + 1];

    printf ("%ld\n", c);

    fclose (sample);
    sample = fopen ("sample.txt", "r");

    read_file(sample, data);
    printf ("Words: %d\n", count_words(data));
    /* print_data(data); */

    fclose (sample);
}

long int count_char(FILE *f)
{
    long count = 0;
    int  ch;

    while ((ch = getc(f)) != EOF)
        if (ch != '.' && ch != ',' && ch != ':')
            ++count;

    return count;
}

void read_file(FILE *f, char *data)
{
    int ch;
    
    while ((ch = getc(f)) != EOF)
        if (ch != '.' && ch != ',' && ch != ':')
            *data++ = ch;

    *data = '\0';
}

void print_data(char const *data)
{
    while (*data != '\0')
        printf ("%c", *data++);

    printf ("\n");
}

int count_words(char *data)
{
    _Bool in_word = false;
    int count = 0;

    while (*data != '\0')
    {
        if (!isspace(*data) && !in_word)
        {
            in_word = true;
            ++count;
        }

        if (isspace(*data) && in_word)
            in_word = false;

        ++data;
    }

    return count;
}
