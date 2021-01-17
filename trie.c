#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "trie.h"


//print an error if malloc has a problem.
void mallocError(void) {
  printf("Memory error has accord, exiting now\n");
  exit(1);
}

//Crates a new node, and init all his parameters.
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

int hasChild(node *n){
  for(size_t i = 0; i < NUM_LETTERS; i++){
    if (n->children[i] != NULL) return TRUE;
  }
  return FALSE;
}

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

//This method creates a new Trie, with reset parameters.
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

//This method clears the memory of a Trie.
void freeTrie(trie *t) {

  if (t == NULL) return;

  for (int i = 0; i < NUM_LETTERS; i++) {
    freeNode(t -> children[i]);
  }
  free(t);
}

boolean isEmpty(trie *t){
  for(size_t i = 0; i < NUM_LETTERS; i++){
    if (t->children[i] != NULL) return FALSE;
  }
  return TRUE;
}
//Insert one char to the Trie.
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

//print an error if malloc has a problem.
void mallocErrorWord(trie *t) {
  free(t -> word);
  if (!(t -> word = (char*) malloc(1 + sizeof(char)*(t -> MaxWordLen))))
    mallocError();
}

//Insert some text to the Trie.
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

//Prints the words of a node.
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

//Print Tthe word in reverse order.
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

//Prints the whole Trie.
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

//Prints the whole Trie, in reverse order.
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
