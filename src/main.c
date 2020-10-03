#include <stdio.h>

#include "lib/bruter.h"

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s\n", *argv);
    exit(1);
  }

  /* Populate host */
  if (check_access(argv[2])) return 0;

  create_hosts_list(argv[1], argv[2]);
  return 0;
}

