#include <cmath>
#include <algorithm>
#include "../../utils/vertex.hpp"
#include "../../utils/periodograms.hpp"
#include "../../utils/grid.hpp"
#include "../../utils/readout.hpp"

output_data ce_b(const star &data, const grid &grid) {
        uint normalization = 0; //comment after implementing switch statement
        output_data best_frequency;

    float wsum=0, wysum=0, y_avg = 0, AAD=0, wsum_inv = 0, tmp;

     float *ts = (float *) malloc(data.x.size() * sizeof(float)), //single precision float representation of time
         *wy = (float *) malloc(data.x.size() * sizeof(float));
    uint i, j, k;

    float max_ent = log2(5*10);

    for (i=0; i<data.x.size(); ++i) {
       ts[i] = float(data.x[i]);
       y_avg += data.y[i];
    }

    float size_inv = 1 / data.x.size();

    y_avg *= size_inv;

    for (i=0; i<data.x.size(); ++i) {wy[i] = data.y[i] - y_avg;}

    float* y_min = std::min_element(wy, wy + data.x.size());
    float* y_max = std::max_element(wy, wy + data.x.size());
    float range = 5 / (y_max - y_min);



    float bins[10][5];
    int idx_1, idx_2;
    for (k=0; k<grid.freq.size(); ++k) {
         for (i=0; i<10; ++i) { for (j=0; j<10; j++) {bins[i][j] = 0;}}

        for (i=0; i<data.x.size(); ++i) {
            idx = std::min(int(10 * ((data.x[i] * grid.freq[k]) - float(int((data.x[i] * grid.freq[k]))))), 9);
            bins[idx_1][idx_2] += wy[i];
            }

        if (bins[0] > 0) {max = bins[0];}
            else {min = bins[0];}

        for (i=1; i<32; i++){
            bins[i] += bins[i - 1];
            if (bins[i] > max) {max = bins[i];}
                else if (bins[i] < min) {min = bins[i];}
        }

        power = max - min;

            //update output data struct
        if (power > best_frequency.power){
            best_frequency.frequency = grid.freq[k];
            best_frequency.amplitude = 1;
            best_frequency.power = power;
            }
        }


    best_frequency.sum_of_powers = grid.freq.size();
    best_frequency.power *= 25; //?!?!? - wyniki się nie zgadzają, ~20x za mała wartość 'power'

free(ts); free(w); free(wy);
return best_frequency;}
