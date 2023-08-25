/** \file utils.c
 * Various utility functions.
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
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <float.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "utils.h"

void Message(Mode mode, const char *fmt, ...)
{
  va_list ap;
  int code;
  FILE *fp = (mode == MSG_USAGE) ? stdout : stderr;
    
  if (mode != MSG_USAGE)
  {
    fprintf(fp, "%s: *** %s: ",
	    ProgName,
	    (mode == MSG_ERROR) ? "ERROR" : "WARNING");
  }

  va_start(ap, fmt);
  vfprintf(fp, fmt, ap);
  va_end(ap);
  fputc('\n', fp);
    
  code = (mode == MSG_ERROR) ? EXIT_FAILURE : EXIT_SUCCESS;
    
  if (mode != MSG_WARNING)
    exit(code);
}

/**
 * Duplicates a string, or exits
 * the programme in case of an error.
 *
 * @param s the string to duplicate.
 */
char *xstrdup(const char *s)
{
  char *p = malloc(strlen(s) + 1);
  if (!p)
    Message(MSG_ERROR, "not enough memory for cloning a string");

  return strcpy(p, s);
}

/**
 * Allocates memory, or exits the programme
 * in case of an error.
 *
 * @param size the memory space.
 */
void *xmalloc(size_t size)
{
  void *p = malloc(size);
  if (!p)
    Message(MSG_ERROR, "not enough memory");
  return p;
}


/**
 * Reallocates memory, or exits the programme
 * in case of an error.
 *
 * \param p the memory area to be reallocated.
 * \param size the memory space.
 */
void* xrealloc(void* p, size_t size)
{
  void* q = realloc(p, size);
  if (!q)
    Message(MSG_ERROR, "not enough memory for reallocation");
  return q;
}

/**
 * Computes the mean and (unbiased) variance
 * of a set of measurements using an acurate
 * one-pass algorithm.
 *
 * \param[in] n the number of measurements.
 * \param[in] y the table of measurements.
 * \param[out] the mean.
 * \param[out] the variance.
 */
void meanAndVariance(int n, const double* y, double* mean, double* variance)
{
  *mean = 0;
  double M2 = 0;

  int nn = 1;
  for (int i = 0; i < n; i++, nn++)
  {
    double delta = y[i] - *mean;
    
    *mean +=  delta / nn;
    M2 += delta * (y[i] - *mean); // This expression uses the new value of mean
  }

  *variance = M2 / (n - 1);
}

/**
 * Centers the measurements around their unweighed average,
 * and determines their unbiased variance.
 *
 * @param n the number of points.
 * @param y table of the measurements (will be modified).
 * @param var the variances of the data.
 *
 * @return the average of the measurements.
 */
double centerData(int n, double *y, double *var)
{
  double average = 0.0;
  meanAndVariance(n, y, &average, var);

  for (int i = 0; i < n; i++)
    y[i] -= average;

  return average;
}

/**
 * Robust strtod.
 *
 * @param s the string out of which a double
 *          is to be extracted.
 * 
 * Exits in case of failed conversion
 * for whatever reason.
 */
double xstrtod(const char *s)
{
  double val;
  char *endp;

  val = strtod(s, &endp);
  if (errno == ERANGE)
  {
    Message(MSG_ERROR,
	    "cannot convert string `%s' to a representable floating number",
	    s);
  }

  if (val == 0 && endp == s)
  {
    Message(MSG_ERROR,
	    "string `%s' does not represent a number", s);
  }

  return val;
}

/**
 * Robust strtol.
 *
 * @param s the string out of which a long
 *          is to be extracted.
 * 
 * Exits in case of failed conversion.
 */
long xstrtol(const char *s)
{
  long val;
  char *endp;

  val = strtol(s, &endp, 10);
  if (errno == ERANGE)
  {
    Message(MSG_ERROR,
        "cannot convert string `%s' to a representable integer", s);
  }

  if (val == 0L && endp == s)
  {
    Message(MSG_ERROR, "string `%s' does not represent an integer", s);
  }

  return val;
}

/**
 * Reduces the times to the interval [-1/2, 1/2).
 *
 * \param[in,out] the times.
 * \param[in] npts the number of time values.
 * \param[in] oversampling the oversampling factor.
 *
 * \note The initial time array is modified.
 */
void reduce(double* t, int npts, double oversampling)
{

  double tmax = t[npts - 1] ;
  double tmin = t[0];

  double range = oversampling * (tmax - tmin);

  const double a = 0.5 ;

  // Reduce to [-1/2, 1/2[
  for (int i = 0; i < npts; i++)
    t[i] = 2 * a * (t[i] - tmin) / range - a;
}

#ifdef BENCHMARK
double timeUsed(void)
{
  static struct rusage tmp;
  double t1, t2;

  getrusage(RUSAGE_SELF, &tmp);
  t1 = tmp.ru_utime.tv_sec;	/* seconds */
  t2 = tmp.ru_utime.tv_usec;	/* milliseconds */

  return t1 + 1e-6 * t2;
}
#endif
