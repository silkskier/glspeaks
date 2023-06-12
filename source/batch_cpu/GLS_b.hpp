#include <cmath>

//const float tau = 6.283185307179586;
using namespace std;

struct output_data {
	float power, amplitude, frequency;
	double sum_of_powers;
};

/* Author: Mathias Zechmeister
 * Date: 2018-10-01
 */
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

   float wsum=0, Y=0, YY=0, C, S, YC, YS, CC, SS, CS, D, self_a, self_b, tmp;
   float *w = (float *) malloc(n * sizeof(float)),
          *wy = (float *) malloc(n * sizeof(float)),
          *cosx = (float *) malloc(n * sizeof(float)),
          *sinx = (float *) malloc(n * sizeof(float)),
          *cosdx = (float *) malloc(n * sizeof(float)),
          *sindx = (float *) malloc(n * sizeof(float));
   unsigned int i, k;

   for (i=0; i<n; ++i) {
      /* weights */
      w[i] = 1 / (e_y[i] * e_y[i]);
      wsum += w[i];
   }
   for (i=0; i<n; ++i) {
      /* mean */
      w[i] /= wsum;                 /* normalised weights */
      Y += w[i] * y[i];             /* Eq. (7) */
   }
   for (i=0; i<n; ++i) {
      /* variance */
      wy[i] = y[i] - Y;             /* Subtract weighted mean */
      YY += w[i] * wy[i] * wy[i];   /* Eq. (10) */
      wy[i] *= w[i];                /* attach weights */

      /* Prepare trigonometric recurrences cos(dx)+i sin(dx) */
      cosdx[i] = cos(2 * M_PI * fstep * t[i]);
      sindx[i] = sin(2 * M_PI * fstep * t[i]);
   }
   /*printf("%i \n");*/





   for (k=0; k<nk; ++k) {

      C = S = YC = YS = CC = CS = 0, self_a = 0, self_b = 0;
	  float power = 0, amplitude = 0; //declares output variables

      for (i=0; i<n; ++i) {
         if (k % 256 == 0) {
            /* init/refresh recurrences to stop error propagation */
            cosx[i] = cos(2 * M_PI * f[k] * t[i]);
            sinx[i] = sin(2 * M_PI * f[k] * t[i]);
         }

         C += w[i] * cosx[i];              /* Eq. (8) */
/*          printf("%.10g %.10g %.10g %.10g %.10g\n", f[k], w[i], t[i], cosx[i], C);*/
         S += w[i] * sinx[i];              /* Eq. (9) */
         YC += wy[i] * cosx[i];            /* Eq. (11) */
         YS += wy[i] * sinx[i];            /* Eq. (12) */
/*         wcosx = w * cosx; */
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
      power = (SS*YC*YC + CC*YS*YS - 2*CS*YC*YS) / (YY*D);  /* Eq. (5) in ZK09 */

      //update output data struct
if (std::isnormal(power)) {best_frequency.sum_of_powers += power;}

if (power > best_frequency.power){
    self_a = (YC*SS-YS*CS) / D;
    self_b = (YS*CC-YC*CS) / D;

    amplitude = sqrt((self_a * self_a) + (self_b * self_b));

  best_frequency.frequency = f[k];
  best_frequency.amplitude = amplitude;
  best_frequency.power = power;
}}

   free(w); free(wy); free(cosx); free(sinx); free(cosdx); free(sindx);
return best_frequency;}
