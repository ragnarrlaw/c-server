#include "../kv_store/dictionary.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  Dictionary *d = init_dictionary();
  insert_kv(d, "content-type", "application/json");
  insert_kv(d, "host", "10.0.0.1");
  insert_kv(d, "method", "GET");

  printf("Searching for a key-value pair: %s\n",
         search_dict(d, "content-type"));

  repr(d);

  free_dict(d);
  return EXIT_SUCCESS;
}
