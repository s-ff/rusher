#include <argp.h>
#include <stdlib.h>

/* Program documentation */
static char doc[] =
    "Rusher - Brute force directories and file names on web applications.";

/* A description of the arguments we accept. */
static char argc_doc[] = "URL";

/* The options we understand. */
static struct argp_option options[] = {
    {"help", 'h', 0, 0, "Print help for rusher."},
    {"wordlist", 'w',"WORDLIST", 0, "Path to the wordlist"},
    {"threads", 't', "THREADS", 0, "Number of concurrent threads (default 10)"},
    {"followredirect", 'r', 0, 0, "Follow redirects"},
    {"statuscodes", 's', "STATUS_CODES",
     0, "Status codes to report. (default \"200,204,301,302,307,401,403\""},
    {"output", 'o', "OUTPUT", 0, "Output result to a file. (default stdout)"},
    {"useragent", 'a', "USER_AGENT", 0, "Set the User-Agent paramater"},
    {0}};

struct arguments {
  char *args[1];
  char *wordlist, *statuscodes, *output, *useragent;
  int threads;
};

/* Parse a single option */
static error_t
parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;

  switch (key) {
    case 'h':
      arguments->wordlist = arg;
      break;
    case 't':
      arguments->threads = atoi(arg);
      break;
    case 's':
      arguments->statuscodes = arg;
      break;
    case 'o':
      arguments->statuscodes = arg;
      break;
    case 'a':
      arguments->useragent = arg;
      break;
    case ARGP_KEY_END:
      if (state->arg_num < 2) argp_usage(state);
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

/* Our argp parser */
static struct argp argp = {options, parse_opt, argc_doc, doc};

int main(int argc, char **argv) {
  struct arguments arguments;

  arguments.wordlist = "~/Documents/directory-list-2.3-small.txt";
  arguments.threads = 10;
  arguments.output = "stdout";
  arguments.useragent =
      "Mozilla/5.0 (X11; Linux x86_64; rv:10.0) Gecko/20100101 Firefox/10.0";

  /* Parse our arguments */
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  return 0;
}

