#include <stdio.h>
#include <unistd.h>

#include "lib/bruter.h"

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s\n", *argv);
    exit(1);
  }

  /* Check if file exits and user has read permissions */
  if (check_access(argv[2])) return 0;

  /* Create a list of hosts from the input host */
  char** hosts_list = create_hosts_list(argv[1], argv[2]);
  for (unsigned int i = 0; i < count_lines(argv[2]); i++) free(hosts_list[i]);
  free(hosts_list);
  return 0;
}

