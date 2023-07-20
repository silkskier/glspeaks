# glspeaks
Repository under construction.

1.0 release planned for August 2023, currently development is on hold.

A simple [GLS](https://github.com/mzechmeister/GLS) periodogram for detection of periodic variable stats candidates. Possibly useful for other periodic brightness change detections (like search for rotation period in asteroids), however not tested for these purposes.

Application based on [fnpeaks](http://helas.astro.uni.wroc.pl/deliverables.php?active=fnpeaks), with the goal of increasing performance, user-friendliness, and accuracy over its predecessor.

### Recommended update/installation command for version alpha 1.0.4:
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
rm -r ~/glspeaks
```
##### For version 1.0.0 and later main package will switch to GNU Guix (a distribution-agnostic package manager). [PPA for x86-64 Debian-based systems](https://silkskier.github.io/ppa/), as well as static executable with installer script for incompatible distros and FreeBSD, will be available as an alternative. Windows binary currently does not hold a high priority due to performance loss in comparison to running Linux binary with WSL2 as well, as simple compilation failure (caused by std::thread) happening during attempted compilation with MinGW-w64.

ICPX (Intel oneAPI C++ Compiler) is a recommended compiler choice due to measured significant performance advantage over alternatives, however, g++ with O3 optimization level can be used to reduce output binary size from ~ 500 kB to less than 200 kB at a cost of ~15% increase of execution time.

The program is designed to work with data stored on flash memory - on HDDs accessing data 'all over the place' due to the usage of OpenMP to parse the input may cause significant overhead. Implementation of buffering algorithm for HDD drives is possible, however, currently, its implementation is not planned due to added complication of the project and limited impact for possible real-life implementations.

As for now, the application is *almost* fully functional. Planned improvements include [NFFT3](https://github.com/NFFT/nfft)-based algorithm available as an alternative for current batch mode for 1.1 release (at least 3-4x speed increase is expected for high-frequency variable stars candidate searches), binary file input option (in NumPy format) as alternative input data for 1.2 release, as well as possible SyCL GPGPU implementation for 1.3 update (planned to be fully bottlenecked by CPU during data parsing, needs to solve the issue of limited static GPU memory, to be implemented efficiently).
