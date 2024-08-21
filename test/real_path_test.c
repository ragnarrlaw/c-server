#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  char *real_path = realpath(argv[1], NULL);
  printf("real path: %s\n", real_path);
  return EXIT_SUCCESS;
}
