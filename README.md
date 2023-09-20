# glspeaks
Repository under construction.

1.0 release planned for late 2023.

A simple [GLS](https://github.com/mzechmeister/GLS) periodogram for detection of periodic variable stats candidates. Possibly useful for other periodic brightness change detections (like search for rotation period in asteroids), however not tested for these purposes.

Application based on [fnpeaks](http://helas.astro.uni.wroc.pl/deliverables.php?active=fnpeaks), with the goal of increasing performance, user-friendliness, and accuracy over its predecessor.

/include subdirectory does contain copy of [niXman/yas](https://github.com/niXman/yas) serializer, [fastfloat/fast_float](https://github.com/fastfloat/fast_float) float parser, (orlp/pdqsort)[https://github.com/orlp/pdqsort] sorting library, (jarro2783/cxxopts)[https://github.com/jarro2783/cxxopts] CLI arguments parser and a header-only variant of [DarkoVeberic/LambertW](https://github.com/DarkoVeberic/LambertW).

### Recommended update/installation command for version beta 1.0.3:
##### No releases are currently available. Compilation from the source is recommended.
<!--```bash
wget https://github.com/silkskier/glspeaks/releases/download/v1.0.4-alpha/glspeaks-alpha-1.0.4_installer.sh && chmod +x ./glspeaks-alpha-1.0.4_installer.sh && ./glspeaks-alpha-1.0.4_installer.sh
```-->
```bash
git clone --depth=1 https://github.com/silkskier/glspeaks
cd glspeaks
cmake .
make
sudo make install
cd plots
make
sudo make install
rm -r ~/glspeaks
```
##### For version 1.0.0 and later [PPA for x86-64 Debian-based systems](https://silkskier.github.io/ppa/) will be available.

ICPX (Intel oneAPI C++ Compiler) is a recommended compiler choice due to measured significant performance advantage over alternatives, however, g++ with O3 optimization level can be used to reduce output binary size from ~ 500 kB to less than 200 kB at a cost of ~5% increase of execution time.

The program is designed to work with data stored on flash memory - on HDDs accessing data 'all over the place' due to the usage of OpenMP to parse the input may cause significant overhead.


As for now, the application is *almost* fully functional. Planned improvements include [NFFT3](https://github.com/NFFT/nfft) and [conditional entropy](https://academic.oup.com/mnras/article/434/3/2629/1044188)-based algorithms available as an alternative for current batch mode and SyCL GPGPU implementation for 1.3 update (planned to be fully bottlenecked by CPU during data parsing, needs to solve the issue of limited static GPU memory, to be implemented efficiently). The stock [FastChi](https://public.lanl.gov/palmer/fastchi.html) algorithm implementation is not planned due to its underwhelming performance (~3 stars/second * core on Ryzen 5 4600H) with precision inferior to CE. Modified implementation is possible at future release. The current performance of [recursive](https://academic.oup.com/mnras/article/213/4/773/951672) [GLS](https://arxiv.org/abs/0901.2573) is close to ~75 stars/second*core on the same hardware at the resolution set to 13 (which results in accuracy close to limitations of the algorithm).

#### Potential alternatives to this application;
1. [AstroPy](https://github.com/astropy/astropy) - Python Library containing alternative implementations of all periodograms used in this application as well, as many other tools.
2. [VStar](https://github.com/AAVSO/VStar) - Alternative GUI OpenSource application allowing for batch searches.
3. [UPSILoN](https://github.com/dwkim78/upsilon/tree/master) - Automatic variable stars classifier based on the Random Forest model.
4. [Peransi](https://www.cbabelgium.com/peranso/index.html) - Proprietary GUI application allowing for analysis of variable stars' periodicity with many different periodograms included.
5. [FastChi$^{2}$](https://public.lanl.gov/palmer/fastchi.html) - Reasonably performant periodogram for batch searches (CLI only).
6. [PDM2](http://www.stellingwerf.com/rfs-bin/index.cgi?action=PageView&id=29) - Accurate low-performance periodogram.
7. [Plotypus](https://github.com/astroswego/plotypus) - Not maintained Python library useful for finding periods as well as lightcurve generation.

#### Inspired by (list does not include software and/or articles mentioned previously in alternatives);
1. [fnpeaks](http://helas.astro.uni.wroc.pl/deliverables.php?active=fnpeaks) - a simple recursive Lomb-Scargle periodogram (CLI only)
2. [AstroPy's periodograms' implementations](https://github.com/astropy/astropy/tree/662528102179cc93ab9e4b21a31b7cbc1d4c299f/astropy/timeseries/periodograms/lombscargle/implementations)
3. [nfft.nfft_adjoint function from nfft Python package](https://github.com/jakevdp/nfft)
4. [adjoint CUNFFT from CUNFFT library](https://github.com/sukunis/CUNFFT)
5. [NFFT3.jl package](https://github.com/NFFT/NFFT3.jl/)
6. [B. Leroy - Fast calculation of the Lomb-Scargle periodogram using NFFTs](https://www.researchgate.net/publication/258561369_Fast_calculation_of_the_Lomb-Scargle_periodogram_using_nonequispaced_fast_Fourier_transforms)
7. [jakevdp/nfftls Python package](https://github.com/jakevdp/nfftls)
8. [pynfftls Python package](https://pypi.org/project/pynfftls/#files)
9. [Matthew J. Graham at all - A comparison of period finding algorithms](https://arxiv.org/pdf/1307.2209.pdf)
10. [Deep Forward Thinking Random Forest ensemble learning method](https://arxiv.org/abs/1705.07366)
11. [randomForest Go machine learning library](https://github.com/malaschitz/randomForest)
12. [GoLearn machine learning library](https://github.com/sjwhitworth/golearn)
13. [Friedman, J. H. (1984) A variable span scatterplot smoother](https://www.slac.stanford.edu/pubs/slacpubs/3250/slac-pub-3477.pdf)
14. [supersmoother Python package](https://github.com/jakevdp/supersmoother)
15. [pdqsort library](https://github.com/orlp/pdqsort)
16. [VkFFT library](https://github.com/DTolm/VkFFT)
17. [Kaixi Hou at all - bb_segsort algoruthm](https://kaixih.github.io/assets/slides/ics'17-segsort-slides.pdf)
18. [FINUFFT](https://github.com/flatironinstitute/finufft)
19. [Sheng Shi, Runkai Yang and Haihang You - NFFT.jl](https://arxiv.org/abs/2208.00049)
20. [Haitham Hassanieh, Piotr Indyk, Dina Katabi, Eric Price - Nearly Optimal Sparse Fourier Transform](https://arxiv.org/abs/1201.2501v1)
21. [sFFT-4.0](http://www.scielo.org.co/scielo.php?script=sci_arttext&pid=S1794-91652015000200004)
22. [Sheng Shi, Runkai Yang, Haihang You - ATSFFT](https://arxiv.org/abs/1908.02461)
23. [GPU-accelerated supersmoother](https://github.com/mgowanlock/gpu_supersmoother)
24. [KuiperVnStatistic](https://github.com/GrAbsRD/KuiperVnStatistic)
25. [GPU accelerated Conditional Entropy periodogram](https://academic.oup.com/mnras/article/503/2/2665/6158396), [Github repo](https://github.com/mikekatz04/gce)
    

<!--  https://sourceforge.net/projects/sfft40/ - sFFT-4.0 library  -->
