#include <cmath>
#include <algorithm>
#include "../../utils/vertex.hpp"
#include "../../utils/periodograms.hpp"
#include "../../utils/grid.hpp"
#include "../../utils/readout.hpp"

void ce_s(const star &data, const Grid &grid, double* powers) {
        uint normalization = 0; //comment after implementing switch statement


    float wsum=0, wysum=0, y_avg = 0, AAD=0, wsum_inv = 0, tmp;

     float *ts = (float *) malloc(data.x.size() * sizeof(float)), //single precision float representation of time
         *wy = (float *) malloc(data.x.size() * sizeof(float));
    uint i, j, k;

    float max_ce = log2(5*10);

    for (i=0; i<data.x.size(); ++i) {
       ts[i] = float(data.x[i]);
       y_avg += data.y[i];
    }

    float size_inv = 1 / data.x.size();

    y_avg *= size_inv;

    float weight = size_inv;

    for (i=0; i<data.x.size(); ++i) {wy[i] = data.y[i] - y_avg;}

    auto min_max_pair = std::minmax_element(wy, wy + data.x.size());
    float y_min = *min_max_pair.first;
    float y_max = *min_max_pair.second;

    float range = 5 / (y_max - y_min); //WTF? Somehow y_min is greater, than y_max

    for (i=0; i<data.x.size(); ++i) {wy[i] -= y_min; wy[i] *= range;}





    float bins[10][5];
    int idx_1, idx_2;
    for (k=0; k<grid.freq.size(); ++k) {
         for (i=0; i<10; ++i) { for (j=0; j<5; j++) {bins[i][j] = 0;}}

        for (i=0; i<data.x.size(); ++i) {
            idx_1 = std::min(int(10 * ((ts[i] * grid.freq[k]) - float(int((ts[i] * grid.freq[k]))))), 9);
            idx_2 = std::min(int(wy[i]), 4);
            bins[idx_1][idx_2] += 1;
            }

        float A = 0;

        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 5; ++j) {
                if (bins[i][j] > 0) {
                    float p_x_y = bins[i][j] / data.x.size();
                    float p_y = 0;
                    float sum_row = 0;

                    for (int jj = 0; jj < 5; ++jj) {
                        sum_row += bins[i][jj];
                    }

                    if (sum_row > 0) {
                        p_y = sum_row / data.x.size();
                    }

                    A += p_x_y * log2(p_y / p_x_y);

                }
            }
        }

        float power = (max_ce - A) / max_ce;



        powers[k] = power;
        }


free(ts); free(wy);
return;}
