#ifndef NFFT_HPP
#define NFFT_HPP

#include <cmath>
#include <complex>

#include <nfft3.h>


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
