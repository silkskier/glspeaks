/** \file io.c
 * Input/output utilities.
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
#include <string.h>

#include "io.h"
#include "utils.h"

/** Maximum character buffer length. */
#define MAXBUF 1000

/** Increment in number of data samples. */
static int INCR = 1000;

/**
 * Sets the increment in data samples
 * for readDataFile().
 *
 * \param newval the new increment value.
 *
 * \return the old increment value.
 */
int setReadDataFileIncrement(int newval)
{
  int old = INCR;
  INCR = newval;
  return old;
}

/**
 * Reads a line and stores it into a buffer.
 *
 * @param line the line to read.
 * @param maxlen the maximal line length.
 * @param fp the \a FILE opened for reading.
 *
 * @return the actual line length (always
 *         less than \a maxlen).
 */
static int readLine(char *line, int maxlen, FILE *fp)
{
  *line = '\0';
  if (fgets(line, maxlen, fp) == NULL)
    return 0;

  return strlen(line);
}

/**
 * Reads the datafile.
 *
 * - Input:
 * \param name the name of the file.
 *
 * - Output:
 * \param t points to the sampling times.
 * \param y points to the measurements.
 *
 * \return the nomber of data points read.
 *
 * \note Comment lines (i.e., lines starting with '#')
 *       are skipped.
 * \note The entries of the data tables are dynamically
 *       allocated in chunks of fixed size (default: 1000
 *       entries); this size may be changed with the function
 *       setDataFileIncrement().
 */
int readDataFile(const char* name, double** t, double** y)
{
  FILE* fp = fopen(name, "r");
  if (!fp)
    Message(MSG_ERROR, "can't open input file `%s'", name);

  *t = NULL;
  *y = NULL;
  int n = 0;			/* number of data samples found */

  char buffer[MAXBUF];
  int len = 0;
  while ((len = readLine(buffer, MAXBUF, fp)) > 0)
  {
    if (len > MAXBUF - 1)
    {
      Message(MSG_ERROR,
	      "line too long (%ld > %d) in file `%s'\n",
	      len, MAXBUF, name);
    }

    if (n % INCR == 0)
    {
      double* tt = xrealloc(*t, (n + INCR) * sizeof(double));
      double* yy = xrealloc(*y, (n + INCR) * sizeof(double));
      *t = tt;
      *y = yy;
    }
    
    if (*buffer == '#') /* skips line of comments */
      continue;
    
    int retval = sscanf(buffer, "%lf %lf", &(*t)[n], &(*y)[n]);
    if (retval != 2)
      Message(MSG_ERROR, "incorrect format of input file `%s'\n", name);
    else
      n++;
  }
  
  fclose(fp);

  return n;
}

