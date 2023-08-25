#include <complex.h>
#include <math.h>
#include <nfft3.h>

template <typename T>
void trig_sum(const T* t, const T* y, int n, int m, std::complex<T>* d) {
    nfft_plan p;
    nfft_init_1d(&p, 2 * m, n);

        for (int i = 0; i < n; i++) {
            p.x[i] = t[i];
            p.f[i] = y[i];
        }

    // Possibly optimizes.
    nfft_precompute_one_psi(&p);

    // Computes the adjoint transform.
    nfft_adjoint(&p);

    // Outputs the positive frequency Fourier coefficients.
    for (int i = 0; i < m; i++)
        d[i] = p.f_hat[i];
    d[m] = conj(p.f_hat[0]);

    nfft_finalize(&p);

return;}
