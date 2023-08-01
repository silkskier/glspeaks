# glspeaks
Repository under construction.

1.0 release planned for August 2023, currently development is on hold.

A simple [GLS](https://github.com/mzechmeister/GLS) periodogram for detection of periodic variable stats candidates. Possibly useful for other periodic brightness change detections (like search for rotation period in asteroids), however not tested for these purposes.

Application based on [fnpeaks](http://helas.astro.uni.wroc.pl/deliverables.php?active=fnpeaks), with the goal of increasing performance, user-friendliness, and accuracy over its predecessor.

### Recommended update/installation command for version beta 1.0.3:
##### No releases are currently available. Compilation from the source is recommended.
<!--```bash
wget https://github.com/silkskier/glspeaks/releases/download/v1.0.4-alpha/glspeaks-alpha-1.0.4_installer.sh && chmod +x ./glspeaks-alpha-1.0.4_installer.sh && ./glspeaks-alpha-1.0.4_installer.sh
```-->
```bash
git clone https://github.com/silkskier/glspeaks
cd glspeaks
cmake .
make
sudo make install
cd plots
make
sudo make install
rm -r ~/glspeaks
```
##### For version 1.0.0 and later [PPA for x86-64 Debian-based systems](https://silkskier.github.io/ppa/), as well as static executable with installer script for incompatible distros and FreeBSD, will be available as an alternative. Windows binary currently does not hold a high priority due to performance loss in comparison to running Linux binary with WSL2 as well, as simple compilation failure (caused by std::thread) happening during attempted compilation with MinGW-w64.

ICPX (Intel oneAPI C++ Compiler) is a recommended compiler choice due to measured significant performance advantage over alternatives, however, g++ with O3 optimization level can be used to reduce output binary size from ~ 500 kB to less than 200 kB at a cost of ~15% increase of execution time.

The program is designed to work with data stored on flash memory - on HDDs accessing data 'all over the place' due to the usage of OpenMP to parse the input may cause significant overhead. Implementation of buffering algorithm for HDD drives is possible, however, currently, its implementation is not planned due to added complication of the project and limited impact for possible real-life implementations.




As for now, the application is *almost* fully functional. Planned improvements include [NFFT3](https://github.com/NFFT/nfft) and [conditional entropy](https://academic.oup.com/mnras/article/434/3/2629/1044188)-based algorithms available as an alternative for current batch mode and SyCL GPGPU implementation for 1.3 update (planned to be fully bottlenecked by CPU during data parsing, needs to solve the issue of limited static GPU memory, to be implemented efficiently). The stock [FastChi](https://public.lanl.gov/palmer/fastchi.html) algorithm implementation is not planned due to its underwhelming performance (~3 stars/second*core on Ryzen 5 4600H) with precision inferior to CE. Modified implementation is possible at future release. The current performance of [recursive](https://academic.oup.com/mnras/article/213/4/773/951672) [GLS](https://arxiv.org/abs/0901.2573) is close to ~35 stars/second*core on the same hardware at the resolution set to 13 (which results in accuracy close to limitations of the algorithm).

#### Potential alternatives to this application;
1. [AstroPy](https://github.com/astropy/astropy) - Python Library containing alternative implementations of all periodograms used in this application as well, as many other tools.
2. [VStar]([https://www.aavso.org/vstar](https://github.com/AAVSO/VStar)https://github.com/AAVSO/VStar) - Alternative GUI OpenSource application allowing for batch searches.
3. [UPSILoN](https://github.com/dwkim78/upsilon/tree/master) - Automatic variable stars classifier based on the Random Forest model.
4. [Peransi](https://www.cbabelgium.com/peranso/index.html) - Proprietary GUI application allowing for analysis of variable stars' periodicity with many different periodograms included.
5. [FastChi$^{2}$](https://public.lanl.gov/palmer/fastchi.html) - Reasonably performant periodogram for batch searches (CLI only).
6. [PDM2](http://www.stellingwerf.com/rfs-bin/index.cgi?action=PageView&id=29) - Accurate low-performance periodogram.
7. [Plotypus](https://github.com/astroswego/plotypus) - Not maintained Python library useful for finding periods as well as lightcurve generation.
