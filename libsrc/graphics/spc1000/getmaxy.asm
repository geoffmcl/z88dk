	INCLUDE	"graphics/grafix.inc"


        SECTION   code_clib
        PUBLIC    getmaxy
        PUBLIC    _getmaxy
	EXTERN	__spc1000_mode

.getmaxy
._getmaxy
        ld      a,(__spc1000_mode)
        and     a
        ld      hl, 32
        ret     z
        ld      hl,maxy - 1
        ret
