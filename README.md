# glspeaks
Repository under construction.

1.0 release is planned for July 2023, currently development is on hold.

### Recommended update/installation command for version alpha 1.0.4:
##### No releases are currently available. Compilation from the source is recommended.
<!--```bash
wget https://github.com/silkskier/glspeaks/releases/download/v1.0.4-alpha/glspeaks-alpha-1.0.4_installer.sh && chmod +x ./glspeaks-alpha-1.0.4_installer.sh && ./glspeaks-alpha-1.0.4_installer.sh
```-->
```bash
git clone https://github.com/silkskier/glspeaks
cd glspeaks
qmake
make
sudo make install
rm -r ~/glspeaks
```
##### For version 1.0.0 and later main package will switch to GNU Guix (a distribution-agnostic package manager). [PPA for Debian-based systems](https://silkskier.github.io/ppa/), as well as static executable with installer script for incompatible distros and FreeBSD, will be available as an alternative. Windows binary currently does not hold a high priority due to performance loss in comparison to running Linux binary with WSL2 as well, as simple compilation failure (caused by std::thread) happening during attempted compilation with MinGW-w64.
