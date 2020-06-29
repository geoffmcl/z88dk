# Build external dependencies

ifeq ($(OS),Windows_NT)
  EXESUFFIX 	:= .exe
  EXEDIR		:= win32
else
  EXESUFFIX 	?=
  EXEDIR		:=
endif

INSTALL			?= install
PREFIX 			?= ..

all: $(PREFIX)/bin/reflex$(EXESUFFIX)

$(PREFIX)/bin/reflex$(EXESUFFIX):
	cd RE-flex && sh clean.sh
	cd RE-flex && sh build.sh
	$(INSTALL) RE-flex/bin/$(EXEDIR)/reflex$(EXESUFFIX) $(PREFIX)/bin/reflex$(EXESUFFIX)

