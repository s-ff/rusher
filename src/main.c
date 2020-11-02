#define _GNU_SOURCE
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#include "lib/argparser.h"
#include "lib/utils.h"
#include "lib/tpool.h"

int main(int argc, char **argv) {
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

  const char *wordlist = arguments.wordlist;

  int fd = open(wordlist, O_RDWR);
  assert(fd >= 0);

  struct stat statbuf;
  int err = fstat(fd, &statbuf);
  assert(err >= 0);

  char *ptr = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  assert(ptr != MAP_FAILED);
  close(fd);

  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  FILE *stream = fopen(wordlist, "r");
  assert(stream != NULL);

  while ((nread = getline(&line, &len, stream)) != -1) {
    char url[BUFSIZ] = {0};
    char *host = "http://ensias.um5.ac.ma";
    snprintf(url, BUFSIZ, "%s/", host);

    line[strlen(line) - 1] = '\0';

    /* append new string using length of previously added string */
    snprintf(url + strlen(url), BUFSIZ - strlen(url), "%s", line);

    // url[strlen(url) - 1] = '\0';
#ifdef DEBUG
    printf("%s\n", url);
#endif
    thpool_add_work(thpool, request, (void *) url);
  }

<<<<<<< Updated upstream
=======
  for (unsigned long int i = 0; i < 100000; i++) {
    if (hosts[i]) {
      printf("hosts[%lu] is %s\n", i, hosts[i]);
      thpool_add_work(thpool, request, (void *) hosts[i]);
    }
  }

  for (unsigned long int i = 0; i < 100000; i++) {
    if (hosts[i]) {
      free(hosts[i]);
    }
  }
  free(hosts);

  thpool_wait(thpool);
  thpool_destroy(thpool);

>>>>>>> Stashed changes
  free(line);
  fclose(stream);

  err = munmap(ptr, statbuf.st_size);
  assert(err >= 0);
  
  thpool_wait(thpool);
  thpool_destroy();

  return 0;
}
