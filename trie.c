#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "trie.h"


 /*
  * Function:  mallocError
  * --------------------
  * function to print error if there is a problem with memory allocation
  * function exits program
  *
  */
void mallocError(void) {
  printf("Memory error has accord, exiting now\n");
  exit(1);
}

/*
 * Function:  newNode
 * --------------------
 *  function creates a new node, allocate memory and inits it.
 *  after creation return new node
 *  c: character to put in node
 */
node * newNode(char c) {
  node * n;
  if (!(n = (node*) malloc(sizeof(node))))
    mallocError();

  int i;
  n -> letter = c;
  n -> count = 0;
  n -> endOfWord = FALSE;
  for (i = 0; i < NUM_LETTERS; i++) {
    (n -> children)[i] = NULL;
  }
  return n;
}
/*
 * Function:  hasChild
 * --------------------
 *  function loops over the children nodes of node and checkes wether they exist
 *  if any child exist return boolean value of TRUE and FALSE if none
 *  n: node to work on
 */
int hasChild(node *n){
  for(size_t i = 0; i < NUM_LETTERS; i++){
    if (n->children[i] != NULL) return TRUE;
  }
  return FALSE;
}
/*
 * Function:  freeNode
 * --------------------
 *  function loops over the children nodes of node and frees memory that they take
 *  if any children node has children of it's own, recursuivly calls freeNode till there are none
 *  n: node to work on
 */
void freeNode(node *n) {
  int i;
  if (n == NULL)
    return;

  if ((hasChild(n) == FALSE)) {
    free(n);
  } else {
    for (i = 0; i < NUM_LETTERS; i++) {
      freeNode((n -> children)[i]);
    }
  }
  return;
}

/*
 * Function:  newTrie
 * --------------------
 *  function creates a new trie, allocate memory and inits it.
 *  after creation return new trie
 */
trie *newTrie() {
  trie *t;
  if (!(t = (trie*) malloc(sizeof(trie))))
    mallocError();

  for (int i = 0; i < NUM_LETTERS; i++) {
    t -> children[i] = NULL;
  }
  t -> curr = NULL;
  t -> MaxWordLen = 0;
  return t;
}

/*
 * Function:  freeTrie
 * --------------------
 *  function loops over the children nodes of trie and frees memory that they take
 *  if any children node has children of it's own, recursuivly calls freeNode till there are none
 *  t: trie to work on
 */
void freeTrie(trie *t) {

  if (t == NULL) return;

  for (int i = 0; i < NUM_LETTERS; i++) {
    freeNode(t -> children[i]);
  }
  free(t);
}
/*
 * Function:  isEmpty
 * --------------------
 *  function loops over the children nodes of node and checkes wether they exist
 *  if any child exist return boolean value of FALSE and TRUE if none
 *  t: trie to work on
 */
boolean isEmpty(trie *t){
  for(size_t i = 0; i < NUM_LETTERS; i++){
    if (t->children[i] != NULL) return FALSE;
  }
  return TRUE;
}
/*
 * Function:  insertChar
 * --------------------
 *  function puts one character in trie.
 *  function checks wether character is alphabetical and ignores it if not.
 *  function then lowers character to lowercase if it's uppercase and puts it in the right place in trie.
 *  t: trie to work on
 *  c: charater to put in trie
 */
int insertChar(trie *t, int c) {
  int index;
  int wordLen = 0;
  if (!isalpha(c)) {
    if (t -> curr != NULL){
    t -> curr -> count++;
    t -> curr -> endOfWord = TRUE;
    t -> curr = NULL;
  }
    return wordLen;
  }
  wordLen++;
  c = tolower(c);
  index = c - 'a';
  if (t -> curr == NULL) {
    if (t -> children[index] == NULL)
      t -> children[index] = newNode(c);
    t -> curr = t -> children[index];
  } else {
    if (t -> curr -> children[index] == NULL)
      t -> curr -> children[index] = newNode(c);
    t -> curr = t -> curr -> children[index];
  }
  return wordLen;
}

