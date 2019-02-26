#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

FILE * sample;
bool eof_flag = false;
unsigned short int max_length;


/* prototypes */
void open_file();
long int count_char(FILE *);
void read_file(FILE *, char *data);
void print_data(char const *data);        // for tests
int count_words(char *data);
void make_ranking(char * data);
char * get_next_word(char *data);

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

    printf ("Characters: %ld\n", c);

    fclose (sample);
    sample = fopen ("sample.txt", "r");

    read_file(sample, data);
    printf ("Words:      %d\n", count_words(data));
    printf ("Max length: %d\n", max_length);
    /* print_data(data); */

    /* struct word */
    /* { */
    /*     char            str[183]; */
    /*     unsigned short  rank; */
    /* }; */


    /* char words[c][200]; */

    if (count_words(data) > 0)
        make_ranking(data);

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
    unsigned short int length;
    int wcount = 0;

    while (*data != '\0')
    {
        if (!isspace(*data))
            if (!in_word)
            {
                length = 0;
                in_word = true;
                ++wcount;
            } else
                ++length;

        if (isspace(*data) && in_word)
        {
            in_word = false;
            if (length > max_length)
                max_length  = length;
        }

        ++data;
    }

    return wcount;
}

void make_ranking(char * data)
{
    printf ("Log: make_ranking\n");

    struct word
    {
        char            str[max_length];
        unsigned short  rank;
    };

    struct word list_head;
    struct word *list_current = &list_head;

    get_next_word(data);
    /* while (*data = "\0") */
}

char * get_next_word(char * data)
{
    static char next_word[200];
    int count = 0;

    while (!isspace(*data))
        next_word[count++] = *data++;

    next_word[count] = '\0';

    printf ("%s\n", next_word);

    return next_word;
}
