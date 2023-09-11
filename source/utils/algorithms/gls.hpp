#include <cmath>
#include "../vertex.hpp"

#ifndef GLS_HPP
#define GLS_HPP

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

#endif
