#ifndef NFFT_HPP
#define NFFT_HPP

#include <cmath>
#include <complex>

#include <nfft3.h>

#include <cmath>
#include <complex>
#include <nfft3.h>

void trig_sum (){}

inline void simple_sum(const float* t, const float* y, int n, int Nf, fftw_complex* results){

    //fftw_complex* results = (fftw_complex*) fftw_malloc(Nf * sizeof(fftw_complex));
    // Convert Nf to m (number of frequencies)
    int m = 2 * Nf;

    // Create an NFFT plan
    nfft_plan plan;
    nfft_init_1d(&plan, m, n);

    // Fill in input data
    for (int i = 0; i < n; i++)
    {
        plan.x[i] = t[i];
        plan.f[i][0] = y[i];  // Real part of y[i]
        plan.f[i][1] = 0.0;   // Imaginary part set to 0
    }

    // Precompute if needed
    if (plan.flags & PRE_ONE_PSI)
        nfft_precompute_one_psi(&plan);

    // Compute the adjoint NFFT
    nfft_adjoint(&plan);

    // Copy results to the output array
    for (int i = 0; i < Nf; i++)
    {
        results[i][0] = plan.f_hat[i + Nf][0];  // Copy real part
        results[i][1] = plan.f_hat[i + Nf][1];  // Copy imaginary part
    }

    // Copy DC component (index 0)
    results[Nf][0] = plan.f_hat[0][0];
    results[Nf][1] = plan.f_hat[0][1];

    // Clean up the NFFT plan
    nfft_finalize(&plan);
}





void nfft(const double* t, const double* y, int n, int m, fftw_complex* d)
{
    nfft_plan p;
    nfft_init_1d(&p, 2 * m, n);

    if (y)      /* spectrum */
    {
        for (int i = 0; i < n; i++)
        {
            p.x[i] = t[i];
            p.f[i][0] = y[i]; // Assign real part of y[i]
            p.f[i][1] = 0.0;  // Imaginary part set to 0
        }
    }
    else        /* window */
    {
        for (int i = 0; i < n; i++)
        {
            p.x[i] = t[i];
            p.f[i][0] = 1.0;  // Set real part to 1.0
            p.f[i][1] = 0.0;  // Set imaginary part to 0.0
        }
    }

    if (p.flags & PRE_ONE_PSI)
        nfft_precompute_one_psi(&p);

    nfft_adjoint(&p);

    for (int i = 0; i < m; i++)
    {
        d[i][0] = p.f_hat[i + m][0]; // Copy real part
        d[i][1] = p.f_hat[i + m][1]; // Copy imaginary part
    }

    d[m][0] = p.f_hat[0][0];
    d[m][1] = p.f_hat[0][1];

    nfft_finalize(&p);
}

#endif // NFFT_HPP
