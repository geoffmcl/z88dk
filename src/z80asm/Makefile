#------------------------------------------------------------------------------
# Z88DK Z80 Module Assembler
#
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

PROJ			:= z80asm

# EXESUFFIX is passed when cross-compiling Win32 on Linux
ifeq ($(OS),Windows_NT)
  EXESUFFIX 	:= .exe
else
  EXESUFFIX 	?=
endif

TARGET 			:= $(PROJ)$(EXESUFFIX)

#------------------------------------------------------------------------------
# UNIXem is needed in both Windows_NT and in a MinGW build in Unix
ifeq ($(EXESUFFIX),.exe)
  UNIXem_CFLAGS	:= -I../../ext/UNIXem/include
  UNIXem_OBJS	:=   ../../ext/UNIXem/src/glob.o \
				     ../../ext/UNIXem/src/dirent.o
endif

CC 				?= gcc
CXX				?= g++
INSTALL 		?= install
CROSS			?= 0

REFLEX			?= reflex --fast --noline
ASTYLE			?= astyle --project=.astylerc
CP				?= cp -f

#------------------------------------------------------------------------------
# Object files
#------------------------------------------------------------------------------
ALL_C_SRCS		:= $(wildcard \
						*.c \
						doc/*.c \
						lib/*.c \
						../common/*.c \
						../../ext/regex/reg*.c)
C_SRCS			:= $(filter-out test.c,$(ALL_C_SRCS))
CXX_SRCS		:= $(wildcard cpp/*.cpp)

OBJS			:= $(C_SRCS:.c=.o) $(CXX_SRCS:.cpp=.o) $(UNIXem_OBJS)
DEPENDS			:= $(C_SRCS:.c=.d) $(CXX_SRCS:.cpp=.d) $(UNIXem_OBJS:.o=.d)

TEST_SRC_MAIN	:= $(wildcard t/test_*.c)
TEST_SRC_LIB	:= t/testlib.c

#------------------------------------------------------------------------------
# compilation flags
#------------------------------------------------------------------------------

OPT 			= -O3

FLAGS			:= $(OPT) -MMD -g -Wall \
				   -I. \
				   -Idoc \
				   -Icpp \
				   -Ilib \
				   -I.. \
				   -I../common \
				   -I../../ext/RE-flex/include \
				   -I../../ext/optparse \
				   -I../../ext/regex \
				   -I../../ext/uthash/src \
				   $(UNIXem_CFLAGS)

CFLAGS			+= -std=gnu11           $(FLAGS)
CXXFLAGS		+= -std=gnu++14 -Wextra $(FLAGS)
LDFLAGS			+= ../../ext/RE-flex/lib/libreflexmin.a

#------------------------------------------------------------------------------
.PHONY: all clean test install astyle z80asm_lib

#------------------------------------------------------------------------------
define MAKE_C_EXE
all: $(1)$(EXESUFFIX)

$(1)$(EXESUFFIX): $(2)
	$(CC) $(CFLAGS) $(2) $(LDFLAGS) -o $(1)$(EXESUFFIX)
	
clean::
	$(RM) $(1) $(1)$(EXESUFFIX) $(2)

test:: $(1)$(EXESUFFIX)
ifeq ($(3),1)
	./$(1)$(EXESUFFIX)
endif
endef

define MAKE_CXX_EXE
all: $(1)$(EXESUFFIX)

$(1)$(EXESUFFIX): $(2)
	$(CXX) $(CXXFLAGS) $(2) $(LDFLAGS) -o $(1)$(EXESUFFIX)
	
clean::
	$(RM) $(1) $(1)$(EXESUFFIX) $(2)

test:: $(1)$(EXESUFFIX)
ifeq ($(3),1)
	./$(1)$(EXESUFFIX)
endif
endef

#------------------------------------------------------------------------------
$(eval $(call MAKE_CXX_EXE,$(PROJ),$(OBJS),0))

#------------------------------------------------------------------------------
$(TARGET): ../config.h

# config file
../config.h:
	touch ../config.h

# RE-flex generated files
$(TARGET): cpp/options.yy.o

cpp/options.yy.o: cpp/options.yy.cpp

cpp/options.yy.cpp cpp/options.yy.h: cpp/options.l
	$(REFLEX) cpp/options.l \
		--lexer=OptionsLexer \
		--outfile=cpp/options.yy.cpp \
		--header-file=cpp/options.yy.h

# documentation files
$(TARGET): doc/z80asm_manual.o doc/z80asm_usage.o

doc/z80asm_manual.o: doc/z80asm_manual.c

doc/z80asm_usage.o: doc/z80asm_usage.c

doc/z80asm_manual.c doc/z80asm_manual.h doc/z80asm_usage.c doc/z80asm_usage.h: \
		$(wildcard doc/*.pl doc/*.in doc/*.txt doc/*.md)
	$(MAKE) -C doc

#------------------------------------------------------------------------------
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

#------------------------------------------------------------------------------
# libraries
#------------------------------------------------------------------------------
all: z80asm_lib

z80asm_lib:
ifeq ($(CROSS),0)
	$(MAKE) -C z80asm_lib
	$(CP) z80asm_lib/$(PROJ)-*.lib .
endif

clean::
	$(MAKE) -C z80asm_lib clean
	$(RM) $(PROJ)-*.lib

#------------------------------------------------------------------------------
# developer generated source files, and test libs
#------------------------------------------------------------------------------
.PHONY: dev
dev:
	perl asmstyle.pl z80asm_lib/*.asm
	$(MAKE) -C tt
	$(MAKE) -C lib
	$(MAKE) -f dev/Makefile
	$(MAKE) all

clean::
	$(MAKE) -C lib clean

#------------------------------------------------------------------------------
# install
#------------------------------------------------------------------------------
install: $(TARGET) z80asm_lib
	$(INSTALL) $(TARGET)     $(PREFIX)/bin/$(EXEC_PREFIX)$(TARGET)
	$(INSTALL) asmpp.pl      $(PREFIX)/bin/asmpp.pl
	$(MAKE) -C z80asm_lib install PREFIX=$(PREFIX_SHARE)

#------------------------------------------------------------------------------
# clean
#------------------------------------------------------------------------------
clean::
	$(RM) *.core lib/*.core lib/t/*.core lib/t/uthash/*.core
	$(RM) *.o    lib/*.o    lib/t/*.o    lib/t/uthash/*.o
	$(RM) *.out  lib/*.out  lib/t/*.out  lib/t/uthash/*.out
	$(RM) *.orig lib/*.orig lib/t/*.orig lib/t/uthash/*.orig
	$(RM) *.exe  lib/*.exe  lib/t/*.exe  lib/t/uthash/*.exe
	$(RM) *.tmp  lib/*.tmp  lib/t/*.tmp  lib/t/uthash/*.tmp
	$(RM) *.bak  lib/*.bak  lib/t/*.bak  lib/t/uthash/*.bak
	$(RM) *.log  lib/*.log  lib/t/*.log  lib/t/uthash/*.log
	$(RM) *~     lib/*~     lib/t/*~     lib/t/uthash/*~
	$(RM) -rf Debug Release x1 x2 x3
	$(RM) $(OBJS) $(TEST_SRC_LIB:.c=.o)

#------------------------------------------------------------------------------
# test
#------------------------------------------------------------------------------
all: t/libtestlib.a

t/libtestlib.a : $(OBJS) $(TEST_SRC_LIB:.c=.o)
	ar rcs t/libtestlib.a $(OBJS:cpp/main.o=) $(TEST_SRC_LIB:.c=.o)

$(TEST_SRC_MAIN:.c=.o) : t/libtestlib.a

.o.out :
	$(CC) $(CFLAGS) $< -o $(<:.o=$(EXESUFFIX)) $(LDFLAGS) -Lt -ltestlib
	$(<:.o=$(EXESUFFIX)) 2> $@~
	diff -w $(<:.o=.bmk) $@~
	mv -f $@~ $@

clean::
	$(RM) t/libtestlib.a t/*.o t/*.out lib/t/*.o lib/t/*.out $(TEST_SRC_MAIN:.c=$(EXESUFFIX))

all:
	perl dev/cpu2/cpu.pl

test:: $(TARGET) z80asm_lib
	$(MAKE) dev
	$(MAKE) z80asm_lib
	$(MAKE) prove_c
	$(MAKE) prove_perl

testdev:
	$(MAKE) -C z80asm_lib test
	perl dev/cpu2/cpu.pl -s test

testlib:
	$(MAKE) -C lib test

prove_c: t/libtestlib.a $(TEST_SRC_MAIN:.c=.o) $(TEST_SRC_MAIN:.c=.out)

prove_perl: $(TARGET)
	perl -S prove --statefile=.prove2 --state=slow,save -j9 t2/*.t
	perl -S prove --statefile=.prove1 --state=slow,save     t/*.t		# these are not thread-safe

clean::
	$(RM) zcc_opt.def zcc_proj.lst

testzcc : $(TARGET)
	zcc +zx -lndos       -create-app -omicroman.bin ../../examples/microman.c
	zcc +zx -lndos -lmzx -create-app -omandel.bin   ../../examples/graphics/mandel.c
	eightyone microman.tap
	eightyone mandel.tap

clean::
	$(RM) zcc_opt.def microman.bin microman.tap
	$(RM) zcc_opt.def mandel.bin   mandel.tap

clean::
	$(RM) -r *.dSYM
	$(RM) $(TARGET) lib/$(TARGET) test* lib/test*
	$(RM) t/*.tmp lib/t/*.tmp t/dev/*.tmp

#------------------------------------------------------------------------------
astyle:
	$(ASTYLE) *.c,*.h cpp/*.cpp,*.h

clean::
	$(RM) *.orig t/*.orig *.bak t/*.bak

#------------------------------------------------------------------------------
# Dependencies
#------------------------------------------------------------------------------
clean::
	$(RM) $(DEPENDS)

-include $(DEPENDS)
