# z80asm restart
# Copyright (c) Paulo Custodio, 2020
# License: http://www.perlfoundation.org/artistic_license_2_0

PROJ			:= z80asm2

# EXESUFFIX is passed when cross-compiling Win32 on Linux
ifeq ($(OS),Windows_NT)
  EXESUFFIX 	:= .exe
else
  EXESUFFIX 	?=
endif

# tools
# Note: gcc implements std::make_unique only in C++14
CC 				?= gcc
CCFLAGS			+= -g -Wall -std=gnu11 -MMD \
				-I../../ext/uthash/src

INSTALL 		?= install
CROSS			?= 0

ASTYLE			:= astyle --project=.astylerc
LEX				:= flex -i -s
YACC			:= bison -Wall -l 

# objects
SRCS			:= $(sort $(wildcard *.c) lex.c gram.c )	# sort removes duplicates
OBJS_ALL		:= $(SRCS:.c=.o)
OBJS			:= $(filter-out $(PROJ).o, $(OBJS_ALL))

T_SRCS			:= $(wildcard t/*.c)
T_OBJS			:= $(T_SRCS:.c=.o)

DEPENDS			:= $(SRCS:.c=.d) $(T_SRCS:.c=.d)

#------------------------------------------------------------------------------
.PHONY: all clean realclean test install astyle

#------------------------------------------------------------------------------
define MAKE_EXE
all: $(1)$(EXESUFFIX)

$(1)$(EXESUFFIX): $(2)
	$(CC) $(CCFLAGS) $(2) $(LDFLAGS) -o $(1)$(EXESUFFIX)
	
clean::
	$(RM) $(1) $(1)$(EXESUFFIX) $(2)

test:: $(1)$(EXESUFFIX)
ifeq ($(3),1)
	./$(1)$(EXESUFFIX)
endif
endef

#------------------------------------------------------------------------------
$(eval $(call MAKE_EXE,$(PROJ),$(OBJS_ALL),0))
$(eval $(call MAKE_EXE,t/test,$(T_OBJS) $(OBJS),1))

#------------------------------------------------------------------------------
.c.o:
	$(CC) $(CCFLAGS) -c $< -o $@

#------------------------------------------------------------------------------
lex.c: lex.l asm.h gram.h
	$(LEX) lex.l
	grep -v '^#line' < lex.c > lex.c1
	mv -f lex.c1 lex.c
	-$(MAKE) astyle

gram.c gram.h: gram.y asm.h parse.h
	$(YACC) gram.y
	-$(MAKE) astyle

realclean:: clean
	$(RM) lex.c gram.c gram.h

#------------------------------------------------------------------------------
astyle:
	$(ASTYLE) ./*.c,*.h,*.def,*.re t/*.c,*.h,*.def

clean::
	$(RM) *.orig t/*.orig *.bak t/*.bak

#------------------------------------------------------------------------------
t/test.o: t/test.def

t/test.def: t/*.c Makefile t/make_tests.pl
	perl t/make_tests.pl

#------------------------------------------------------------------------------
install: $(PROJ)$(EXESUFFIX)
	$(INSTALL) $(PROJ)$(EXESUFFIX) $(PREFIX)/bin/$(EXEC_PREFIX)$(PROJ)$(EXESUFFIX)

-include $(DEPENDS)

clean::
	$(RM) $(DEPENDS)
