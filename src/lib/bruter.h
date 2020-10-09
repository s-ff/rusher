#ifndef BRUTER_H
#define BRUTER_H

#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include <pthread.h>

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
  url[strlen(url)-1] = '\0';

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
  for (i = 0; i < count; ++i) {
#ifdef DEBUG
    printf("%s", format_host(host, paths[i]));
#endif
    paths[i] = format_host(host, paths[i]);
  }

  // According to the man pages, a user has to explicity free the line variable.
  free(line);

  return paths;
}

void request(void *_url) {
  pthread_t self;
  self = pthread_self();

  char *url = (char *)_url;
  CURL *curl;
  CURLcode status;
  long code;

  curl = curl_easy_init();
  if (!curl) return;
  curl_easy_setopt(curl, CURLOPT_URL, url);
  // curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
  curl_easy_setopt(curl, CURLOPT_POSTREDIR, CURL_REDIR_POST_ALL);
  //curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, atol(argv[3]));

  status = curl_easy_perform(curl);
  if (status != 0) {
    fprintf(stderr, "error: unable to request data from %s:\n", url);
    fprintf(stderr, "%s\n", curl_easy_strerror(status));
    return;
  }

  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);

  fprintf(stdout, "%s server responded with code %ld\n", url, code);
  curl_easy_cleanup(curl);
  curl_global_cleanup();

	return;
}

#endif
