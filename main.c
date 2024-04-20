#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie structure
struct Trie {
    struct Trie *children[ALPHABET_SIZE];
    int isEndOfWord;
    int count;
};

// Initializes a trie structure
struct Trie *createTrie() {
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    if (pTrie) {
        pTrie->isEndOfWord = 0;
        pTrie->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
            pTrie->children[i] = NULL;
    }
    return pTrie;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word) {
    struct Trie *crawl = pTrie;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!crawl->children[index])
            crawl->children[index] = createTrie();
        crawl = crawl->children[index];
    }
    crawl->isEndOfWord = 1;
    crawl->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurances(struct Trie *pTrie, char *word) {
    struct Trie *crawl = pTrie;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!crawl->children[index])
            return 0;
        crawl = crawl->children[index];
    }
    return (crawl != NULL && crawl->isEndOfWord) ? crawl->count : 0;
}

// Deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie) {
    if (pTrie) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (pTrie->children[i])
                deallocateTrie(pTrie->children[i]);
        }
        free(pTrie);
    }
    return NULL;
}

// This function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file.\n");
        exit(1);
    }

    int count = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        pInWords[count] = strdup(buffer);
        pInWords[count][strlen(pInWords[count]) - 1] = '\0'; 
        count++;
    }

    fclose(file);
    return count;
}

int main(void)
{
  char *inWords[256];

  //read the number of the words in the dictionary
  int numWords = readDictionary("dictionary.txt", inWords);
  for (int i=0;i<numWords;++i)
  {
    printf("%s\n",inWords[i]);
  }

  struct Trie *pTrie = createTrie();
  for (int i=0;i<numWords;i++)
  {
    insert(pTrie, inWords[i]);
  }
  // parse lineby line, and insert each word to the trie data structure
  char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
  for (int i=0;i<5;i++)
  {
    printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
  }
  pTrie = deallocateTrie(pTrie);
  if (pTrie != NULL)
    printf("There is an error in this program\n");
  return 0;
}
