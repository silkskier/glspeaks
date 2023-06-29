# glspeaks
Repository under construction.

1.0 release is planned for July 2023, currently development is on hold.

### Recommended update/installation command for version alpha 1.0.4:
##### No releases are currently available. Currently, compilation from source is recommended.
<!--```bash
wget https://github.com/silkskier/glspeaks/releases/download/v1.0.4-alpha/glspeaks-alpha-1.0.4_installer.sh && chmod +x ./glspeaks-alpha-1.0.4_installer.sh && ./glspeaks-alpha-1.0.4_installer.sh
```-->
```bash
git clone https://github.com/silkskier/glspeaks
cd glspeaks
qmake
make
sudo make install
rm -r /glspeaks
```
##### For versions beta 1.0.0 and later packaging will switch to GNU Guix (a distribution-agnostic package manager).
