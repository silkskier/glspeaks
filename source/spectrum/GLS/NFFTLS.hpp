#include <cmath>
#include <complex>

//includes for finufft
#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <finufft.h>

#include "../../utils/periodograms.hpp"
#include "../../utils/grid.hpp"
#include "../../utils/readout.hpp"

inline void fastsum(float *x, std::complex<float> *y, float *f, std::complex<float> *out, unsigned int n, unsigned int nk){
    finufft_opts* opts = new finufft_opts;
    finufftf_default_opts(opts);
    int ier = finufftf1d3(n, &x[0], &y[0], +1, 1e-5, nk, &f[0], &out[0], opts);
return;}

void nfftls_s(const star &data, grid &grid, double* p) {

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

   float wsum=0, Y=0, YY=0;
   float *ts =  (float *) malloc(n * sizeof(float)),
         *SC =  (float *) malloc(nk * sizeof(float)),
         *S2w =  (float *) malloc(nk * sizeof(float)),
         *C2w =  (float *) malloc(nk * sizeof(float)),
         *Sw =  (float *) malloc(nk * sizeof(float)),
         *Cw =  (float *) malloc(nk * sizeof(float)),
         *YC =  (float *) malloc(nk * sizeof(float)),
         *YS =  (float *) malloc(nk * sizeof(float)),
         *CC =  (float *) malloc(nk * sizeof(float)),
         *SS =  (float *) malloc(nk * sizeof(float));

   std::complex<float> *wy = (std::complex<float> *) malloc(n * sizeof(std::complex<float>)),
                       *w =  (std::complex<float> *) malloc(n * sizeof(std::complex<float>)),
                       *Sh_Ch = (std::complex<float> *) malloc (nk * sizeof(std::complex<float>)),
                       *S2_C2 = (std::complex<float> *) malloc (nk * sizeof(std::complex<float>));


   for (unsigned int i=0; i<n; ++i) {
      /* weights */
      w[i] = 1 / (data.dy[i] * data.dy[i]);
      wsum += w[i].real();
      ts[i] = 2 * M_PI * data.x[i];
   }
   for (unsigned int i=0; i<n; ++i) {
      /* mean */
      w[i] /= wsum;                 /* normalised weights */
      Y += w[i].real() * data.y[i];             /* Eq. (7) */
   }
   for (unsigned int i=0; i<n; ++i) {
      /* variance */
      wy[i] = data.y[i] - Y;             /* Subtract weighted mean */
      YY += w[i].real() * wy[i].real() * wy[i].real();   /* Eq. (10) */
      wy[i] *= w[i].real();                /* attach weights */
   }

   fastsum(ts, wy, grid.freq.data(), Sh_Ch, n, nk);

   for (unsigned int i=0; i<n; ++i) {ts[i] *= 2;}

   fastsum(ts, w, grid.freq.data(), S2_C2, n, nk);

    for (unsigned int i=0; i<nk; ++i){
        SC[i] = S2_C2[i].imag() / S2_C2[i].real();
        S2w[i] = SC[i] / sqrt(1 + (SC[i] * SC[i]));
        C2w[i] = 1 / sqrt(1 + (SC[i] * SC[i]));
        Cw[i] = M_SQRT1_2 * sqrt(1 + C2w[i]);
        Sw[i] = M_SQRT1_2 * std::copysignf(1.0, S2w[i]) * sqrt(1 - C2w[i]);

        YC[i] = (Sh_Ch[i].real() * Cw[i]) + (Sh_Ch[i].imag() * Sw[i]);
        YS[i] = (Sh_Ch[i].imag() * Cw[i]) - (Sh_Ch[i].real() * Sw[i]);

        CC[i] = 0.5 * ((1 + S2_C2[i].real() * S2_C2[i].real()) + (S2_C2[i].imag() * S2_C2[i].imag()));
        SS[i] = 0.5 * ((1 + S2_C2[i].real() * S2_C2[i].real()) - (S2_C2[i].imag() * S2_C2[i].imag()));

        p[i] =  ((YC[i] * YC[i] / CC[i]) + (YS[i] * YS[i] / SS[i])) / YY;
    } // tan_2omega_tau









    return;}





