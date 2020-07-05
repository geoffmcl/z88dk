# z80asm restart
# Copyright (c) Paulo Custodio, 2020
# License: http://www.perlfoundation.org/artistic_license_2_0

all: 	z80asm.md   z80asm-future.md \
		z80asm.html z80asm-future.html \
		z80asm.txt  z80asm-future.txt \
		z80asm_manual.c z80asm_manual.h \
		z80asm_usage.c  z80asm_usage.h

.SUFFIXES: .md .in .html .txt

.in.md:
	perl make_index.pl < $< > $@.tmp
	mv -f $@.tmp $@

.md.html:
	@echo use grip from https://github.com/joeyespo/grip
	grip $< --export $@

.html.txt:
	@echo use elinks to render html
	elinks -dump $< > $@

z80asm.md z80asm-future.md: Makefile make_index.pl

z80asm_manual.c z80asm_manual.h: Makefile make_c.pl z80asm.txt
	perl make_c.pl -n z80asm_manual z80asm.txt

z80asm_usage.c z80asm_usage.h: Makefile make_c.pl usage.txt
	perl make_c.pl -n z80asm_usage usage.txt

