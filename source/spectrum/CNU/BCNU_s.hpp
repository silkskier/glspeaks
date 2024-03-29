#include <cmath>

#include "../../utils/vertex.hpp"
#include "../../utils/periodograms.hpp"
#include "../../utils/grid.hpp"
#include "../../utils/readout.hpp"

void bcnu_s(const star &data, const Grid &grid, double* powers) {
        uint normalization = 0; //comment after implementing switch statement



    float wsum=0, wysum=0, y_avg = 0, AAD=0, wsum_inv = 0, tmp;

     float *ts = (float *) malloc(data.x.size() * sizeof(float)), //single precision float representation of time
         *w = (float *) malloc(data.x.size() * sizeof(float)),
         *wy = (float *) malloc(data.x.size() * sizeof(float));
    uint i, k;


    for (i=0; i<data.x.size(); ++i) {
       ts[i] = float(data.x[i]);
       w[i] = 1 / (data.dy[i] * data.dy[i]);
       y_avg += data.y[i];
    }

    y_avg /= data.x.size();



    switch (normalization) {

        case 0: //simple linear

            for (i=0; i<data.x.size(); ++i) {
                wy[i] = data.y[i] - y_avg;
                AAD += fabs(wy[i]);
            }

            wsum_inv = 2/AAD; //because sum of normalised Kuiper before weighting lies between 0 and 1/2
            AAD /= data.x.size();

            for (i=0; i<data.x.size(); ++i) {
                wy[i] *= wsum_inv;
            }

            break;

        case 1: //weighted linear

            for (i=0; i<data.x.size(); ++i) {
                wy[i] = (data.y[i] - y_avg) * w[i];
                wsum += wy[i];
            }
            wsum /= data.x.size();
            for (i=0; i<data.x.size(); ++i) {
                wy[i] -= wsum;
                AAD += fabs(wy[i]);
            }
            wsum_inv = 2/AAD;
            AAD /= data.x.size(); //idk what to do with that, so I just leave it here.

            for (i=0; i<data.x.size(); ++i) {
                wy[i] *= wsum_inv;
            }

            break;

        case 2: //simple quadratic

            for (i=0; i<data.x.size(); ++i) {
                wy[i] = data.y[i] - y_avg;
                wy[i] = wy[i] * fabs(wy[i]);
                AAD += fabs(wy[i]);
            }

            wsum_inv = 2/AAD; //because sum of normalised Kuiper before weighting lies between 0 and 1/2

            for (i=0; i<data.x.size(); ++i) {
                wy[i] *= wsum_inv;
            }

            break;

        case 3: //weighted quadratic
            for (i=0; i<data.x.size(); ++i) {
                wy[i] = (data.y[i] - y_avg) * fabs(data.y[i] - y_avg) * w[i];
                wsum += wy[i];
            }
            wsum /= data.x.size();
            for (i=0; i<data.x.size(); ++i) {
                wy[i] -= wsum;
                AAD += fabs(wy[i]);
            }
            wsum_inv = 2/AAD;
            AAD /= data.x.size(); //idk what to do with that, so I just leave it here.

            for (i=0; i<data.x.size(); ++i) {
                wy[i] *= wsum_inv;
            }

            break;

        default:
            std::cout << "Invalid choice of normalization" << std::endl;
    }


    float bins[32];
    int idx;
    for (k=0; k<grid.freq.size(); ++k) {
         for (i=0; i<32; ++i) {bins[i] = 0;}

        float min = 0, max = 0, power = 0;
        for (i=0; i<data.x.size(); ++i) {
            idx = std::min(int(32 * ((data.x[i] * grid.freq[k]) - float(int((data.x[i] * grid.freq[k]))))), 31);
            bins[idx] += wy[i];
            }

        if (bins[0] > 0) {max = bins[0];}
            else {min = bins[0];}

        for (i=1; i<32; i++){
            bins[i] += bins[i - 1];
            if (bins[i] > max) {max = bins[i];}
                else if (bins[i] < min) {min = bins[i];}
        }

        power = max - min;
        powers[k] = power;

        }

free(ts); free(w); free(wy);
return;}
