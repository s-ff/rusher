#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>

#include "lib/bruter.h"
#include "lib/tpool.h"

int main(int argc, char** argv) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s\n", *argv);
    exit(1);
  }

  /* Check if file exits and user has read permissions */
  if (check_access(argv[2])) return 0;

  /* Create a list of hosts from the input host */
  char** hosts_list = create_hosts_list(argv[1], argv[2]);

  int num_threads = atoi(argv[3]);
  printf("Making threadpool with %d threads\n", num_threads);
  threadpool thpool = thpool_init(num_threads);

  puts("Adding tasks to threadpool");
  for (int i = 0; i < (int) count_lines(argv[2]); i++) {
    thpool_add_work(thpool, request, (void*) hosts_list[i]);
  };

  thpool_wait(thpool);
  puts("Killing threadpool");
  thpool_destroy(thpool);

  for (unsigned int i = 0; i < count_lines(argv[2]); i++) free(hosts_list[i]);
  free(hosts_list);
  return 0;
}

