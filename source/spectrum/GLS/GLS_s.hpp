#include <cmath>
#include "../../utils/grid.hpp"
#include "../../utils/readout.hpp"
/* Author: Mathias Zechmeister
 * Date: 2018-10-01
 */


void gls_s(const star &data, const Grid &grid, double* p) {

   const uint nk = grid.freq.size();
   const uint n = data.x.size();

   /*
    * t : time array
    * y : data array
    * e_y : uncertainties array
    * n : length of data
        * fbeg : start frequencies
        * nk : number of frequencies
        * fstep : step size for frequency
    * f : frequency array
    */

   double wsum=0, Y=0, YY=0;
   double *w = (double *) malloc(n * sizeof(double)),
          *wy = (double *) malloc(n * sizeof(double)),
          *cosdx = (double *) malloc(n * sizeof(double)),
          *sindx = (double *) malloc(n * sizeof(double));


   for (unsigned int i=0; i<n; ++i) {
      /* weights */
      w[i] = 1 / (data.dy[i] * data.dy[i]);
      wsum += w[i];
   }
   for (unsigned int i=0; i<n; ++i) {
      /* mean */
      w[i] /= wsum;                 /* normalised weights */
      Y += w[i] * data.y[i];             /* Eq. (7) */
   }
   for (unsigned int i=0; i<n; ++i) {
      /* variance */
      wy[i] = data.y[i] - Y;             /* Subtract weighted mean */
      YY += w[i] * wy[i] * wy[i];   /* Eq. (10) */
      wy[i] *= w[i];                /* attach weights */

      /* Prepare trigonometric recurrences cos(dx)+i sin(dx) */
      cosdx[i] = cos(2 * M_PI * grid.fstep * data.x[i]);
      sindx[i] = sin(2 * M_PI * grid.fstep * data.x[i]);
   }
   /*printf("%i \n");*/






#pragma omp parallel for
for (unsigned int k=0; k < nk; k+= 128) {

double     *cosx = (double *) malloc(n * sizeof(double)),
          *sinx = (double *) malloc(n * sizeof(double));

   for (unsigned int j = 0; j < std::min<unsigned int>(128, (nk - k)); ++j) {
   double C = 0, S = 0, YC = 0, YS = 0, CC = 0, SS = 0, CS = 0, D = 0, self_a = 0, self_b = 0, tmp = 0;
      for (unsigned int i=0; i<n; ++i) {
         if (j == 0) {
            /* init recurrences to stop error propagation */
            cosx[i] = cos(2 * M_PI * grid.freq[k + j] * data.x[i]);
            sinx[i] = sin(2 * M_PI * grid.freq[k + j] * data.x[i]);
         }

         C += w[i] * cosx[i];              /* Eq. (8) */
         S += w[i] * sinx[i];              /* Eq. (9) */
         YC += wy[i] * cosx[i];            /* Eq. (11) */
         YS += wy[i] * sinx[i];            /* Eq. (12) */
         CC += w[i] * cosx[i] * cosx[i];   /* Eq. (13) */
         CS += w[i] * cosx[i] * sinx[i];   /* Eq. (15) */

         /* increase freq for next loop */
         tmp = cosx[i] * cosdx[i] - sinx[i] * sindx[i];
         sinx[i] = cosx[i] * sindx[i] + sinx[i] * cosdx[i];
         cosx[i] = tmp;
      }
   







      SS = 1. - CC;
      CC -= C * C;              /* Eq. (13) */
      SS -= S * S;              /* Eq. (14) */
      CS -= C * S;              /* Eq. (15) */
      D = CC*SS - CS*CS;        /* Eq. (6) */


      /* power */
      p[j + k] = (SS*YC*YC + CC*YS*YS - 2*CS*YC*YS) / (YY*D);  /* Eq. (5) in ZK09 */
   }
free(cosx); free(sinx);
}
free(w); free(wy); free(cosdx); free(sindx);
}
