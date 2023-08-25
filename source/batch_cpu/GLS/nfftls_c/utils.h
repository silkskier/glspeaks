/** \file utils.h
 * Declarations for various utility functions.
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
#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

void *xmalloc(size_t size);
void *xrealloc(void* p, size_t size);
char *xstrdup(const char *s);
double xstrtod(const char *s);
long xstrtol(const char *s);
double centerData(int n0, double *y, double *var);
extern void reduce(double* t, int npts, double oversampling);
void meanAndVariance(int n, const double* y, double* mean, double* variance);

#ifdef BENCHMARK
double timeUsed();
#endif

extern const char* ProgName;
extern const char* VersionStr;

typedef enum { MSG_WARNING, MSG_ERROR, MSG_USAGE } Mode;
void Message(Mode mode, const char *fmt, ...);

#endif /* !UTILS_H */
