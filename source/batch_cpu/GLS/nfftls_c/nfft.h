/** \file nfft.h
 * Declaration for NFFTs.
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
#ifndef NFFT_H
#define NFFT_H

#include <complex.h>
#include <stdbool.h>

extern void cnfft(const double* t, const double complex * y, int n, int m,  double complex * d);
extern void rnfft(const double* t, const double* y, int n, int m,  double complex * d);
extern void rdft(const double* t, const double* y,  int n, const double *f, int m, int dir ,  double complex * d);
extern void cdft(const double* t, const double complex * y,  int n, const double *f, int m, int dir ,  double complex * d ) ;



void nfft(const double* t, const double* y, int n, int m,  double complex * d) ;


#endif /* !NFFT_H */
