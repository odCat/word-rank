#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

FILE * sample;
bool eof_flag = false;
unsigned short int max_length;


/* prototypes */
void open_file();
long int count_char(FILE *);
void read_file(FILE *, char *data);
void print_data(char const *data);        // for tests
int count_words(char *data);
int make_ranking(char *data);
char * get_next_word(char *data);

int main (int argc, char *argv[])
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

int make_ranking(char * data)
{
    printf ("Log: make_ranking\n");

    struct word
    {
        char            str[max_length];
        unsigned short  rank;
        struct word *next;
    };
    char * next_word;
    int is_ranked(char * word);

    struct word *list_head = (struct word *) 0;

    while (*data != '\0')
    {
        struct word *temp = (struct word *)
            calloc (1, sizeof(struct word));
        if (temp == (struct word *) 0)
        {
            fprintf (stderr, "calloc: allocation failed\n");
            exit(1);
        }
        if (list_head == (struct word *) 0)
            list_head = temp;
        
        next_word = get_next_word(data);

        // Check if the word is new
        if (1)
            strcpy (temp->str, next_word);

        printf ("%s\n", list_head->str);

        if (list_head->next == NULL)
            printf ("Null pointer\n");

        /* Delete list rankeings */
        while (list_head != (struct word *) 0)
        {
            temp = list_head;
            while (temp->next != (struct word *) 0)
                temp = temp->next;
            printf ("Free a word\n");
            free(list_head);
            list_head = NULL;
            if (list_head == NULL)
            {
                printf ("Null head\n");
                break;
            }
        }

        break;

    }

    return 0;
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

/* int is_ranked(char * word) */
/* { */

/*     if (strcmp(list_head->str, "Comunicat")) */
/*         return 0; */
/*     else */
/*         return 1; */
/* } */
