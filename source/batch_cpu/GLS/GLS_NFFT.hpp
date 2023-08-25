#include <cmath>
#include <complex>
#include "../../extras/vertex_32.hpp"
#include <stdlib.h>

#include "trig_sum.hpp"

inline void subtractDotProduct(float* y, float* w, int size) {
    float dotProduct = 0.0f;
    for (int i = 0; i < size; ++i) {
        dotProduct += w[i] * y[i];
    }

    for (int i = 0; i < size; ++i) {
        y[i] -= dotProduct;
    }
}

output_data gls_b(float* t,float* y,float* e_y, unsigned int n, unsigned int nk, float fstep, float* f) { //

output_data best_frequency; best_frequency.power = 0; best_frequency.power = 0; best_frequency.sum_of_powers = 0;

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

   float wsum=0, Y=0, YY=0, C, S, YC, YS, CS, D, self_a, self_b, tmp, wsum_1;
   float *w = (float *) malloc(n * sizeof(float)),
          *t2 = (float *) malloc(n * sizeof(float)), //2*w - required for NFFT
          *wy = (float *) malloc(n * sizeof(float)),
          *Sh = (float *) malloc(nk * sizeof(float)),
          *Ch = (float *) malloc(nk * sizeof(float)),
          *SS = (float *) malloc(nk * sizeof(float)),
          *CC = (float *) malloc(nk * sizeof(float));
          //*cosx = (float *) malloc(n * sizeof(float)),
          //*sinx = (float *) malloc(n * sizeof(float)),
          //*cosdx = (float *) malloc(n * sizeof(float)),
          //*sindx = (float *) malloc(n * sizeof(float));
          ;
   unsigned int i, k;

    for (i=0; i<n; ++i) {
      /* weights */
      w[i] = 1 / (e_y[i] * e_y[i]);
      wsum += w[i];
   }

    for (i=0; i<n; ++i) {
      /* mean */
      w[i] /= wsum;
   }

   wsum_1 = 1/wsum;

   for (i=0; i<n; ++i) {
      /* mean */
      w[i] *= wsum_1;
      t2[i] = t[i] * 2;
   }

   /*
   for (i=0; i<n; ++i) {    // Center the data. Even if we're fitting the offset, this step makes the expressions below more succint
    y[i] = y[i] - (w[i] * y[i]);
    }*/

   subtractDotProduct(y, w, n); // Center the data. Even if we're fitting the offset, this step makes the expressions below more succint

    //if nk%2 != 0 //dodaj jeden element, jeśli nieparzyste

    //fstep - rozmiar kroku

   Sh, Ch = trig_sum(t, wy);
   SS, CC = trig_sum(t2, w);




return best_frequency;}
