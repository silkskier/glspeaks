/** \file ls.h
 * Declaration for LombScargle().
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
#ifndef FOURIER_H
#define FOURIER_H

typedef struct
{
  double* freqs;
  double* Pn;
  int nfreqs;
  int jmax;		    // index of the peak
  double proba;             // proba of white noise peak at freq. jmax

#ifdef BENCHMARK 
  double tused;             // time used
#endif
} LS;

void LSfree(LS* p);
LS* LSalloc(int m);
LS* periodogram(const double* t, const double* y, int npts,
		double over, double hifac);
int periodogram_simple(const double* tobs, const double* yobs, int npts, double over, double hifac, double *  freqs  , double *  Pn   ) ;

double probability(double Pn, int npts, int nfreqs, double over);


#endif /* !FOURIER_H */
