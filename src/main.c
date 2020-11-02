#define _GNU_SOURCE
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
#include "lib/tpool.h"
#include "lib/utils.h"

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

  threadpool thpool = thpool_init(arguments.threads);

  while ((nread = getline(&line, &len, stream)) != -1) {
    char url[BUFSIZ] = {0};
    char *host = arguments.args[0];
    snprintf(url, BUFSIZ, "%s/", host);

    // TODO: I have to fix trailing white spaces, such as
    // multiple carriage returns \r.
    // This is a temporarily fix in case of one \r character.
    line[strlen(line) - 1] = '\0';
    size_t s = strlen(line);
    if (s && (line[s-1] == '\r')) line[--s] = 0;

    /* append new string using length of previously added string */
    snprintf(url + strlen(url), BUFSIZ - strlen(url), "%s\n", line);
    
    url[strlen(url) - 1] = '\0';

    thpool_add_work(thpool, request, (void *)url);
    //usleep(100);
  }

  thpool_wait(thpool);
  thpool_destroy(thpool);

  free(line);
  fclose(stream);

  err = munmap(ptr, statbuf.st_size);
  assert(err >= 0);

  return 0;
}
