# Makefile.  Generated from Makefile.in by configure.

package 	= dseq
version		= 0.1
tarname		= dseq
distdir		= $(tarname)-$(version)

prefix		= /usr/local
exec_prefix	= ${prefix}
bindir		= ${exec_prefix}/bin

# package = dseq
# version = 0.1
# tarname = $(package)
# distdir = $(tarname)-$(version)
# prefix  = /usr/local
# exec_prefix = $(prefix)
# bindir = $(exec_prefix)/bin

# export prefix
# export exec_prefix
# export bindir

all clean install uninstall dseq:
	cd src && $(MAKE) $@

dist: $(distdir).tar.gz

distcheck: $(distdir).tar.gz
	gzip -cd $(distdir).tar.gz | tar xvf -
	cd $(distdir) && ./configure
	cd $(distdir) && $(MAKE) all
	cd $(distdir) && $(MAKE) DESTDIR=$${PWD}/_inst install
	cd $(distdir) && $(MAKE) DESTDIR=$${PWD}/_inst uninstall
	cd $(distdir) && $(MAKE) clean
	rm -rf $(distdir)
	@echo "*** Package $(distdir).tar.gz is ready for distribution."

$(distdir).tar.gz:	$(distdir)
	tar chof - $(distdir) | gzip -9 -c > $@
	rm -rf $(distdir)

$(distdir): FORCE
	mkdir -p $(distdir)/src
	cp configure.ac $(distdir)
	cp configure $(distdir)
	cp Makefile.in $(distdir)
	cp src/Makefile.in $(distdir)/src
	cp src/dseq.c $(distdir)/src

Makefile: Makefile.in config.status
		./config.status $@

config.status: configure
		./config.status --recheck

FORCE:
	-rm $(distdir).tar.gz >/dev/null 2>&1
	-rm -rf $(distdir) >/dev/null 2>&1

.PHONY: FORCE all clean dist distcheck install uninstall
