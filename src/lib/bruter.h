#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ASSUME_HTTP "http://"

int check_access(const char *pathname) {
  if (access(pathname, F_OK)) {
    fprintf(stderr, "Error: %s does not exist.\n", pathname);
    return 1;
  }

  if (access(pathname, R_OK)) {
    fprintf(stderr, "Error: %s: Read access denied\n", pathname);
    return 1;
  }

  return 0;
}

unsigned int count_lines(const char *pathname) {
  FILE *fp = fopen(pathname, "r");
  int count = 0;
  char c = fgetc(fp);

  while (c != EOF) {
    if (c == '\n') count++;
    c = fgetc(fp);
  }
  fclose(fp);
  return count;
}

char *format_host(const char *host, const char *path) {
  regex_t regex;

  if (regcomp(&regex, "^https?://", REG_EXTENDED)) {
    exit(1);
  }
  char *url = (char *)malloc(512 * sizeof(char));

  if (regexec(&regex, host, 0, NULL, 0)) {
    sprintf(url, ASSUME_HTTP);
  }

  sprintf(url + strlen(url), "%s/", host);
  sprintf(url + strlen(url), "%s", path);

  return url;
}

char **create_hosts_list(const char *host, const char *pathname) {
  int count = count_lines(pathname);
  char **paths = (char **)malloc(count * sizeof(char *));

  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  FILE *fp = fopen(pathname, "r");  // Don't forget to catch errors.
  int i = 0;
  while ((nread = getline(&line, &len, fp)) != -1) {
    paths[i] = malloc(BUFSIZ * sizeof(char));
    sprintf(paths[i], "%s", line);
    i++;
  }
  fclose(fp);
#ifdef DEBUG
  for (i = 0; i < count; ++i) {
    printf("%s", format_host(host, paths[i]));
  }
#endif

  // According to the man pages, a user has to explicity free the line variable.
  free(line);

  return paths;
}
