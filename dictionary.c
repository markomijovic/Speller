// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"
#include <string.h>
#include <strings.h>
#include <ctype.h>
// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 15000;

// Hash table
node *table[N] = {NULL};

// Global count
int count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TO LOWER CASE
    char copy[strlen(word) + 1];
    strcpy(copy, word);
    char *p = copy;
    for ( ; *p; ++p) *p = tolower(*p);

    int hash_index = hash(copy);
    node *cursor = table[hash_index]; // used to travel down the linked list
    while (cursor != NULL)
    {
        // Check if found
        if (strcasecmp(cursor->word, word) == 0)
        {
            //printf("%s,,,, %s\n", cursor->word, word);
            return true;
        }
        // Update the cursor
        cursor = cursor->next;
    }
    // printf("%s not in dict\n", word);
    return false;
}

// Hashes word to a number
// Obtained from http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash(const char *word)
{
    unsigned int hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
/*
    1) Open the dictionary file
    2) Read word one at a time
    3) Create new node for each word read
    4) Hash the word
    5) Insert the node to hashtable[hash_index]
*/
bool load(const char *dictionary)
{
    // 1) Open the dict. file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    // 2) Read word line by line
    char word[LENGTH + 1];
    int hash_index;
    while (EOF != fscanf(file, "%s", word))
    {
        // 3) Create new node for each word read
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, word);
        n->next = NULL;
        count++;
        // 4) Hash the word
        hash_index = hash(word);
        // 5) Insert the node to hashtable[hash_index]
        // Check for empty list
        if (table[hash_index] == NULL)
        {
            table[hash_index] = n;
        }
        // Else insert new word
        else
        {
            n->next = table[hash_index];
            table[hash_index] = n;
        }
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (count != 0)
    {
        return count;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
