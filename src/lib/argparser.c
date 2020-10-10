#include <stdlib.h>
#include <argp.h>

const char *argp_program_version =
  "rusher 0.1.0";
const char *argp_program_bug_address =
  "<soufiane.fariss@um5s.net.ma>";

/* Program documentation. */
static char doc[] =
  "Rusher - brute force directories and file names on web applications.";

/* A description of the arguments we accept. */
static char args_doc[] = "URL";

/* The options we understand. */
static struct argp_option options[] = {
  {"verbose",  'v', 0,      0,  "Produce verbose output" },
  {"quiet",    'q', 0,      0,  "Don't produce any output" },
  {"output",   'o', "FILE", 0, "Output to FILE instead of standard output" },
  {"wordlist", 'w', "WORDLIST", 0, "Path to wordlist" },
  {"followredirects",   'r', 0, 0, "Follow redirects (default no)" },
  {"threads", 't', "THREADS", 0, "Number of concurrent threads (default 10)" },
  {"statuscodes",   's', "CODES", 0, "Status codes to scan for (default 200,204,301,302,307,401,403)" },
  {"useragent",   'a', "USER_AGENT", 0, "Set the User-Agent parameter" },

  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
  char *args[1];                /* arg1 */
  int silent, verbose, followredirects, threads;
  char *output_file, *wordlist, *statuscodes, *useragent;
};

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'q':
      arguments->silent = 1;
      break;
    case 'v':
      arguments->verbose = 1;
      break;
    case 'o':
      arguments->output_file = arg;
      break;

    case 'w':
      arguments->wordlist = arg;
      break;

    case 'r':
      arguments->followredirects = 1;
      break;

    case 't':
      arguments->threads = atoi(arg);
      break;

    case 's':
      arguments->statuscodes = arg;
      break;

    case 'a':
      arguments->useragent = arg;
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 1)
        /* Too many arguments. */
        argp_usage (state);

      arguments->args[state->arg_num] = arg;

      break;

    case ARGP_KEY_END:
      if (state->arg_num < 1)
        /* Not enough arguments. */
        argp_usage (state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };

int
main (int argc, char **argv)
{
  struct arguments arguments;

  /* Default values. */
  arguments.silent = 0;
  arguments.verbose = 0;
  arguments.output_file = "-";
  arguments.wordlist = "Wordlist";
  arguments.followredirects = 0;
  arguments.threads = 10;
  arguments.statuscodes = "200,204,301,302,307,401,403";
  arguments.useragent = "Mozilla/5.0 (X11; Linux x86_64; rv:10.0) Gecko/20100101 Firefox/10.0";


  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  printf ("URL = %s\nOUTPUT_FILE = %s\n"
          "VERBOSE = %s\nSILENT = %s\n"
	  "WORDLIST = %s\nREDIRECTS = %d\n"
          "THREADS = %d\nSTATUS_CODES = %s\n"
	  "USER_AGENT = %s\n",

	  arguments.args[0],
          arguments.output_file,
          arguments.verbose ? "yes" : "no",
          arguments.silent ? "yes" : "no",
	  arguments.wordlist,	
	  arguments.followredirects,
	  arguments.threads,
	  arguments.statuscodes,
	  arguments.useragent

	  );

  exit (0);
}
