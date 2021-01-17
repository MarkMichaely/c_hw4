#define NUM_LETTERS ((int)26)

#ifndef Node
#define Node


typedef enum {FALSE=0, TRUE=1} boolean;

typedef struct node{

   char letter;
   long unsigned int count;
   struct node* children[NUM_LETTERS];
   boolean endOfWord;
   boolean hasChild;

} node;
#endif

#ifndef Trie
#define Trie
typedef struct trie{

   node* children[NUM_LETTERS];
   node* curr;
   char* word;
   int MaxWordLen;
   boolean empty;

} trie;
#endif


void freeTrie(trie* t);
trie* insertWords();
void printTrieInLexOrder(trie* root);
void printTrieReverseLexOrder(trie* root);
void mallocError(void);
void mallocErrorWord(trie* root);
node* newNode(char ch);
void freeNode(node* n);
trie* newTrie();
void closeWord(trie* root);
boolean isEmpty(trie* root);
int insertChar(trie* root, int c);
void printWordsInLexOrder(trie* root);
void printWordsReverseLexOrder(trie* root);
