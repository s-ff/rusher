#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>

#include "lib/argparser.h"
#include "lib/bruter.h"
#include "lib/tpool.h"

int main(int argc, char** argv) {

  struct arguments arguments;

  /* Default values. */
  arguments.silent = 0;
  arguments.verbose = 0;
  arguments.output_file = "-";
  arguments.wordlist = "Wordlist";
  arguments.followredirects = 0;
  arguments.threads = 10;
  arguments.statuscodes = "200,204,301,302,307,401,403";
  arguments.useragent =
      "Mozilla/5.0 (X11; Linux x86_64; rv:10.0) Gecko/20100101 Firefox/10.0";

  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  /* Check if file exits and user has read permissions */
  if (check_access(arguments.wordlist)) return 0;

  /* Create a list of hosts from the input host */
  char** hosts_list = create_hosts_list(arguments.args[0], arguments.wordlist);

  int num_threads = arguments.threads;
  printf("Making threadpool with %d threads\n", num_threads);
  threadpool thpool = thpool_init(num_threads);

  puts("Adding tasks to threadpool");
  for (int i = 0; i < (int) count_lines(arguments.wordlist); i++) {
    thpool_add_work(thpool, request, (void*)hosts_list[i]);
  };

  thpool_wait(thpool);
  puts("Killing threadpool");
  thpool_destroy(thpool);

  for (unsigned int i = 0; i < count_lines(arguments.wordlist); i++) free(hosts_list[i]);
  free(hosts_list);
  return 0;
}