/*
 * Function:  mallocErrorWord
 * --------------------
 * function to print error if there is a problem with memory allocation with the trie word
 * function exits program
 *
 */
void mallocErrorWord(trie *t) {
  free(t -> word);
  if (!(t -> word = (char*) malloc(1 + sizeof(char)*(t -> MaxWordLen))))
    mallocError();
}

/*
 * Function:  insertWords
 * --------------------
 *  function creates new tire and puts words in trie.
 *  function continusly puts characters in trie while not reached End Of File
 *  functions calculate the amount of memory to allocate to word in trie and calls on
 *  function to allcate memory.
 *  function returns trie filled with words.
 */
trie *insertWords() {
  trie *t;
  int c;
  int wordLen;
  t = newTrie();
  while ((c = getchar()) != EOF) {
    wordLen = insertChar(t, c);
    if (wordLen > t -> MaxWordLen)
      t -> MaxWordLen = wordLen;
  }
  mallocErrorWord(t);
  return t;
}

/*
 * Function:  printWordsInLexOrder
 * --------------------
 *  function gets trie and prints it's words in lexicographic order
 *  t: trie to work on
 */
void printWordsInLexOrder(trie *t) {
  static int index = 0;
  int i;
  node * curr;
  t -> word[index++] = t -> curr -> letter;
  if (t -> curr -> endOfWord) {
    t -> word[index] = '\0';
    printf("%s %ld\n", t -> word, t -> curr -> count);
  }
  if (hasChild(t -> curr)) {
    for (i = 0; i < NUM_LETTERS; ++i) {
      if (t -> curr -> children[i] == NULL)
        continue;
      curr = t -> curr; /* remember */
      t -> curr = t -> curr -> children[i];
      printWordsInLexOrder(t);
      t -> curr = curr; /* recall */
    }
  } else {
    index--;
    return;
  }
  index--;
}

/*
 * Function:  printWordsReverseLexOrder
 * --------------------
 *  function gets trie and prints it's words in reverse lexicographic order
 *  t: trie to work on
 */
void printWordsReverseLexOrder(trie *t) {
  static int index = 0;
  int i;
  node *curr;
  t -> word[index++] = t -> curr -> letter;
  if (hasChild(t -> curr)) {
    for (i = NUM_LETTERS - 1; i >= 0; --i) {
      if (t -> curr -> children[i] == NULL)
        continue;
      curr = t -> curr;
      t -> curr = t -> curr -> children[i];
      printWordsReverseLexOrder(t);
      t -> curr = curr;
    }
  }
  if (t -> curr -> endOfWord) {
    t -> word[index] = '\0';
    printf("%s %ld\n", t -> word, t -> curr -> count);
  }
  index--;
}

/*
 * Function:  printTrieInLexOrder
 * --------------------
 *  function gets trie and checks wether it's not null or empty
 *  and if not, calls on printWordsInLexOrder function to print the words on each of trie's children
 *  t: trie to work on
 */
void printTrieInLexOrder(trie *t) {
  int i;
  if (t == NULL || isEmpty(t))  return;
  for (i = 0; i < NUM_LETTERS; ++i) {
    if (t -> children[i] == NULL)
      continue;
    t -> curr = t -> children[i];
    printWordsInLexOrder(t);
  }
}

/*
 * Function:  printWordsReverseLexOrder
 * --------------------
 *  function gets trie and checks wether it's not null or empty
 *  and if not, calls on printWordsReverseLexOrder function to print the words on each of trie's children
 *  t: trie to work on
 */
void printTrieReverseLexOrder(trie *t) {
  int i;
  if (t == NULL || isEmpty(t))  return;
  for (i = NUM_LETTERS - 1; i >= 0; --i) {
    if (t -> children[i] == NULL)
    continue;
    t -> curr = t -> children[i];
    printWordsReverseLexOrder(t);
  }
}
