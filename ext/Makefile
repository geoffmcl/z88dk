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

clean:
	$(RM) $(PREFIX)/bin/reflex$(EXESUFFIX)
	cd RE-flex && ./clean.sh

$(PREFIX)/bin/reflex$(EXESUFFIX):
	cd RE-flex && ./clean.sh
	cd RE-flex && ./build.sh
	$(INSTALL) RE-flex/bin/$(EXEDIR)/reflex$(EXESUFFIX) $(PREFIX)/bin/reflex$(EXESUFFIX)

