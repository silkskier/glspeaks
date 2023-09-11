#include <cmath>
#include <iostream>
#include "../../utils/periodograms.hpp"
#include "../../utils/grid.hpp"
#include "../../utils/readout.hpp"
//#include "../../extras/NFFT.hpp"

/* Author: Mathias Zechmeister
 * Date: 2018-10-01
 */


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
