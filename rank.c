/*
 * Copyright 2019 Mihai Gătejescu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef DEBUG_ON
#   define DEBUG(...) fprintf (stderr,  __VA_ARGS__)
#   define DEBUG_DATA(arg) print_data(arg)
#else
#   define DEBUG(...)
#   define DEBUG_DATA(arg)
#endif

FILE * sample;
bool eof_flag = false;
unsigned short int max_length = 0;


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

    fclose (sample);
    sample = fopen ("sample.txt", "r");

    read_file(sample, data);

    DEBUG ("Characters: %ld\n", c);
    DEBUG ("Words:      %d\n", count_words(data));
    DEBUG ("Max length: %d\n", max_length);
    DEBUG_DATA (data);

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
    printf ("\ndata: \n");

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
                length = 1;
                in_word = true;
                ++wcount;
            } else
                ++length;

        if (isspace(*data) && in_word)
        {
            in_word = false;
            if (length > max_length)
            {
                DEBUG ("Length: %i\n", length);
                max_length = length;
                in_word = false;
            }
        }

        ++data;
    }

    return wcount;
}

int make_ranking(char * data)
{
    DEBUG ("Log: make_ranking\n");

    struct word
    {
        char            str[max_length];
        unsigned short  rank;
        struct word *next;
    };
    char * next_word;
    struct word *temp;
    /* int is_ranked(char * word); */

    struct word *list_head = (struct word *) 0;

    while (*data != '\0')
    {
        temp = (struct word *) calloc (1, sizeof(struct word));
        if (temp == (struct word *) 0)
        {
            fprintf (stderr, "calloc: allocation failed\n");
            exit(1);
        }
        if (list_head == (struct word *) 0)
            list_head = temp;
        
        next_word = get_next_word(data);

        // Check if the word is new
        _Bool found = false;
        while (1)
        {
            if (next_word == temp->str)
            {
                ++temp->rank;
                found = true;
                break;
            }
            if (temp->next != (struct word *) 0)
                temp = temp->next;
            else
                break;
        }

        if (!found)
        {
            temp->next = (struct word *)
                calloc (1, sizeof(struct word *));
            if (temp->next == (struct word *) 0)
            {
                fprintf (stderr, "calloc: allocation failed\n");
                exit(1);
            } else {
                temp = temp->next;
                strcpy (temp->str, next_word);
            }
        }

        // Debug statements
        /* DEBUG ("%s\n", list_head->str); */
        /* DEBUG ("%s\n", list_head->next->str); */
        /* if (list_head->next == NULL) */
        /*     DEBUG ("Null pointer\n"); */
        /* break; */
    }

    /* Print list rankings */
    temp = list_head;
    while (temp != (struct word *) 0)
    {
        printf ("%s%5i\n", temp->str, temp->rank);
        temp = temp->next;
    }

    /* Delete list rankeings */
    while (list_head != (struct word *) 0)
    {
        temp = list_head;
        while (temp->next != (struct word *) 0)
            temp = temp->next;
        DEBUG ("Free a word\n");
        free(list_head);
        list_head = NULL;
        if (list_head == NULL)
        {
            DEBUG ("Null head\n");
            break;
        }
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

    DEBUG ("%s\n", next_word);

    return next_word;
}
