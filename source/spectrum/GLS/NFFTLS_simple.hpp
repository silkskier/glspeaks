#include <cmath>
#include <complex>

//includes for finufft
#include <finufft.h>

#include "../utils/periodograms.hpp"
#include "../utils/grid.hpp"
#include "../utils/readout.hpp"

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
                       *Sh_Ch = (std::complex<float> *) malloc ((((2 * dk) + 1) * sizeof(std::complex<float>))),;

   float YC, YS;

   xspan = data.x[n-1] - data.x[0];

   for (unsigned int i=0; i<n; ++i) {
      /* weights */
      w[i] = 1;
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

      wy[i] *= w[i].real();               /* attach weights */
   }


   //zero-mean


   //fastsum_b(ts, wy, Sh_Ch, n, dk, shift, opts);
   int ier = finufftf1d1(n, &ts[0], &wy[0], +1, 1e-4, 2 * dk, &Sh_Ch[0], opts);
   for (unsigned int i=0; i<n; ++i) {ts[i] *= 2;}


      for (unsigned int i=shift; i<dk; ++i){
      //non-weighted implementation (?)
      YC = Sh_Ch[i].real();
      YS = Sh_Ch[i].imag();

    // Calculate the power at this frequency without weights
    p[i - shift] = 2 * ((YC * YC) + (YS * YS)) / (YY);
      }
      //SC - tan_2omega_tau

    free(ts); free(w); free(wy); free(Sh_Ch);
    delete opts;

    return;}
