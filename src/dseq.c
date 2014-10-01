#define _XOPEN_SOURCE

/* dseq -- print a sequence of dates to standard out.  Like seq, but for dates.

   Copyright 2014 Dallas Lynn

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
#include <errno.h>    /* errno */
#include <limits.h>
#include <unistd.h>   /* stdout */
#include <locale.h>   /* setlocale */

#include "xstrtol.h"  /* xstrtol */
#include "progname.h" /* set_program_name */
#include "quote.h"    /* quote */
#include "closeout.h" /* close_stdout */
#include "gettext.h"

#define PROGRAM_NAME "dseq"
#define AUTHORS proper_name ("Dallas Lynn")
#define ONE_DAY 24 * 60 * 60

#define _(msgid) gettext(msgid)
#define N_(msgid) msgid

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


void
usage(int status) { 
  if (status != EXIT_SUCCESS) {
    fprintf(stderr, _("Try '%s --help' for more information.\n"), PROGRAM_NAME);
  } else {
    // TODO: write the actual usage
    printf("Use me like so motherfucker\n");    
  }

  exit(status);
}


static size_t _GL_ATTRIBUTE_PURE
format_size (char const *fmt, struct tm *t) {
  return strftime(NULL, SIZE_MAX, fmt, t) + 1;
}


static char const * _GL_ATTRIBUTE_CONST
get_default_format() {
  return "%Y-%m-%d";
}


char *
xstrptime(const char *s, const char *format, struct tm *tm) {
  char *end = strptime(s, format, tm);
  if(end == NULL || *end != '\0') {
    error(0, 0, "bad date format: %s\n", quote(s));
    usage(EXIT_FAILURE);
  }

  return end;
}


// TODO: step could be a struct with days, minutes, seconds, etc. fields. that 
// could be added individually.
static void
print_dates(struct tm start, struct tm end, long step, char const *fmt) {
  
  if(step == 0) {
    error(0, 0, "step can not be zero\n");
    usage(EXIT_FAILURE);
  }

  size_t buf_size = format_size(fmt, &start);
  char buf[buf_size];
  time_t first = mktime(&start);
  time_t last = mktime(&end);

  /* conceptually counting down with a positive step or counting up with a negative step
   * makes no sense, attempt to do what one means by inverting the signs in those cases.
   */
  if(((first > last) && (step > 0)) || ((first < last) && (step < 0)))
    step = -step;

  bool out_of_range = (step > 0 ? first > last : first < last);

  if(! out_of_range) {
    time_t next = first;

    for(;;) {
      strftime(buf, sizeof(buf), fmt, &start);
      fputs(buf, stdout);

      start.tm_mday += step;
      next = mktime(&start);

      out_of_range = (step > 0 ? next > last : next < last);

      if(out_of_range)
        break;

      fputs(separator, stdout);
    }
  }

  fputs(terminator, stdout);
}


int main(int argc, char **argv) {
  int optc;
  long offset;
  struct tm start_tm, end_tm;
  memset(&start_tm, 0, sizeof(struct tm));
  memset(&end_tm, 0, sizeof(struct tm));

  char const *format_str = get_default_format();
  char const *output_format = get_default_format();
  separator = "\n";
  step = 1;

  set_program_name(argv[0]);
  setlocale(LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  atexit(close_stdout);

  while(optind < argc) {
    if(argv[optind][0] == '-' && isdigit(argv[optind][1]))
      break;

    optc = getopt_long(argc, argv, "+i:o:s:", long_options, NULL);
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
    error(0, 0, _("missing operand"));
    usage(EXIT_FAILURE);
  }

  if(n_args > 3) {
    error(0, 0, _("extra operand %s"), quote(argv[optind + 3]));
    usage(EXIT_FAILURE);
  }

  /* if there is one arg it must be an integer and the implicit start is today */
  if(n_args == 1) {
    if(xstrtol(argv[optind], NULL, 10, &offset, "") != LONGINT_OK) {
      error(0, 0, _("invalid integer argument: %s\n"), quote(argv[optind]));
      usage(EXIT_FAILURE);
    }

    time_t now = time(NULL);
    step = offset > 0 ? 1 : -1;

    // TODO: overflow
    time_t then = now + (offset * ONE_DAY) - (ONE_DAY * step);
    localtime_r(&now, &start_tm);
    localtime_r(&then, &end_tm);
  }

  /* if there are two args then the first should be a date and the second should 
   * be a date or an int.  If it's a date we print the days between them, if it's an 
   * int print that many days from the start date (forward or backward). 
   */
  if(n_args == 2) {
    xstrptime(argv[optind], format_str, &start_tm);

    if(xstrtol(argv[optind+1], NULL, 10, &offset, "") != LONGINT_OK) {
      /* not a valid integer, check for valid date */      
      xstrptime(argv[optind+1], format_str, &end_tm);
    /* second arg is an offset */
    } else {
      end_tm = start_tm;

      if(offset == 0)
        exit(EXIT_SUCCESS);

      end_tm.tm_mday += (offset > 0) ? offset -1 : offset;
      time_t then = mktime(&end_tm);
      gmtime_r(&then, &end_tm);
    }

    if(mktime(&start_tm) > mktime(&end_tm))
      step = -1;
  }

  /* if there are three args then it's start_date interval end_date */
  if(n_args == 3) {
    xstrptime(argv[optind], format_str, &start_tm); 
    xstrptime(argv[optind+2], format_str, &end_tm);
    if(xstrtol(argv[optind+1], NULL, 10, &step, "") != LONGINT_OK) {
      error(0, 0, _("invalid integer argument: %s\n"), quote(argv[optind+1]));
      usage(EXIT_FAILURE);
    }
  }

  print_dates(start_tm, end_tm, step, output_format);
  
  exit(EXIT_SUCCESS);
}
