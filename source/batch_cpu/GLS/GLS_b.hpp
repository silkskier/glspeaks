#include <cmath>
#include <iostream>
#include "../../utils/vertex.hpp"
#include "../../utils/grid.hpp"
#include "../../utils/readout.hpp"
//#include "../../extras/NFFT.hpp"

/* Author: Mathias Zechmeister
 * Date: 2018-10-01
 */

inline void gls_freq(const uint &k, const uint &n, //function used for GLS calculation for each frequency
      float &SS, float &YY, const float* const f,
      const float* const t, const float* const w, const float* const wy, float* cosx, float* sinx,  const float* const cosdx, const float* const sindx,
      output_data &best_frequency){

      float C, D, S, YC, YS, CC, CS, tmp, self_a, self_b;
      C = D = S = YC = YS = CC = CS = tmp = self_a = self_b = 0;
	  float power = 0, amplitude = 0; //declares output variables

      for (uint i=0; i<n; ++i) {
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




inline void gls_dfreq(const uint &n,  //function used for fine-tuning of resulting best frequency
      float &YY, double f,
      const double* const t, const float* const w, const float* const wy,
      output_data &best_frequency){

      double cosx, sinx, SS, C, D, S, YC, YS, CC, CS, self_a, self_b;
      C = D = S = YC = YS = CC = CS = self_a = self_b = 0;
	  float power = 0, amplitude = 0; //declares output variables

	  for (uint i=0; i<n; ++i) {

            /* init/refresh recurrences to stop error propagation */
            cosx = cos(2 * M_PI * f * t[i]);
            sinx = sin(2 * M_PI * f * t[i]);

         C += w[i] * cosx;              /* Eq. (8) */
/*          printf("%.10g %.10g %.10g %.10g %.10g\n", f, w[i], t[i], cosx[i], C);*/
         S += w[i] * sinx;              /* Eq. (9) */
         YC += wy[i] * cosx;            /* Eq. (11) */
         YS += wy[i] * sinx;            /* Eq. (12) */
/*         wcosx = w * cosx; */
         CC += w[i] * cosx * cosx;   /* Eq. (13) */
         CS += w[i] * cosx * sinx;   /* Eq. (15) */


      }


      SS = 1. - CC;
      CC -= C * C;              /* Eq. (13) */
      SS -= S * S;              /* Eq. (14) */
      CS -= C * S;              /* Eq. (15) */
      D = CC*SS - CS*CS;        /* Eq. (6) */


      /* power */
      power = (SS*YC*YC + CC*YS*YS - 2*CS*YC*YS) / (YY*D);  /* Eq. (5) in ZK09 */

//update output data struct
if (power > best_frequency.power){
    self_a = (YC*SS-YS*CS) / D;
    self_b = (YS*CC-YC*CS) / D;

    amplitude = sqrt((self_a * self_a) + (self_b * self_b));

  best_frequency.frequency = f;
  best_frequency.amplitude = amplitude;
  best_frequency.power = power;
}}




//output_data gls_b(double* t,float* y,float* e_y, unsigned int n, unsigned int nk, float fstep, const float* f) { //
output_data gls_b(const star &data, const grid &grid) { //
output_data best_frequency; best_frequency.power = 0; best_frequency.power = 0; best_frequency.sum_of_powers = 0;

   /*
    * data.x : time array
    * data.y : data array
    * data.dy : uncertainties array
    * data.x.size() : length of data
        * grid.freq.size() : number of frequencies
        * grid.fstep : step size for frequency
    * grid.freq : frequency array
    */

   float wsum=0, Y=0, YY=0, C, S, YC, YS, CC, SS, CS, D, self_a, self_b, tmp;
   float *ts = (float *) malloc(data.x.size() * sizeof(float)), //single precision float representation of time
         *w = (float *) malloc(data.x.size() * sizeof(float)),
          *wy = (float *) malloc(data.x.size() * sizeof(float)),
          *cosx = (float *) malloc(data.x.size() * sizeof(float)),
          *sinx = (float *) malloc(data.x.size() * sizeof(float)),
          *cosdx = (float *) malloc(data.x.size() * sizeof(float)),
          *sindx = (float *) malloc(data.x.size() * sizeof(float));
   unsigned int i, k;

   for (i=0; i<data.x.size(); ++i) {
      /* weights */
      w[i] = 1 / (data.dy[i] * data.dy[i]);
      wsum += w[i];
      ts[i] = float(data.x[i]);
   }

   for (i=0; i<data.x.size(); ++i) {
      /* mean */
      w[i] /= wsum;                 /* normalised weights */
      Y += w[i] * data.y[i];             /* Eq. (7) */
   }
   for (i=0; i<data.x.size(); ++i) {
      /* variance */
      wy[i] = data.y[i] - Y;             /* Subtract weighted mean */
      YY += w[i] * data.y[i] * data.y[i];   /* Eq. (10) */
      wy[i] *= w[i];                /* attach weights */

      /* Prepare trigonometric recurrences cos(dx)+i sin(dx) */
      cosdx[i] = cos(2 * M_PI * grid.fstep * ts[i]);
      sindx[i] = sin(2 * M_PI * grid.fstep * ts[i]);
   }



   for (k=0; k<grid.freq.size(); ++k) {
         gls_freq(k, data.x.size(),
         SS, YY,
         grid.freq.data(), ts, w, wy, cosx, sinx, cosdx, sindx,
         best_frequency);

     }

   free(cosdx); free(sindx); free(ts); free(cosx); free(sinx);
   //fine-tune the resulting frequency in order to increase precision

   double dfstep = grid.fstep;

    for (k=0; k<8; ++k) {
    double f0 = best_frequency.frequency - dfstep;
    double f1 = best_frequency.frequency + dfstep;

    gls_dfreq(data.x.size(),
    YY, f0,
    data.x.data(), w, wy,
    best_frequency);

    gls_dfreq(data.x.size(),
    YY, f1,
    data.x.data(), w, wy,
    best_frequency);

    dfstep *= 0.5;
    }


   free(w); free(wy);
//std::cout << best_frequency.power << std::endl;
return best_frequency;}
