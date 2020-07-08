#------------------------------------------------------------------------------
# z80asm restart
# Copyright (c) Paulo Custodio, 2011-2020
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

START_DIR		:= $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
Z80ASM_DIR		:= $(realpath $(START_DIR)/..)
Z88DK_DIR		:= $(realpath $(START_DIR)/../../..)

# UNIXem is needed in both Windows_NT and in a MinGW build in Unix
ifeq ($(EXESUFFIX),.exe)
  UNIXem_CFLAGS	:= -I$(Z88DK_DIR)/ext/UNIXem/include
  UNIXem_OBJS	:=   $(Z88DK_DIR)/ext/UNIXem/src/glob.o \
				     $(Z88DK_DIR)/ext/UNIXem/src/dirent.o
endif

CC 				?= gcc
CXX				?= g++
INSTALL 		?= install
CROSS			?= 0

ASTYLE			:= astyle --project=.astylerc

CP				:= cp -f 

#------------------------------------------------------------------------------
ALL_C_SRCS		:= $(wildcard \
						$(Z80ASM_DIR)/*.c \
						$(Z80ASM_DIR)/doc/*.c \
						$(Z80ASM_DIR)/lib/*.c \
						$(Z88DK_DIR)/src/common/*.c \
						$(Z88DK_DIR)/ext/regex/reg*.c)
C_SRCS			:= $(filter-out \
						$(Z80ASM_DIR)/test.c, \
						$(filter-out \
						$(Z80ASM_DIR)/main.c, \
						$(ALL_C_SRCS)))
CXX_SRCS		:= $(wildcard *.cpp)
OBJS			:= $(C_SRCS:.c=.o) $(CXX_SRCS:.cpp=.o) $(UNIXem_OBJS)
DEPENDS			:= $(C_SRCS:.c=.d) $(CXX_SRCS:.cpp=.d)

FLAGS			:= -MMD -g -Wall \
				   -I$(Z80ASM_DIR)/z80asm2 \
				   -I$(Z88DK_DIR)/ext/RE-flex/include \
				   -I$(Z80ASM_DIR)/doc \
				   -I$(Z80ASM_DIR)/lib \
				   -I$(Z88DK_DIR)/src \
				   -I$(Z88DK_DIR)/src/common \
				   -I$(Z88DK_DIR)/ext/optparse \
				   -I$(Z88DK_DIR)/ext/regex \
				   -I$(Z88DK_DIR)/ext/uthash/src \
				   $(UNIXem_CFLAGS)
CCFLAGS			+= -std=gnu11           $(FLAGS)
CXXFLAGS		+= -std=gnu++14 -Wextra $(FLAGS)
LDFLAGS			+= -L$(Z88DK_DIR)/ext/RE-flex/lib -lreflexmin

#------------------------------------------------------------------------------
.PHONY: all clean astyle

#------------------------------------------------------------------------------
define MAKE_EXE
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
$(eval $(call MAKE_EXE,$(PROJ),$(OBJS),0))

$(TARGET): $(Z88DK_DIR)/src/config.h

$(Z88DK_DIR)/src/config.h:
	touch $(Z88DK_DIR)/src/config.h

$(TARGET): options.yy.o

options.yy.o: options.yy.cpp

options.yy.cpp options.yy.h: options.l
	reflex options.l --outfile=options.yy.cpp --header-file=options.yy.h \
		--fast --noline --lexer=OptionsLexer

$(TARGET): $(Z80ASM_DIR)/doc/z80asm_manual.o $(Z80ASM_DIR)/doc/z80asm_usage.o

$(Z80ASM_DIR)/doc/z80asm_manual.o: $(Z80ASM_DIR)/doc/z80asm_manual.c

$(Z80ASM_DIR)/doc/z80asm_usage.o: $(Z80ASM_DIR)/doc/z80asm_usage.c

$(Z80ASM_DIR)/doc/z80asm_manual.c \
$(Z80ASM_DIR)/doc/z80asm_manual.h \
$(Z80ASM_DIR)/doc/z80asm_usage.c \
$(Z80ASM_DIR)/doc/z80asm_usage.h: \
		$(wildcard \
			$(Z80ASM_DIR)/doc/*.pl \
			$(Z80ASM_DIR)/doc/*.in \
			$(Z80ASM_DIR)/doc/*.txt \
			$(Z80ASM_DIR)/doc/*.md)
	$(MAKE) -C $(Z80ASM_DIR)/doc

all: $(Z80ASM_DIR)/$(TARGET)

$(Z80ASM_DIR)/$(TARGET): $(TARGET)
	$(CP) $(TARGET) $(Z80ASM_DIR)/$(TARGET)

#------------------------------------------------------------------------------
.c.o:
	$(CC) $(CCFLAGS) -c $< -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

#------------------------------------------------------------------------------
astyle:
	$(ASTYLE) ./*.cpp,*.h

clean::
	$(RM) *.orig t/*.orig *.bak t/*.bak

#------------------------------------------------------------------------------
-include $(DEPENDS)

clean::
	$(RM) $(DEPENDS) 
	$(RM) $(Z80ASM_DIR)/$(PROJ) 
	$(RM) $(Z80ASM_DIR)/$(TARGET)
