#include <cmath>

//const float tau = 6.283185307179586;
using namespace std;



/* Author: Mathias Zechmeister
 * Date: 2018-10-01
 */
void gls_s(float* t,float* y,float* e_y, unsigned int n,unsigned int nk, float fstep,float* f,float* p) { //


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

   float wsum=0, Y=0, YY=0;
   float *w = (float *) malloc(n * sizeof(float)),
          *wy = (float *) malloc(n * sizeof(float)),
          *cosdx = (float *) malloc(n * sizeof(float)),
          *sindx = (float *) malloc(n * sizeof(float));


   for (unsigned int i=0; i<n; ++i) {
      /* weights */
      w[i] = 1 / (e_y[i] * e_y[i]);
      wsum += w[i];
   }
   for (unsigned int i=0; i<n; ++i) {
      /* mean */
      w[i] /= wsum;                 /* normalised weights */
      Y += w[i] * y[i];             /* Eq. (7) */
   }
   for (unsigned int i=0; i<n; ++i) {
      /* variance */
      wy[i] = y[i] - Y;             /* Subtract weighted mean */
      YY += w[i] * wy[i] * wy[i];   /* Eq. (10) */
      wy[i] *= w[i];                /* attach weights */

      /* Prepare trigonometric recurrences cos(dx)+i sin(dx) */
      cosdx[i] = cos(2 * M_PI * fstep * t[i]);
      sindx[i] = sin(2 * M_PI * fstep * t[i]);
   }
   /*printf("%i \n");*/






#pragma omp parallel for
for (unsigned int k=0; k < nk; k+= 128) {

float     *cosx = (float *) malloc(n * sizeof(float)),
          *sinx = (float *) malloc(n * sizeof(float));

   for (unsigned int j = 0; j < std::min<unsigned int>(128, (nk - k)); ++j) {
   float C = 0, S = 0, YC = 0, YS = 0, CC = 0, SS = 0, CS = 0, D = 0, self_a = 0, self_b = 0, tmp = 0;
      for (unsigned int i=0; i<n; ++i) {
         if (j == 0) {
            /* init recurrences to stop error propagation */
            cosx[i] = cos(2 * M_PI * f[k + j] * t[i]);
            sinx[i] = sin(2 * M_PI * f[k + j] * t[i]);
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
