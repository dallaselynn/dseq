bin_PROGRAMS = dseq
dseq_SOURCES = dseq.c
AM_CPPFLAGS = -I$(top_srcdir)/lib -I$(top_builddir)/lib -Wall -Wextra -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wstrict-prototypes -Wpointer-arith -Wwrite-strings -Wcast-qual -Wcast-align -Wbad-function-cast -Wformat-security -Wmissing-format-attribute -Winline -funsigned-char -pedantic
localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@
LDADD = $(LIBINTL) $(top_builddir)/lib/libgnu.a
