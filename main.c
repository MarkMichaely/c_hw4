#include <stdio.h>
#include "trie.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    trie *t;
    t = insertWords();

    if (argc == 1)
    {
        printTrieInLexOrder(t);
    }
    else if (argc > 1 && *(argv[1]) == 'r')
    {
        printTrieReverseLexOrder(t);
    }
    else
    {
        printf("expected 'r', exiting\n");
        exit(1);
    }
    freeTrie(t);
}
