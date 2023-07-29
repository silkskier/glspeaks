package main

import(
	"fmt"
	"os"
	"path/filepath"
)

type Parameters struct {
    gls_amplitude float64 //periodogram amplitude
    hl_amp_ratio float64 //higher than average to lower than average ratio
    kurtosis float64
    skewness float64
    K float64 //Stetson K index
    period float64
    phase_eta float64
    //quartile31 float64 //low 25% to high 75% ratio
    a21 float64 //ratio of amplitudes
    a31 float64 //ratio of amplitudes
    W float64 //https://en.wikipedia.org/wiki/Shapiro%E2%80%93Wilk_test
    N float64 //https://arxiv.org/pdf/1403.6131.pdf - von Neumann n

    slope_per10 float64 //10th percentile of slopes between adjacent mags after the light curve is folded on 2P
    slope_per90 float64 //90th percentile of slopes between adjacent mags after the light curve is folded on 2P
    //just phase photometry over 2P, and sort the array of magnitude changes between previous and next measurement. Then take ones from 1/10 and 9/10 indices. It's probably better to use the normalised lightcurve instead.

    norm_phase_cusum float64 // Difference between minimum and maximum of cumsum of magnitudes relative to average measured magnitude divided by number of measurements (UPSILoN - not normalised)

    //phi21 float64 //phaseshift from Fourier decomposition - (almost)useless - Dae-Won Kim at all, UPSILoN
    //phi31 float64 // //phaseshift from Fourier decomposition - (almost)useless - Dae-Won Kim at all, UPSILoN
    // https://arxiv.org/pdf/1403.6131.pdf


    lin2sin float64 // ratio of squared residuals of linear interpolation to sinusoidal fit. Both derived from smoothened lightcurve
    min_phase float64 // phase of brightness minimum on phased graph
    mes_pred_ch float64 //ratio between expected and predited sum of brightness changes
    p2_ch float64 //ratio between 2nd best frequency and fine-tuned period sums of changes
    p3_ch float64 //ratio between 3nd best frequency and fine-tuned period sums of changes

    top25_ratio float64 //ratio of measurements above 75% of distance between min and max brightness of smoothened lightcurve
    bottom25_ratio float64 //ratio of measurements below 25% of distance between min and max brightness of smoothened lightcurve

}

func listFiles(dirPath string) ([]string, error) {
	var files []string

	err := filepath.Walk(dirPath, func(path string, info os.FileInfo, err error) error {
		if err != nil {
			fmt.Println("Error accessing path:", path, err)
			return nil
		}

		if !info.IsDir() {
			files = append(files, path)
		}

		return nil
	})

	if err != nil {
		return nil, err
	}

	return files, nil
}
