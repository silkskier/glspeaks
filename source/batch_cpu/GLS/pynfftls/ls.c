/** \file ls.c
 * Implementation of LombScargle().
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
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nfft.h"
#include "ls.h"
#include "utils.h"

/**
 * Returns the square of its argument.
 *
 * \param a the value to be squared.
 */
inline double square(double a)
{
  return a * a; 
}

/**
 * Transfers the sign of its 2nd argument to its first argument.
 *
 * \param a the argument to which the sign is to be transferred.
 * \param b the argument that transfers its sign.
 *
 * \returns \f$|a| sgn(b).\f$
 */
inline double sign(double a, double b)
{
  return ((b >= 0) ? 1 : -1) * fabs(a);
}

/**
 * Allocates memory for a LombScargle structure.
 *
 * \param n the number of frequencies.
 */
LS* LSalloc(int n)
{
  LS* ls = xmalloc(sizeof(LS));
  ls->freqs = xmalloc(n * sizeof(double));
  ls->Pn = xmalloc(n * sizeof(double));
  ls->nfreqs = n;
  ls->jmax = -1;		/* invalid value */
  ls->proba = -1;		/* invalid value */
  
  return ls;
}

/**
 * Releases the memory occupied by a LombScargle structure.
 *
 * \param ls the LS structure to be freed.
 */
void LSfree(LS* ls)
{
  free(ls->Pn);
  free(ls->freqs);
  free(ls);
}

/**
 * Computes the Lomb-Scargle normalised periodogram of a time series.
 *
 * \param[in] tobs the times.
 * \param[in] yobs measurements.
 * \param[in] npts the length of the time series.
 * \param[in] over the oversampling factor.
 * \param[in] hifac the highest frequency in units of the Nyquist
 *            frequency that would correspond to a uniform sampling.
 *
 * \return a LombScargle structure.
 *
 * \note The periodogram is restricted to the strictly positive frequency part.
 */

LS* periodogram(const double* tobs, const double* yobs, int npts,
		double over, double hifac)
{
  // Clones the data.
  double* t = xmalloc(npts * sizeof(double));
  memcpy(t, tobs, npts * sizeof(double));
  double* y = xmalloc(npts * sizeof(double));
  memcpy(y, yobs, npts * sizeof(double));

  // Centers the data.
  double var;
  centerData(npts, y, &var);

  // Determines the Nyquist frequency for
  // a uniform sampling and the frequency
  // resolution.
  double T = t[npts - 1] - t[0];
  double df = 1.0 / (over * T);

  // Defermines the highest frequency, fmax,
  // and the corresponding index, m, in
  // the positive part of the spectrum.
  //
  // fc = npts / (2 * T)
  // fmax = hifac * fc
  // m = fmax / df
  int m = (int)floor(0.5 * npts * over * hifac);

  // Allocates space for the output frequencies
  // and values of the periodogram.
  int nfreqs = m;
  LS* ls = LSalloc(nfreqs);
    
  // Reduces the times to [-1/2, 1/2).
  reduce(t, npts, over);

#ifdef BENCHMARK
  // Initialises time and memory used.
  ls->tused = timeUsed();
#endif

  // Unnormalised Fourier transform of the data.
  double complex* sp = xmalloc((m + 1) * sizeof(double complex));
  rnfft(t, y, npts, m, sp);

  // Unnormalised Fourier transform of the window.
  double complex* win = xmalloc((2 * m + 1) * sizeof(double complex));
  rnfft(t, NULL, npts, 2 * m, win);
  
  double pmax = -1.0;

  for (int j = 1; j <= m; j++)
  {
    double complex z1 = sp[j];
    double complex z2 = win[2 * j];
    double hypo = cabs(z2);
    double hc2wt = 0.5 * cimag(z2) / hypo;
    double hs2wt = 0.5 * creal(z2) / hypo;
    double cwt = sqrt(0.5 + hc2wt);
    double swt = sign(sqrt(0.5 - hc2wt), hs2wt);
    double den = 0.5 * npts + hc2wt * creal(z2) + hs2wt * cimag(z2);
    double cterm = square(cwt * creal(z1) + swt * cimag(z1)) / den;
    double sterm = square(cwt * cimag(z1) - swt * creal(z1)) / (npts - den);

    int j1 = j - 1;
    ls->freqs[j1] = j * df;
    ls->Pn[j1] = (cterm + sterm) / (2 * var);

    if (ls->Pn[j1] > pmax)
    {
      ls->jmax = j1;
      pmax = ls->Pn[j1];
    }
  }

#ifdef BENCHMARK
  // Determines the time and memory used.
  ls->tused = timeUsed() - ls->tused;
#endif

  // Determines the false alarm probability
  // of the periodogram maximum.
  ls->proba = probability(pmax, npts, nfreqs, over);

  free(win);
  free(sp);
  free(y);
  free(t);
  
  return ls;
}

