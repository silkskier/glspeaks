all:
	go build -o glspeaksplot -compiler=gccgo -gccgoflags="-O3" main.go
install:
	cp -t /usr/local/bin glspeaksplot
uninstall:
	rm -f /usr/local/bin/glspeaksplot

.PHONY: all install uninstall
