#define _XOPEN_SOURCE

/* dseq - print sequence of dates to standard output 

seq will go backwards only if increment is given.

*** dseq - like seq unix built in but for dates
**** get_date / gnulib
**** dseq "something parsable by getdate" number_of_days
**** dseq "getdate() string" "get_date() string"
**** --output-format="%Y%m%d"
**** --backward (countdown instead of up)
**** [[http://stackoverflow.com/questions/2655883/bash-shell-date-parsing-start-with-specifc-date-and-loop-through-each-day-in-mon][Stack Overflow Question on Bash Date Loops]]
**** incorporate timers?
**** tai64?
**** hour/minute/second/ms/us instead of just dates

seq 10 -> print 10 days into the future, starting from today   X
seq -10 -> print 10 days into the past, starting from today
seq '2014-07-11' 10 -> print 10 days starting with 2014-07-11  
seq '2014-07-11' '2014-07-17' -> print all days from 7-11-2014 to 7-17-2014, inclusive  X
seq '2014-07-11' 4 '2015-07-11' -> print every fourth day between 2014-07-11 and 2015-07-11, inclusive, starting at 2014
seq '2015-07-11' -4 '2014-07-11' -> print every fourth day between 2015-07-11 and 2014-07-11, inclusive, starting at 2015.

*/

#include <getopt.h>   
#include <stdio.h>
#include <stdbool.h>  /* bool */
#include <stdlib.h>   /* exit */
#include <error.h>    /* error */
#include <time.h>
#include <string.h>

#define PROGRAM_NAME "dseq"
#define AUTHORS proper_name ("Dallas Lynn")

static bool equal_width;
static char const *separator;
static long step;

// TODO: take an input format and an output format.
// TODO: remove equal_width, it's pointless w/ explicit format
static struct option const long_options[] =
{
  { "equal-width", no_argument, NULL, 'w'},
  { "format", required_argument, NULL, 'f'},
  { "separator", required_argument, NULL, 's'},
  { "help", no_argument, NULL, 'h'},
  { "version", no_argument, NULL, 'v'},
  { NULL, 0, NULL, 0}
};


/* Check if FORMAT is a valid <function we use> format string */
__attribute__ ((pure)) static bool
is_valid_date_format (char const *fmt) {
  
  return true;
}


// TODO: should take a locale so we can take on on the comamnd line?
/* Get the default format for the current locale */
static char const *
get_default_format() {
  return "%Y-%m-%d";
}


// TODO: step could be a struct with days, minutes, seconds, etc. fields. that 
// could be added individually.
static void
print_dates(struct tm start, struct tm end, long step, char const *fmt) {
  // TODO: how to size given arbitrary fmt string?
  char buf[11];
  time_t next;

  printf("step is %ld\n", step);
  for(;;) {
    strftime(buf, 11, fmt, &start);

    // TODO: instead of printf should fwrite to stdout from a pre-allocated buffer with
    // the separator added?  seq does this.
    printf("%s%s", buf, separator);
    start.tm_mday += step;
    next = mktime(&start);
    strftime(buf, 11, fmt, localtime(&next));
    
    // TODO: only calculate mktime(&end) once and reuse.
    if(next > mktime(&end))
       break;
  }
}

void
usage(int status) {
  if (status != EXIT_SUCCESS) {
    //  TODO: gettext that and all dat text shit
    fprintf(stderr, "Try '%s --help' for more information.\n", PROGRAM_NAME);
  } else {
    printf("Use me like so motherfucker\n");    
  }

  exit(status);
}

#define ISDIGIT(c) ((unsigned int) (c) - '0' <= 9)

int main(int argc, char **argv) {
  int optc;
  time_t now;
  struct tm start_tm, end_tm;
  memset(&start_tm, 0, sizeof(struct tm));
  memset(&end_tm, 0, sizeof(struct tm));

  char const *format_str = get_default_format();

  equal_width = false;
  separator = "\n";
  step = 1;

  /* TODO 
     - set_program_name
     - setlocale
     - bindtextdomain / textdomain ?
     - atexit (close_stdout)
  */

  while(optind < argc) {
    /* TODO: use ISDIGIT macro instead? */
    if(argv[optind][0] == '-' && isdigit(argv[optind][1])) {
      printf("skiping minus sign\n");
      break;
    }

    optc = getopt_long(argc, argv, "+f:s:w", long_options, NULL);
    if(optc == -1)
      break;
    
    switch(optc) {
    case 'f':
      format_str = optarg;
      break;
    case 's':
      separator = optarg;
      break;
    case 'w':
      equal_width = true;
      break;
    case 'h':
      usage(EXIT_SUCCESS);
      break;
    case 'v':
      usage(EXIT_SUCCESS);
      break;
    default:
      usage(EXIT_FAILURE);
    }
  }
  
  unsigned int n_args = argc - optind;
  
  if(n_args < 1) {
    error(0, 0, "missing operand");
    usage(EXIT_FAILURE);
  }

  if(n_args > 3) {
    // TODO: should quote the argv part?
    error(0, 0, "extra operand %s", argv[optind + 3]);
    usage(EXIT_FAILURE);
  }

  /* if there is one arg it must be an integer and the implicit start is today */
  if(n_args == 1) {
    now = time(NULL);
    start_tm = *localtime(&now);
    end_tm = start_tm;
    // TODO: can tm_mday overflow?
    // TODO: check return of strtoul or precheck to make sure a real int arg instead of eg. 10aaa
    printf("arg: %s\n", argv[optind]);
    end_tm.tm_mday += strtoul(argv[optind], NULL, 10);
  }
  
  /* if there are two args then the first should be a date and the second should
     be a date and we print the days between them */
  if(n_args == 2) {
    printf("printing days between %s and %s\n", argv[optind], argv[optind+1]);
    strptime(argv[optind], format_str, &start_tm);
    strptime(argv[optind+1], format_str, &end_tm);
  }

  /* if there are three args then it's start_date interval end_date */
  if(n_args == 3) {
    printf("printing days between %s and %s in steps of %s\n", argv[optind], argv[optind+2],
           argv[optind+1]);

    strptime(argv[optind], format_str, &start_tm);
    strptime(argv[optind+2], format_str, &end_tm);
    step = atol(argv[optind+1]);
  }

  print_dates(start_tm, end_tm, step, format_str);
  
  exit(EXIT_SUCCESS);
}