/**
 * Returns the probability that a peak of a given power
 * appears in the periodogram when the signal is white
 * Gaussian noise.
 *
 * \param Pn the power in the periodogram.
 * \param npts the number of samples.
 * \param nfreqs the number of frequencies in the periodogram.
 * \param over the oversampling factor.
 *
 * \note This is the expression proposed by A. Schwarzenberg-Czerny
 * (MNRAS 1998, 301, 831), but without explicitely using modified
 * Bessel functions.
 * \note The number of effective independent frequencies, effm,
 * is the rough estimate suggested in Numerical Recipes. 
 */
double probability(double Pn, int npts, int nfreqs, double over)
{
  double effm = 2.0 * nfreqs / over;
  double Ix = 1.0 - pow(1 - 2 * Pn / npts, 0.5 * (npts - 3));

  double proba = 1 - pow(Ix, effm);
  
  return proba;
}


/**
 * Computes the Lomb-Scargle normalised periodogram of a time series.
 * 
 * Similar  as periodogram() but computes just the periodogram 
 * (does not return a probability, nor the structure LS) 
 * 
 * The frequencies and the periodogram are in freqs and Pn
 * these arrays must be allocated by the user
 * 
 * Return value : the number of frequencies
 *
 * 
 * 
 */
int periodogram_simple(const double* tobs, const double* yobs , int npts,
		       double over, double hifac, double *  freqs , double * Pn   )

{
  // Clones the data.
  double* t = xmalloc(npts * sizeof(double));
  memcpy(t, tobs, npts * sizeof(double));
  double* y = xmalloc(npts * sizeof(double));
  memcpy(y, yobs, npts * sizeof(double));

  // Centers the data.
  double var;
  centerData(npts, y, &var);

  // Determines the Nyquist frequency for
  // a uniform sampling and the frequency
  // resolution.
  double T = t[npts - 1] - t[0];
  double df = 1.0 / (over * T);

  // Defermines the highest frequency, fmax,
  // and the corresponding index, m, in
  // the positive part of the spectrum.
  //
  // fc = npts / (2 * T)
  // fmax = hifac * fc
  // m = fmax / df
  int m = (int)floor(0.5 * npts * over * hifac);

  // Allocates space for the output frequencies
  int nfreqs = m;
    
  // Reduces the times to [-1/2, 1/2).
  reduce(t, npts, over);

  // Unnormalised Fourier transform of the data.
  double complex* sp = xmalloc((m + 1 ) * sizeof(double complex));
  nfft(t, y, npts, m, sp);

  // Unnormalised Fourier transform of the window.
  double complex* win = xmalloc((2 * m +1) * sizeof(double complex));
  nfft(t, NULL, npts, 2 * m, win);

  /*
  * freqs = xmalloc(nfreqs * sizeof(double));
  * Pn = xmalloc(nfreqs * sizeof(double));
  */
  
  int j ;
  for (j = 1; j <= m; j++)
  {
    double complex z1 = sp[j];
    double complex z2 = win[2 * j];
    double hypo = cabs(z2);
    double hc2wt = 0.5 * cimag(z2) / hypo;
    double hs2wt = 0.5 * creal(z2) / hypo;
    double cwt = sqrt(0.5 + hc2wt);
    double swt = sign(sqrt(0.5 - hc2wt), hs2wt);
    double den = 0.5 * npts + hc2wt * creal(z2) + hs2wt * cimag(z2);
    double cterm = square(cwt * creal(z1) + swt * cimag(z1)) / den;
    double sterm = square(cwt * cimag(z1) - swt * creal(z1)) / (npts - den);

    int j1 = j - 1;
    freqs[j1] = j * df;
    Pn[j1] = (cterm + sterm) / (2 * var);


  }

  free(win);
  free(sp);
  free(t);
  free(y);
  
  return nfreqs ; 
}
