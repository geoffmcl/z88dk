# z80asm restart
# Copyright (c) Paulo Custodio, 2020
# License: http://www.perlfoundation.org/artistic_license_2_0

all: z80asm.md z80asm-future.md z80asm.html z80asm-future.html

.SUFFIXES: .md .in .html

.in.md:
	perl make_index.pl < $< > $@.tmp
	mv -f $@.tmp $@

.md.html:
	@echo use grip from https://github.com/joeyespo/grip
	grip $< --export $@

z80asm.md z80asm-future.md: Makefile make_index.pl
