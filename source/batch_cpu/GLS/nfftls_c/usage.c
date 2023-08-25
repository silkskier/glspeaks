/** \file usage.c
 * Implementation of getOptions().
 *
 * \author B. Leroy
 *
 * This file is part of nfftls.
 *
 * Nfftls is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nfftls is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with nfftls.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2012 by B. Leroy
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "usage.h"
#include "utils.h"

#define CMDLEN 1024

/**
 * Incrementally stores the command line
 * in a way appropriate for printing in
 * results file.
 *
 * \param name     short option name string.
 * \param val      value string or NULL.
 *
 * \return the formatted command line (each output
 *         line prefixed by '# ').
 */
static char *store(const char *name, const char *val)
{
  static char *cmd = NULL;
  static char *p = NULL;
  static int i = 0;
  static size_t remlen = 0;
  static size_t total_len = 0;
  
  size_t curlen;
  size_t len = strlen(name) + ((val) ? strlen(val) : 0);
  
  if (remlen <= len)
  {
    p = (char *)realloc(cmd, ++i * CMDLEN * sizeof(char));
    if (!p)
      Message(MSG_ERROR, "not enough memory for storing the command line.");
    cmd = p;
    p += total_len;
    remlen += CMDLEN;
  }
  
  sprintf(p, "# %s %s \\\n", name, (val) ? val : "");

  curlen = strlen(p);
  
  p += curlen;
  remlen -= curlen;
  total_len += curlen;
  
  return cmd;
}

/** Prints a short usage reminder. */
static void briefUsage(void)
{
  Message(MSG_USAGE,
	  "Usage: %s --data-file|-d FILE [--output-file|-o FILE] \\\n"
	  "              [--oversampling|-s REAL] [--hifreq-factor|-f REAL] \\\n"
          "              [--number-frequencies|-n INTEGER]\n"
	  "       %s --version|-v\n"
	  "       %s --help|-h",
	  ProgName, ProgName, ProgName);
  exit(EXIT_FAILURE);
}

/** Prints a detailed usage message. */
static void usage(void)
{
  Message(MSG_USAGE,
	  "Usage: %s --data-file|-d DATAFILE[--output-file|-o OUTFILE]  \\\n"
	  "              [--oversampling|-s OVER] [--hifreq-factor|-f HIFAC] \\\n"
          "              [--number-frequencies|-n INTEGER]\n"
	  "       %s --version|-v\n"
	  "       %s --help|-h\n\n"
	  "This program outputs to stdout or to the file OUTFILE the Lomb-Scargle\n"
	  "periodogram of a data set found in the file DATAFILE in 2-column ASCII\n"
	  "format.  The oversampling factor OVER is a real number >= 1 (default: 4);\n"
	  "the high frequency control factor HIFAC is a real number > 0\n"
	  "(default: 1).\n\n"
          "Option --number-frequencies fixes the number of (independent) frequencies\n"
	  "to use for the periodogram.\n\n"
	  "This program is copyrighted under the GNU General Public License 3.\n"
	  "Copyright (C) 2012 by B. Leroy.",
	  ProgName, ProgName, ProgName);

  exit(EXIT_FAILURE);
}

void initOptions(Options* opts, double over, double hifac)
{
  opts->datafile = NULL;
  opts->outfile = NULL;
  opts->cmdline = NULL;
  opts->oversampling = over;
  opts->hifactor = hifac;
  opts->nfreqs = 0;
}

void cleanupOptions(Options* opts)
{
  free((void*)(opts->cmdline));
  if (opts->outfile)
    free((void*)(opts->outfile));
  free((void*)(opts->datafile));
}

int getOptions(int argc, char** argv, Options* opts)
{
  if (argc < 2)
    briefUsage();
  
  static int help = 0;
  static int version = 0;
  
  /* Flags for mandatory options. */
  int dflag = 0;		/* data file */

  /* Flags for optional options. */
  int nflag = 0;		/* number of frequencies */
  int sflag = 0;		/* oversampling > 1 */
  int fflag = 0;		/* hifreq factor defined */

  /* To ensure using GNU basename. */
#define basename
  char optname[CMDLEN];
  char* cmd = store((const char*)basename(argv[0]), NULL);
  
  while (1)
  {
    static struct option long_options[] =
      {
	{ "data-file", required_argument, 0, 'd' },
	{ "output-file", required_argument, 0, 'o' },
	{ "oversampling", required_argument, 0, 's' },
	{ "hifreq-factor", required_argument, 0, 'f' },
	{ "number-frequencies", required_argument, 0, 'n' },
	{ "version", no_argument, &version, 1 },
	{ "help", no_argument, &help, 1 },
	{ 0, 0, 0, 0 }
      };

    int option_index = 0;
    int c = getopt_long(argc, argv, "hvd:n:o:s:f:", long_options, &option_index);

    /* End of the options. */
    if (c == -1)
      break;
    
    switch(c)
    {
      /* Options setting a flag. */
      case 0:
	sprintf(optname, "--%s", long_options[option_index].name);
	cmd = store(optname, NULL);
	break;

	/* Data file. */
      case 'd':
	opts->datafile = xstrdup(optarg);
	dflag = 1;
	cmd = store("-d", optarg);
	break;

	/* Output file. */
      case 'o':
	opts->outfile = xstrdup(optarg);
	cmd = store("-o", optarg);
	break;

	/* Oversampling. */
      case 's':
	opts->oversampling = xstrtod(optarg);
	if (opts->oversampling < 1.0)
	  Message(MSG_ERROR, "the oversampling must be >= 1.0");
	sflag = 1;
	cmd = store("-s", optarg);
	break;

	/* High frequency control parameter. */
      case 'f':
	opts->hifactor = xstrtod(optarg);
	if (opts->hifactor <= 0)
	  Message(MSG_ERROR, "the high frequency control parameter must be > 0");
	fflag = 1;
	cmd = store("-f", optarg);
	break;

	/* Number of (independent) frequencies. */
      case 'n':
	opts->nfreqs = (int)xstrtol(optarg);
	if (opts->nfreqs <= 0)
	  Message(MSG_ERROR, "the number of independent frequencies must be > 0");
	nflag = 1;
	cmd = store("-n", optarg);
	break;

	/* Help. */
      case 'h':
	help = 1;
	break;

      case 'v':
	version = 1;
	break;
	
      case '?':
	/* getopt_long already printed an error message. */
	briefUsage();
	break;

      default:
	Message(MSG_ERROR, "unreachable code - please contact the developer");
    }
  }

  char *p = strrchr(cmd, '\\');
  if (p)
    *p = '\0';
  opts->cmdline = cmd;
  
  if (help)
    usage();

  if (version)
  {
    printf("%s - v%s - (C) 2012, B. Leroy\n", ProgName, VersionStr);
    exit(EXIT_FAILURE);
  }
  
  if (!dflag)
  {
    fprintf(stderr, "%s: *** ERROR: missing name of datafile\n\n", ProgName);
    briefUsage();
  }

  if (nflag && (sflag || fflag))
  {
    Message(MSG_WARNING,
	    "Option --number-frequencies should not be combined with options\n"
	    "--oversampling or --hifreq-factor.  Setting therefore both \n"
	    "oversampling and hifreq factor equal to 1.");
  }
  
  return optind;
}
