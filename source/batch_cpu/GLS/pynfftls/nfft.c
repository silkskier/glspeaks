/** \file nfft.c
 * Nonuniform Fourier Transforms.
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
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <complex.h>
#include <nfft3.h>
#include "utils.h"
#include "ls.h"

#ifndef PI
#define PI acos(-1.)
#endif



const char* ProgName = "pynfftls";
const char* VersionStr = "1.5";



void nfft(const double* t, const double* y, int n, int m,  double complex * d)
{
  nfft_plan p;
  nfft_init_1d(&p, 2 * m, n);

  if (y)			/* spectrum */
  {
    for (int i = 0; i < n; i++)
    {
      p.x[i] = t[i];
      p.f[i] = y[i];
    }
  }
  else				/* window */
  {
    for (int i = 0; i < n; i++)
    {
      p.x[i] = t[i];
      p.f[i] = 1.0;
    }
  }

  if (p.flags & PRE_ONE_PSI)
    nfft_precompute_one_psi(&p);

  nfft_adjoint(&p);


  for (int i = 0; i < m; i++)
    d[i] = p.f_hat[i + m];
  /*
   * NFFT plan produces Fourier components for
   * frequencies in an open interval [-m, m[;
   * but here we're working with the closed interval.
   */
  d[m] = conj(p.f_hat[0]);

  nfft_finalize(&p);
}


/* Computation of the /full/ Non-equidistant Fast Fourier Transform (NFFT) of a real time-series y(t).
*
* Inputs:
* t the times reduced to [1/2 , 1/2)
* y the measurements (NULL , for computing the FT of the window )
* n the number of measurements
* m the number of positive frequencies (null frequency included)

* Output :
* d the Fourier coefficients ( preallocated array of (2*m) elements )
* associated with  m negative frequencies, and m positive frequencies.
*
*/
void rnfft(const double* t, const double* y, int n, int m,  double complex * d )
{
  nfft_plan p;
  nfft_init_1d(&p, 2 * m, n);


  if (y)			/* spectrum */
  {
    for (int i = 0; i < n; i++)
    {
      p.x[i] = t[i];
      p.f[i] = y[i];

    }
  }
  else				/* window */
  {
    for (int i = 0; i < n; i++)
    {
      p.x[i] = t[i];
      p.f[i] = 1.0;
    }
  }

  if (p.flags & PRE_ONE_PSI)
    nfft_precompute_one_psi(&p);

  nfft_adjoint(&p);
  /*
   * NFFT plan produces Fourier components for
   * frequencies in an open interval [-m, m[;
   */


  for (int i = 0; i < m; i++)// negative frequencies (m)
    d[i+m] = conj(p.f_hat[i]);

  for (int i = 0; i < m; i++) // positive frequencies (m) , null frequency included
    d[i] = conj(p.f_hat[ i+m]);

  nfft_finalize(&p);

}



/* Computation of the /full/ Non-equidistant Fast Fourier Transform (NFFT) of a complex time-series y(t).
*
* Inputs:
* t the times reduced to [1/2 , 1/2)
* y the measurements (NULL , for computing the FT of the window )
* n the number of measurements
* m the number of positive frequencies (null frequency included)

* Output :
* d the Fourier coefficients ( preallocated array of (2*m) elements )
* associated with  m negative frequencies, and m positive frequencies.
*
*/
void cnfft(const double* t, const double complex * y, int n, int m,  double complex * d )
{
  nfft_plan p;

  nfft_init_1d(&p, 2 * m, n);

  if (y)			/* spectrum */
  {
    for (int i = 0; i < n; i++)
    {
      p.x[i] = t[i];
      p.f[i] = y[i];
    }
  }
  else				/* window */
  {
    for (int i = 0; i < n; i++)
    {
      p.x[i] = t[i];
      p.f[i] = 1.0;
    }
  }

  if (p.flags & PRE_ONE_PSI)
    nfft_precompute_one_psi(&p);

   nfft_adjoint(&p);

  /*
   * NFFT plan produces Fourier components for
   * frequencies in an open interval [-m, m[;
   */


  for (int i = 0; i < m; i++)// negative frequencies (m)
    d[i+m] = conj(p.f_hat[i]);

  for (int i = 0; i < m; i++) // positive frequencies (m) , null frequency included
    d[i] = conj(p.f_hat[ i+m]);




  nfft_finalize(&p);
}




/* Computation of the Discrete Fast Fourier Transform (NFFT) of a real time-series y(t).
*
* Inputs:
* t the times
* y the measurements
* n the number of measurements
* f the frequencies for which we want to compute the DFT
* m the number of frequencies
* dir the direction of the trasnform (dir>= for the forward transform, and dir<0 for the inverse transform)

* Output :
* d the Fourier coefficients ( preallocated array of m elements )
* associated with  the frequencie f
*
*/
void rdft(const double* t, const double* y,  int n, const double *f, int m, int dir ,  double complex * d )
{
  double dirf = (dir >=0) ? 1. : -1. ;
  double twopi = 2.*PI ;
  double complex sum ;
  double  w , a ;
  for (int i=0 ; i < m; i++) {
    sum = 0. ;
    a = twopi*f[i] ;
    for (int j=0 ; j <n ; j++)
      {
	w = a*t[j] ;
	sum = sum + y[j] * cos(w) - _Complex_I * (y[j] * dirf * sin(w) ) ;
      }
    d[i] = sum ;
  }

}



/* Computation of the Discrete Fast Fourier Transform (NFFT) of a complex time-series y(t).
*
* Inputs:
* t the times
* y the measurements
* n the number of measurements
* f the frequencies for which we want to compute the DFT
* m the number of frequencies
* dir the direction of the trasnform (dir>= for the forward transform, and dir<0 for the inverse transform)

* Output :
* d the Fourier coefficients ( preallocated array of m elements )
* associated with  the frequencie f
*
*/
void cdft(const double* t, const double complex * y,  int n, const double *f, int m, int dir ,  double complex * d )
{
  double dirf = (dir >=0) ? 1. : -1. ;
  double twopi = 2.*PI ;
  double complex sum ;
  double c , s , w , a ;
  for (int i=0 ; i < m; i++) {
    sum = 0. ;
    a = twopi*f[i] ;
    for (int j=0 ; j <n ; j++) {
      w = a*t[j] ;
      c = cos(w);
      s = dirf * sin(w);
      sum = sum +  ( creal(y[j]) *  c +  cimag(y[j]) * s )
	+ _Complex_I * ( cimag(y[j]) *  c  - creal(y[j]) * s ) ;
    }
    d[i] = sum ;
  }

}


