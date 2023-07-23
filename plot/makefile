all:
	go build -o glspeaksplot main.go # -compiler=gccgo -gccgoflags="-O3"
install:
	cp -t /usr/local/bin glspeaksplot
uninstall:
	rm -f /usr/local/bin/glspeaksplot

.PHONY: all install uninstall
