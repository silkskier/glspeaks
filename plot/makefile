all:
	go build -o glspeaksplot ./source #-compiler=gccgo -gccgoflags="-static -Ofast -m64 -msse -msse2 -mavx -mavx2"

install:
	cp -t /usr/local/bin glspeaksplot

uninstall:
	rm -f /usr/local/bin/glspeaksplot

.PHONY: all install uninstall
