#include "dictionary.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Hash function for hashing the keys
*/
unsigned int hash(const char *key) {
  unsigned int hash = 0;
  while (*key) hash = (hash << 5) + *key++;
  return hash % HASH_TABLE_SIZE;
}

/**
 * Initialize a Dictionary with HASH_TABLE_SIZE number of
 * hash key capacity
*/
Dictionary* init_dictionary() {
  Dictionary *d = (Dictionary *)malloc(sizeof(Dictionary));
  for (int i = 0; i < HASH_TABLE_SIZE; i++) d->buckets[i] = NULL;
  return d;
}

/**
 * Insert a given key & value pair to the dictionary. On failure returns false
*/
bool insert_kv(Dictionary *d, const char *key, const char *value) {
  unsigned int index = hash(key);
  KVEntry *kv_pair = malloc(sizeof(KVEntry));
  if (!kv_pair) {
    return false;
  }
  kv_pair->key = strdup(key);
  kv_pair->value = strdup(value);
  kv_pair->next = d->buckets[index];
  d->buckets[index] = kv_pair;
  return true;
}

/**
 * Search for a give key in dictionary
 * If the key is not found then NULL is returned,
 * otherwise the key is returned
*/
const char *search_dict(Dictionary *d, char *key) {
  unsigned int index = hash(key);
  KVEntry *entry = d->buckets[index];
  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      return entry->value;
    }
  }
  return NULL;
}

void repr(Dictionary *d) {
  printf("Stored Key Value data:\n");
  printf("------------------------------------------------------------------\n");
  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    KVEntry *entry = d->buckets[i];
    while (entry != NULL) {
      printf("\t %s : %s\n", entry->key, entry->value);
      entry = entry->next;
    }
  }
  printf("------------------------------------------------------------------\n");
}

/**
 * This function assumes that the dictionary taken is
 * initialized using the init_dictionary function and
 * tries to deallocate the dictionary itself.
*/
void free_dict(Dictionary *d) {
  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    KVEntry *entry = d->buckets[i];
    while(entry != NULL) {
      KVEntry *previous = entry;
      entry = entry->next;
      free(previous->key);
      free(previous->value);
      free(previous);
    }
  }
  free(d);
  d = NULL;
}

