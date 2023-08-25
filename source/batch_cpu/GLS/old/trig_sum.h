#include <complex.h>
#include <math.h>
#include <nfft3.h>

/* Computation of the positive frequency
* part of the (unnormalised ) Fourier
* transform of a times - series (t, y).

Input:
* t the times reduced to [1/2, 1/2)
* y the measurements (NULL , for computing the FT of the window)
* n the number of measurements
* m the number of positive frequencies

* Output:
* d the Fourier coefficients
* (preallocated array for (m+1) elements)
*/

typedef struct {
    double* freqs; // (>0) frequencies
    double* Pn; // periodogram ordinates
    int nfreqs; // number of frequencies
} LS;


void nnft (const double* t, const double* y, int n, int m, double complex* d) {
    // Creates NFFT plan for 2*m Fourier
    // coefficients (positive and negative
    // frequencies) and n data samples.
    nfft_plan p;
    nfft_init_1d(&p, 2 * m, n);

    if (y != NULL) { // data spectrum
        for (int i = 0; i < n; i++) {
            p.x[i] = t[i];
            p.f[i] = y[i];
        }
    } else { // window spectrum
        for (int i = 0; i < n; i++) {
            p.x[i] = t[i];
            p.f[i] = 1.0;
        }
    }

    // Possibly optimizes.
    if (p.fftw_flags & PRE_ONE_PSI) nfft_precompute_one_psi(&p);

    // Computes the adjoint transform.
    nfft_adjoint(&p);

    // Outputs the positive frequency Fourier coefficients.
    for (int i = 0; i < m; i++)
        d[i] = p.f_hat[i];
    d[m] = conj(p.f_hat[0]);

    nfft_finalize(&p);
}


/*
The undisplayed header file declar.h (line 3) contains the declarations of the function nfft as well as of the auxiliary functions

sign (line 48), which transfers the sign of itssecond argument to its first argument, i.e. sign(a, b) returns |a|sgnb,where sgn(x) yields the sign of x   ;

square (lines 50−51), which returns the square of its argument;

xmalloc (lines 26–28, 32, 35), which is a wrapper of the standard function malloc that terminates the program with a message in the case of an error.
*/
