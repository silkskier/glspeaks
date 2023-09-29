#include <cmath>
#include <complex>

//includes for finufft
#include <finufft.h>

#include "../../utils/periodograms.hpp"
#include "../../utils/grid.hpp"
#include "../../utils/readout.hpp"

   void nfftls_s(const star &data, const Grid &grid, double* p) {

   finufft_opts* opts = new finufft_opts;
   finufftf_default_opts(opts);
   opts->nthreads = 1;
   opts->modeord = 1;

   int nk = grid.freq.size();
   int n = data.x.size();

   int dk = grid.freq[nk-1]/grid.fstep;
   int shift = grid.freq[0]/grid.fstep;

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

   float wsum=0, xspan, Y=0, YY=0;
   float *ts =  (float *) malloc(n * sizeof(float));

   std::complex<float> *wy = (std::complex<float> *) malloc(n * sizeof(std::complex<float>)),
                       *w =  (std::complex<float> *) malloc(n * sizeof(std::complex<float>)),
                       *Sh_Ch = (std::complex<float> *) malloc ((((2 * dk) + 1) * sizeof(std::complex<float>))),
                       *S2_C2 = (std::complex<float> *) malloc ((((2 * dk) + 1) * sizeof(std::complex<float>)));

   float SC, S2w, C2w, Cw, Sw, YC, YS, CC, SS, power;

   xspan = data.x[n-1] - data.x[0];

   for (unsigned int i=0; i<n; ++i) {
      /* weights */
      w[i] = 1 / (data.dy[i] * data.dy[i]);
      wsum += w[i].real();
      ts[i] = 0.00048828125 * M_PI * (data.x[i] - (data.x[0] + (xspan/2))); // 0.00048828125 - 2^-11
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


   //zero-mean


   //fastsum_b(ts, wy, Sh_Ch, n, dk, shift, opts);
   int ier = finufftf1d1(n, &ts[0], &wy[0], +1, 1e-4, 2 * dk, &Sh_Ch[0], opts);
   for (unsigned int i=0; i<n; ++i) {ts[i] *= 2;}
   //fastsum_b(ts, w,  S2_C2, n, dk, shift, opts);
   ier = finufftf1d1(n, &ts[0], &w[0], +1, 1e-4, 2 * dk, &S2_C2[0], opts);


      for (unsigned int i=shift; i<dk; ++i){
        SC = S2_C2[i].imag() / S2_C2[i].real();
        S2w = SC / sqrt(1 + (SC * SC));
        C2w = 1 / sqrt(1 + (SC * SC));
        Cw = M_SQRT1_2 * sqrt(1 + C2w);
        Sw = M_SQRT1_2 * std::copysignf(1.0, S2w) * sqrt(1 - C2w);

        YC = (Sh_Ch[i].real() * Cw) + (Sh_Ch[i].imag() * Sw);
        YS = (Sh_Ch[i].imag() * Cw) - (Sh_Ch[i].real() * Sw);

        CC = 0.5 * ((1 + S2_C2[i].real() * S2_C2[i].real()) + (S2_C2[i].imag() * S2_C2[i].imag()));
        SS = 0.5 * ((1 + S2_C2[i].real() * S2_C2[i].real()) - (S2_C2[i].imag() * S2_C2[i].imag()));

        p[i-shift] =  ((YC * YC / CC) + (YS * YS / SS)) / YY;
      }
      //SC - tan_2omega_tau

    free(ts); free(w); free(wy); free(Sh_Ch); free(S2_C2);
    delete opts;

    return;}
