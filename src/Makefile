all: dseq

CFLAGS = -g -O0 -std=gnu99 -Wall

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

uninstall:
	-rm $(DESTDIR)$(bindir)/dseq

.PHONY: all clean install uninstall
