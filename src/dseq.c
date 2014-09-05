#define _XOPEN_SOURCE

/* dseq - print sequence of dates to standard output 

seq will go backwards only if increment is given.

*** dseq - like seq unix built in but for dates
**** get_date / gnulib
**** dseq "something parsable by getdate" number_of_days
**** dseq "getdate() string" "get_date() string"
**** --output-format="%Y%m%d"
**** [[http://stackoverflow.com/questions/2655883/bash-shell-date-parsing-start-with-specifc-date-and-loop-through-each-day-in-mon][Stack Overflow Question on Bash Date Loops]]
**** incorporate timers?
**** tai64?
**** hour/minute/second/ms/us instead of just dates

seq 10 -> print 10 days into the future, starting from today   X
seq -10 -> print 10 days into the past, starting from today  X
seq '2014-07-11' 10 -> print 10 days starting with 2014-07-11 
seq '2014-07-11' '2014-07-17' -> print all days from 7-11-2014 to 7-17-2014, inclusive  X
seq '2014-07-11' 4 '2015-07-11' -> print every fourth day between 2014-07-11 and 2015-07-11, inclusive, starting at 2014 X
seq '2015-07-11' -4 '2014-07-11' -> print every fourth day between 2015-07-11 and 2014-07-11, inclusive, starting at 2015.  X

*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <getopt.h>   
#include <stdio.h>
#include <stdbool.h>  /* bool */
#include <stdlib.h>   /* exit */
#include <error.h>    /* error */
#include <time.h>
#include <string.h>
#include <stdint.h>   /* SIZE_MAX */
#include <error.h>    /* errno */

#define PROGRAM_NAME "dseq"
#define AUTHORS proper_name ("Dallas Lynn")
#define ONE_DAY 24 * 60 * 60

//extern int errno;

static char const *separator;
static long step;
static char const terminator[] = "\n";

static struct option const long_options[] =
{
  { "input", required_argument, NULL, 'i'},
  { "output", required_argument, NULL, 'o'},
  { "separator", required_argument, NULL, 's'},
  { "help", no_argument, NULL, 'h'},
  { "version", no_argument, NULL, 'v'},
  { NULL, 0, NULL, 0}
};


/* Check if FORMAT is a valid <function we use> format string */
static bool __attribute__ ((pure)) 
is_valid_date_format (char const *fmt) {
  
  return true;
}


static size_t __attribute__ ((pure)) 
format_size (char const *fmt, struct tm *t) {
  return strftime(NULL, SIZE_MAX, fmt, t) + 1;
}


// TODO: test if this needs to be locale aware or strftime does it for me?
static char const * __attribute__ ((const))
get_default_format() {
  return "%Y-%m-%d";
}


// TODO: step could be a struct with days, minutes, seconds, etc. fields. that 
// could be added individually.
static void
print_dates(struct tm start, struct tm end, long step, char const *fmt) {
  size_t buf_size = format_size(fmt, &start);
  char buf[buf_size];
  time_t next;
  time_t last = mktime(&end);

  for(;;) {
    next = mktime(&start);

    // TODO: make sure works with step '0' 
    if((step > 0 && next > last) || (step < 0 && next < last))
       break;
  
    strftime(buf, sizeof(buf), fmt, &start);

    // TODO: instead of printf should fwrite to stdout from a pre-allocated buffer with
    // the separator added?  seq does this.
    printf("%s%s", buf, separator);
    start.tm_mday += step;
  }

  printf("%s", terminator);
}


void __attribute__ ((noreturn))
usage(int status) { 
  if (status != EXIT_SUCCESS) {
    //  TODO: gettext that and all dat text shit
    fprintf(stderr, "Try '%s --help' for more information.\n", PROGRAM_NAME);
  } else {
    printf("Use me like so motherfucker\n");    
  }

  exit(status);
}


int main(int argc, char **argv) {
  int optc;                 
  struct tm start_tm, end_tm;
  memset(&start_tm, 0, sizeof(struct tm));
  memset(&end_tm, 0, sizeof(struct tm));

  char const *format_str = get_default_format();
  char const *output_format = get_default_format();
  separator = "\n";
  step = 1;

  /* TODO 
     - set_program_name
     - setlocale
     - bindtextdomain / textdomain ?
     - atexit (close_stdout) ?
  */

  while(optind < argc) {
    if(argv[optind][0] == '-' && isdigit(argv[optind][1]))
      break;

    optc = getopt_long(argc, argv, "+i:o:s", long_options, NULL);
    if(optc == -1)
      break;
    
    switch(optc) {
    case 'i':
      format_str = optarg;
      break;
    case 'o':
      output_format = optarg;
      break;
    case 's':
      separator = optarg;
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
    // TODO: check return of strtol or precheck to make sure a real int arg instead of eg. 10aaa
    // or xstrtol from gnulib
    long offset = strtol(argv[optind], NULL, 10);
    time_t now = time(NULL);
    step = offset > 0 ? 1 : -1;

    // TODO: overflow
    time_t then = now + (offset * ONE_DAY) - (ONE_DAY * step);
    localtime_r(&now, &start_tm);
    localtime_r(&then, &end_tm);
  }
  
  /* if there are two args then the first should be a date and the second should
     be a date and we print the days between them */
  if(n_args == 2) {
    /* printf("printing days between %s and %s\n", argv[optind], argv[optind+1]); */
    // TODO: check return of strptime.
    strptime(argv[optind], format_str, &start_tm);
    strptime(argv[optind+1], format_str, &end_tm);
    if(mktime(&start_tm) > mktime(&end_tm))
      step = -1;
  }

  /* if there are three args then it's start_date interval end_date */
  if(n_args == 3) {
    /* printf("printing days between %s and %s in steps of %s\n", argv[optind], argv[optind+2], */
    /*        argv[optind+1]); */

    // TODO: check return of strptime.
    strptime(argv[optind], format_str, &start_tm);
    strptime(argv[optind+2], format_str, &end_tm);
    step = atol(argv[optind+1]);
  }

  print_dates(start_tm, end_tm, step, output_format);
  
  exit(EXIT_SUCCESS);
}
