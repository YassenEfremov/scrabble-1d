#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

// ============================================================================================= //


struct node_t {
	struct node_t *children[26];
	bool isEndOfWord;
};


struct trie_t {
	int size;
	struct node_t *root;
};


// ============================================================================================= //
struct node_t *getNode(void)
{
    struct node_t *pNode = NULL;
    pNode = (struct node_t *)malloc(sizeof(struct node_t));
    if (pNode)
    {
        int i;
        pNode->isEndOfWord = false;
        for (i = 0; i < 26; i++)
            pNode->children[i] = NULL;
    }
    return pNode;
}

extern void trie_init(struct trie_t *trie) {
	trie->size = 0;
	trie->root = NULL;
	
}
bool search(struct node_t *root, const char *key)
{
    int level;
    int length = strlen(key);
    int index;
    struct node_t *pCrawl = root;

    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);

        if (!pCrawl->children[index])
            return false;

        pCrawl = pCrawl->children[index];
    }

    return (pCrawl != NULL && pCrawl->isEndOfWord);
}

extern void trie_insert(struct trie_t *trie, const char *key) {
    int level;
    int length = strlen(key);
    int index;

    struct node_t *pCrawl = trie->root;

    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

        pCrawl = pCrawl->children[index];
    }

    pCrawl->isEndOfWord = true;
}

extern void trie_delete(struct trie_t *trie) {

	// frees the memory allocated for the tree
}
