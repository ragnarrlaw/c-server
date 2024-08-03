#ifndef _DICTIONARY_H
#define _DICTIONARY_H

#include <stdbool.h>

#define HASH_TABLE_SIZE 100

typedef struct KVEntry {
  char *key;
  char *value;
  struct KVEntry *next; // collision
} KVEntry;

typedef struct Dictionary {
  KVEntry *buckets[HASH_TABLE_SIZE];
} Dictionary;

unsigned int hash(const char *key);

Dictionary* init_dictionary();

bool insert_kv(Dictionary *d, const char *key, const char *value);

const char *search_dict(Dictionary *d, char *key);

void repr(Dictionary *d);

void free_dict(Dictionary *d);

#endif // !_DICTIONARY_H
