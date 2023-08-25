#include <math.h>
#include <stdlib.h>
#include "trig_sum.h"
//http://www.fftw.org/download.html
//https://www-user.tu-chemnitz.de/~potts/nfft/download.php

//source; https://www.researchgate.net/publication/258561369_Fast_calculation_of_the_Lomb-Scargle_periodogram_using_nonequispaced_fast_Fourier_transforms#pf9

/* Computes the Lomb- Scargle normalised periodogram of a times-series.


t the times, reduced to [ -1/2,1/2).
y the measurements, centred around <y>.
npts the length of the times -series
over the oversampling factor.
hifac the highest frequency in units of "average " Nyquist frequency.
This function returns the results in a structure, LS (see text ). */

LS* periodogram(const double* t, const double* y, int npts, double over, double hifac) {
    double df = 1.0 / (over * (t[npts - 1] - t[0]));

    // Index of the highest frequency in the positive frequency part of spectrum.
    int m = floor(0.5 * npts * over * hifac);
    LS* ls = malloc(sizeof(LS));
    ls->freqs = malloc(m * sizeof(double));
    ls->Pn = malloc(m * sizeof(double));
    ls->nfreqs = m;

// Unnormalised FTs of the data and window
    double complex* sp = malloc((m + 1) * sizeof(double complex));
    nfft(t, y, npts, m, sp);
    double complex* win = malloc((2 * m + 1) * sizeof(double complex));
    nfft(t, NULL, npts, 2 * m, win);

    // Computes the periodogram ordinates ,
    // and store the results in the LS structure.

    for (int j = 1; j <= m; j ++) {
        double complex z1 = sp[j]; // FT of data at \omega
        double complex z2 = win[2 * j]; // FT of window at 2\omega
        double absz2 = cabs (z2 );
        double hc2wt = 0.5 * cimag(z2) / absz2;
        double hs2wt = 0.5 * creal(z2) / absz2;
        double cwt = sqrt(0.5 + hc2wt);
        double swt = sign(sqrt (0.5 - hc2wt), hs2wt);
        double den = 0.5 * npts + hc2wt * creal(z2) + hs2wt * cimag(z2);
        double cterm = (cwt * creal(z1) + swt * cimag(z1)) * (cwt * creal(z1) + swt * cimag(z1)) / den;
        double sterm = (cwt * cimag(z1) - swt * creal(z1)) * (cwt * cimag(z1) - swt * creal(z1)) / (npts - den);
        ls->freqs[j - 1] = (j - 1) * df;
        ls->Pn[j - 1] = (cterm + sterm) / (2 * var);
    }

    free (win);
    free (sp);
    return ls;
}
