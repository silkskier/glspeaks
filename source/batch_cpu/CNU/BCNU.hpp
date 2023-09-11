#include <cmath>
#include "../../extras/vertex.hpp"

output_data bncu_b(const star &data, const grid &grid) {
        uint normalization = 0; //comment after implementing switch statement
        output_data best_frequency;



    float wsum=0, wysum=0, y_avg = 0, AAD=0, wsum_inv = 0, tmp;

     float *ts = (float *) malloc(data.x.size() * sizeof(float)), //single precision float representation of time
         *w = (float *) malloc(data.x.size() * sizeof(float)),
         *wy = (float *) malloc(data.x.size() * sizeof(float)),
    unsigned int i, k;


    for (i=0; i<data.x.size(); ++i) {
       ts[i] = float(data.x[i]);
       w[i] = 1 / (data.dy[i] * data.dy[i]);
       y_avg += data.y[i];
    }

    y_avg /= data.x.size();



    switch (normalization) {

        case 0: //simple linear

            for (i=0; i<data.x.size(); ++i) {
                wy[i] = y[i] - y_avg;
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
                wy[i] = (y[i] - y_avg) * w[i];
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
                wy[i] = y[i] - y_avg;
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
                wy[i] = (y[i] - y_avg) * fabs(y[i] - y_avg) * w[i];
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
    */



    }
