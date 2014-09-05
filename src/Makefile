# src/Makefile.  Generated from Makefile.in by configure.

package 	= dseq
version		= 0.1
tarname		= dseq
distdir		= $(tarname)-$(version)

prefix		= /usr/local
exec_prefix	= ${prefix}
bindir		= ${exec_prefix}/bin

srcdir		= @srcdir@
VPATH		= @srcdir@

all: dseq

CFLAGS = -g -O0 -std=gnu99 -Wall -Wsign-conversion

dseq: dseq.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ dseq.c

clean:
	-rm dseq

install:
	install -d $(DESTDIR)$(bindir)
	install -m 0755 dseq $(DESTDIR)$(bindir)
	#cp dseq /usr/local/bin
	#chown root:root /usr/local/bin/dseq
	#chmod +x /usr/local/bin/dseq

$(distdir): FORCE
		mkdir -p $(distdir)/src
		cp $(srcdir)/configure.ac $(distdir)
		cp $(srcdir)/configure $(distdir)
		cp $(srcdir)/Makefile.in $(distdir)
		cp $(srcdir)/src/Makefile.in $(distdir)
		cp $(srcdir)/src/dseq.c $(distdir)

uninstall:
	-rm $(DESTDIR)$(bindir)/dseq

Makefile: Makefile.in ../config.status
		cd .. && ./config.status src/$@

../config.status: ../configure
		cd .. && ./config.status --recheck

.PHONY: all clean install uninstall
