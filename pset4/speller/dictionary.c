// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 2600

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];
//Represent size of hash table
int oSize = 0;

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
// djb2's hasing algorith,
unsigned int hash(const char *word)
{
    unsigned long hashC = 5381;

    for (int i = 0; i < strlen(word); i++)
    {
        hashC = ((hashC << 5) + hashC) + tolower(word[i]);
    }

    return hashC % N;
    //return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // Get word and create node
        node *currNode = malloc(sizeof(node));
        if (!currNode)
        {

            unload();
            return false;
        }

        // Node Word
        strcpy(currNode->word, word);
        currNode->next = NULL;

        //Put into hashtable
        int index = hash(currNode->word);
        if (hashtable[index] != NULL)
        {
            currNode->next = hashtable[index];
        }
        hashtable[index] = currNode;
        oSize++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return oSize;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{

    // Get hash value of word
    int index  = hash(word);

    // Lope over linked list at hashtable[index]
    for (node *currNode = hashtable[index]; currNode != NULL; currNode = currNode->next)
    {
        // Check w/o checking case
        if (strcasecmp(currNode->word, word) == 0)
        {

            return true;
        }
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Loop over hashtable
    for (int i = 0; i < N; i++)
    {
        // Check if linked list exists at i'th pos^n
        if (hashtable[i] != NULL)
        {
            node *currNode = hashtable[i], *tmp;
            while (currNode != NULL)
            {
                tmp = currNode;
                currNode = currNode->next;
                free(tmp);
            }
        }
    }
    return true;
}