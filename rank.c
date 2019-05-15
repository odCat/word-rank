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

#define INPUT_FILE "sample.txt"

#ifdef DEBUG_ON
#   define DEBUG(...) fprintf (stderr,  __VA_ARGS__)
#   define DEBUG_DATA(arg) print_data(arg)
#else
#   define DEBUG(...)
#   define DEBUG_DATA(arg)
#endif

// TODO: update this list
//       Romanian quotes are not working...
#define NOT_PUNCTUATION(ch) \
        (ch != '.' && ch != ',' && ch != ':' && ch != ';'    \
         && ch != '?' && ch != '!' && ch != '(' && ch != ')' \
         && ch != '"' && ch != '\'')

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
    if ((sample = fopen (INPUT_FILE, "r")) == NULL)
    {
        printf ("Could not open file %s\n", INPUT_FILE);;
        exit(1);
    }

    long c = count_char(sample);
    char data[c + 1];

    fclose (sample);
    if ((sample = fopen (INPUT_FILE, "r")) == NULL)
    {
        printf ("Could not open file %s\n", INPUT_FILE);;
        exit(1);
    }

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
        if (NOT_PUNCTUATION(ch))
            ++count;

    return count;
}

/*
 * read data from file jumping over empty lines
 *   and punctuation
 */
void read_file(FILE *f, char *data)
{
    int ch;
    _Bool preceded_by_nl = false;
    _Bool found_hyphen = false;
    _Bool not_started = true;
    
    while ((ch = getc(f)) != EOF)
    {
        if (NOT_PUNCTUATION(ch))
        {
            switch (ch)
            {
                case ' ':
                    if (not_started)
                        continue;
                    if (*(data - 1) == ' ' || *(data - 1) == '\n')
                        continue;
                    if (found_hyphen)
                    {
                        --data;
                        continue;
                    }
                case '-':
                    if (*(data - 1) == ' ')
                        continue;
                    found_hyphen = true;
                    break;
                case '\n':
                    if (preceded_by_nl)
                    {
                        continue;
                    } else
                        if (found_hyphen)
                        {
                            --data;
                            found_hyphen = false;
                            continue;
                        }
                    preceded_by_nl = true;
                    break;
                case 152:           // Left single quotation mark
                case 153:           // Right single quotation mark
                case 156:           // Left double quotation mark
                case 157:           // Right double quotation mark
                    if (*(data - 1 ) == -128 && *(data - 2) == -30)
                    {
                        data -= 2;
                        continue;
                    }
                default:
                {
                    preceded_by_nl = false;
                    found_hyphen = false;
                }
            }
            if (not_started)
                not_started = false;
            *data++ = ch;
        } 
    }

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
        struct word     *next;
    };
    char * next_word;
    struct word *temp;
    /* int is_ranked(char * word); */

    struct word *list_head = (struct word *) 0;

    while (*data != '\0')
    {
        next_word = get_next_word(data);

        // Initialize list if neccessary
        if (list_head == (struct word *) 0)
        {
            temp = (struct word *) calloc (1, sizeof(struct word));
            if (temp == (struct word *) 0)
            {
                fprintf (stderr, "calloc: allocation failed\n");
                exit(1);
            }
            list_head = temp;
            strcpy (list_head->str, next_word);
            DEBUG ("Head\nStr: %s\nRank: %i\n",
                    list_head->str, list_head->rank);
        }

        /* DEBUG ("temp->rank = %i\n", temp->rank); */

        // Check if the word is new
        _Bool found = false;
        temp = list_head;
        while (1)
        {
            if (strcmp(next_word, temp->str) == 0)
            {
                DEBUG ("%s = %s\n", next_word, temp->str);
                ++temp->rank;
                found = true;
                break;
            }
            if (temp->next != (struct word *) 0)
                temp = temp->next;
            else
                break;
        }

        DEBUG ("temp->rank = %i\n", temp->rank);

        if (!found)
        {
            temp->next = (struct word *)
                calloc (1, sizeof(struct word *));
            if (temp->next == (struct word *) 0)
            {
                fprintf (stderr, "calloc: allocation failed\n");
                exit(1);
            }
            temp = temp->next;
            ++temp->rank;
            strcpy (temp->str, next_word);
        }

        data += strlen(next_word) + 1;
        // Debug statements
        /* DEBUG ("%s\n", list_head->str); */
        /* DEBUG ("%s\n", list_head->next->str); */
        /* if (list_head->next == NULL) */
        /*     DEBUG ("Null pointer\n"); */
        /* break; */
    }

    /* Print list rankings */
    temp = list_head;
    printf ("\nRanking list:\n\n");
    while (temp != (struct word *) 0)
    {
        printf ("%4i    %s\n", temp->rank, temp->str);
        temp = temp->next;
    }
    printf ("\n");

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
