;       Kludgey startup for Spectra
;       Options:
;          startup=2  --> ROM mode (position code at location 0 and provide minimal interrupt services)
;          startup=3  --> Place variables in printer buffer to save memory (may be dangerous !)
;
;       djm 18/5/99
;
;       $Id: spec_crt0.asm,v 1.40 2016-04-26 21:19:20 dom Exp $
;


        MODULE  zx82_crt0

        
;--------
; Include zcc_opt.def to find out some info
;--------
        INCLUDE "zcc_opt.def"

;--------
; Some scope definitions
;--------

        EXTERN    _main           ; main() is always external to crt0 code

        PUBLIC    cleanup         ; jp'd to by exit()
        PUBLIC    l_dcal          ; jp(hl)


        PUBLIC    exitsp          ; atexit() variables
        PUBLIC    exitcount

        PUBLIC    heaplast        ; Near malloc heap variables
        PUBLIC    heapblocks

        PUBLIC    __sgoioblk      ; stdio info block

        PUBLIC    base_graphics   ; Graphical variables
        PUBLIC    coords          ; Current xy position

        PUBLIC    snd_tick        ; Sound variable
        PUBLIC    bit_irqstatus   ; current irq status when DI is necessary

        PUBLIC    _RND_BLOCKSIZE;

        PUBLIC    call_rom3       ; Interposer
       
        PUBLIC    _FRAMES
        defc      _FRAMES = 23672 ; Timer

;--------
; Set an origin for the application (-zorg=) default to 32768
;--------

        IF DEFINED_ZXVGS
        IF !myzorg
                DEFC    myzorg = $5CCB     ; repleaces BASIC program
        ENDIF
        IF !STACKPTR
                DEFC    STACKPTR = $FF57   ; below UDG, keep eye when using banks
        ENDIF
        ENDIF

        
        IF      !myzorg
            IF (startup=2)                 ; ROM ?
                defc    myzorg  = 0
                defc    STACKPTR = 32767
            ELSE
                defc    myzorg  = 32768
            ENDIF
        ENDIF


        org     myzorg


start:

        ; --- startup=2 ---> build a ROM

IF (startup=2)

        di          ; put hardware in a stable state
        ld      a,$3F
        ld      i,a
        jr      init            ; go over rst 8, bypass shadow ROM

        defs    $0008-ASMPC
if (ASMPC<>$0008)
        defs    CODE_ALIGNMENT_ERROR
endif

        ; --- rst 8 ---
        ld      hl,($5c5d)      ; It was the address reached by CH-ADD.
        nop                     ; one byte still, to jump over the
                                ; Opus Discovery and similar shadowing traps
        ; --- nothing more ?

init:
        im      1
        ld      sp,STACKPTR-64
        ld      a,@111000       ; White PAPER, black INK
        call    zx_internal_cls
        ld      (hl),0
        ld      bc,42239
        ldir
        call    zx_internal_init
        ei
ELSE

        ; --- startup=[default] ---

        ld      iy,23610        ; restore the right iy value, 
                                ; fixing the self-relocating trick, if any
  IF !DEFINED_ZXVGS
        ld      (start1+1),sp   ; Save entry stack
  ENDIF
  IF      STACKPTR
        ld      sp,STACKPTR
  ENDIF
        ld      hl,-64
        add     hl,sp
        ld      sp,hl
        ld      (exitsp),sp
; Optional definition for auto MALLOC init; it takes
; all the space between the end of the program and UDG
IF DEFINED_USING_amalloc
        ld  hl,_heap
        ld  c,(hl)
        inc hl
        ld  b,(hl)
        inc bc
        ; compact way to do "mallinit()"
        xor a
        ld  (hl),a
        dec hl
        ld  (hl),a

        ;  Stack is somewhere else, no need to reduce the size for malloc
        ld  hl,65535-168 ; Preserve UDG
        sbc hl,bc   ; hl = total free memory

        push bc ; main address for malloc area
        push hl ; area size
        EXTERN sbrk_callee
        call    sbrk_callee
ENDIF

  IF DEFINED_ZXVGS
;setting variables needed for proper keyboard reading
        LD      (IY+1),$CD      ; FLAGS #5C3B
        LD      (IY+48),1       ; FLAGS2 #5C6A
        EI                      ; ZXVGS starts with disabled interrupts
  ENDIF
        ld      a,2             ; open the upper display (uneeded?)
        call    5633

ENDIF


IF DEFINED_NEEDresidos
        call    residos_detect
        jp      c,cleanup_exit
ENDIF
	call	crt0_init_data
        call    _main           ; Call user program
cleanup:
;
;       Deallocate memory which has been allocated here!
;
        push    hl
IF !DEFINED_nostreams
IF DEFINED_ANSIstdio
        EXTERN     closeall
        call    closeall
ENDIF
ENDIF


IF (startup=2)      ; ROM ?

cleanup_exit:
        rst     0

        defs    56-cleanup_exit-1

if (ASMPC<>$0038)
        defs    CODE_ALIGNMENT_ERROR
endif

; ######## IM 1 MODE INTERRUPT ENTRY ########

      INCLUDE "spec_crt0_rom_isr.as1"

; ########  END OF ROM INTERRUPT HANDLER ######## 

PUBLIC zx_internal_cls
zx_internal_cls:
        ld      hl,$4000        ; cls
        ld      d,h
        ld      e,l
        inc     de
        ld      (hl),0
        ld      bc,$1800
        ldir
        ld      (hl),a
        ld      bc,768
        ldir
        rrca
        rrca
        rrca
        out     (254),a
        ret

zx_internal_init:
        ld      a,@111000       ; White PAPER, black INK
        ld      ($5c48),a       ; BORDCR
        ld      ($5c8d),a       ; ATTR_P
        ld      ($5c8f),a       ; ATTR_T

        ld      hl,$8080
        ld      (fp_seed),hl
        ret

ELSE
  IF DEFINED_ZXVGS
        POP     BC              ;let's say exit code goes to BC
        RST     8
        DEFB    $FD             ;Program finished
  ELSE
cleanup_exit:
        ld      hl,10072        ;Restore hl' to what basic wants
        exx
        pop     bc
start1: ld      sp,0            ;Restore stack to entry value
        ret
  ENDIF
ENDIF



l_dcal: jp      (hl)            ;Used for function pointer calls


;---------------------------------
; Select which printf core we want
;---------------------------------
    PUBLIC  asm_vfprintf
IF DEFINED_floatstdio
    EXTERN  asm_vfprintf_level3
    defc    asm_vfprintf = asm_vfprintf_level3
ELSE
    IF DEFINED_complexstdio
            EXTERN  asm_vfprintf_level2
        defc    asm_vfprintf = asm_vfprintf_level2
    ELSE
            EXTERN  asm_vfprintf_level1
        defc    asm_vfprintf = asm_vfprintf_level1
    ENDIF
ENDIF

;---------------------------------------------
; Some +3 stuff - this needs to be below 49152
;---------------------------------------------
IF DEFINED_NEEDresidos
        INCLUDE "idedos.def"

        defc    ERR_NR=$5c3a            ; BASIC system variables
        defc    ERR_SP=$5c3d

        PUBLIC    dodos
;
; This is support for residos, we use the normal
; +3 -lplus3 library and rewrite the values so
; that they suit us somewhat
dodos:
        exx
        push    iy
        pop     hl
        ld      b,PKG_IDEDOS
        rst     RST_HOOK
        defb    HOOK_PACKAGE
        ld      iy,23610
        ret

; Detect an installed version of ResiDOS.
;
; This should be done before you attempt to call any other ResiDOS/+3DOS/IDEDOS
; routines, and ensures that the Spectrum is running with ResiDOS installed.
; Since +3DOS and IDEDOS are present only from v1.40, this version must
; be checked for before making any further calls.
;
; If you need to use calls that were only provided from a certain version of
; ResiDOS, you can check that here as well.
;
; The ResiDOS version call is made with a special hook-code after a RST 8,
; which will cause an error on Speccies without ResiDOS v1.20+ installed,
; or error 0 (OK) if ResiDOS v1.20+ is present. Therefore, we need
; to trap any errors here.
residos_detect:
        ld      hl,(ERR_SP)
        push    hl                      ; save the existing ERR_SP
        ld      hl,detect_error
        push    hl                      ; stack error-handler return address
        ld      hl,0
        add     hl,sp
        ld      (ERR_SP),hl             ; set the error-handler SP
        rst     RST_HOOK                ; invoke the version info hook code
        defb    HOOK_VERSION
        pop     hl                      ; ResiDOS doesn't return, so if we get
        jr      noresidos               ; here, some other hardware is present
detect_error:
        pop     hl
        ld      (ERR_SP),hl             ; restore the old ERR_SP
        ld      a,(ERR_NR)
        inc     a                       ; is the error code now "OK"?
        jr      nz,noresidos            ; if not, ResiDOS was not detected
        ex      de,hl                   ; get HL=ResiDOS version
        push    hl                      ; save the version
        ld      de,$0140                ; DE=minimum version to run with
        and     a
        sbc     hl,de
        pop     bc                      ; restore the version to BC
       ret     nc                      ; and return with it if at least v1.40
noresidos:
        ld      bc,0                    ; no ResiDOS
        ld      a,$ff
        ld      (ERR_NR),a              ; clear error
        ret


ENDIF


;---------------------------------------------
; Some +3 stuff - this needs to be below 49152
;---------------------------------------------
IF DEFINED_NEEDplus3dodos
;       Routine to call +3DOS Routines. Located in startup
;       code to ensure we don't get paged out
;       (These routines have to be below 49152)
;       djm 17/3/2000 (after the manual!)
        PUBLIC    dodos
dodos:
        call    dodos2          ;dummy routine to restore iy afterwards
        ld      iy,23610
        ret
dodos2:
        push    af
        push    bc
        ld      a,7
        ld      bc,32765
        di
        ld      (23388),a
        out     (c),a
        ei
        pop     bc
        pop     af
        call    cjumpiy
        push    af
        push    bc
        ld      a,16
        ld      bc,32765
        di
        ld      (23388),a
        out     (c),a
        ei
        pop     bc
        pop     af
        ret
cjumpiy:
        jp      (iy)
ENDIF


; Call a routine in the spectrum ROM
; The routine to call is stored in the two bytes following
call_rom3:
        exx                      ; Use alternate registers
IF DEFINED_NEED_ZXMMC
        push    af
        xor     a                ; standard ROM
        out     ($7F),a          ; ZXMMC FASTPAGE
        pop     af
ENDIF
        ex      (sp),hl          ; get return address
        ld      c,(hl)
        inc     hl
        ld      b,(hl)           ; BC=BASIC address
        inc     hl
        ex      (sp),hl          ; restore return address
        push    bc
        exx                      ; Back to the regular set
        ret
        
IF NEED_floatpack
      INCLUDE   "float.asm"
ENDIF

        defm    "Small C+ ZX"   ;Unnecessary file signature
        defb    0

;--------
; Variables: we have two options, to keep them in the program block or to
; locate such stuff somewhere else in RAM (to make ROMable code).
;--------

    SECTION code_crt_init
        ; Setup std* streams
crt0_init_data:
        ; TODO: Reset the bss section
        ld      hl,$8080        ;Initialise floating point seed
        ld      (fp_seed),hl
IF DEFINED_ANSIstdio
        ld      hl,__sgoioblk
        ld      de,__sgoioblk+1
        ld      bc,39
        ld      (hl),0
        ldir
        ld      hl,__sgoioblk+2
        ld      (hl),19 ;stdin
        ld      hl,__sgoioblk+6
        ld      (hl),21 ;stdout
        ld      hl,__sgoioblk+10
        ld      (hl),21 ;stderr
ENDIF
IF DEFINED_USING_amalloc
        EXTERN ASMTAIL
	ld	(_heap),ASMTAIL
ENDIF
    ; SDCC initialisation code gets placed here
    SECTION code_crt_exit

        ret
    SECTION code_compiler
    SECTION code_clib
    SECTION code_crt0_sccz80
    SECTION code_l_sdcc
    SECTION code_math
    SECTION code_error
    SECTION data_compiler
    SECTION rodata_compiler
    SECTION rodata_clib

    SECTION bss_crt
    ; Variables need by crt0 code and some lib routines can be kept separately
IF (startup=2) | (startup=3) ; ROM or moved system variables
        IF !DEFINED_sysdefvarsaddr
             defc sysdefvarsaddr = 23552-70   ; Just before the ZX system variables
        ENDIF
        org sysdefvarsaddr
ELSE
        ; For non-ROM startup move all variables together
        IF DEFINED_defvarsaddr
            org defvarsaddr
        ENDIF
ENDIF
coords:         defw    0       ; Current graphics xy coordinates
base_graphics:  defw    0       ; Address of the Graphics map

IF !DEFINED_HAVESEED
                PUBLIC    _std_seed        ;Integer rand() seed
_std_seed:      defw    0       ; Seed for integer rand() routines
ENDIF

exitsp:         defw    0       ; Address of where the atexit() stack is
exitcount:      defb    0       ; How many routines on the atexit() stack


heaplast:       defw    0       ; Address of last block on heap
heapblocks:     defw    0       ; Number of blocks


IF DEFINED_USING_amalloc
                EXTERN ASMTAIL
PUBLIC _heap
; The heap pointer will be wiped at startup,
; but first its value (based on ASMTAIL)
; will be kept for sbrk() to setup the malloc area
_heap:
                defw 0		; Initialised by code_crt_init - location of the last program byte
                defw 0
ENDIF

IF DEFINED_NEED1bitsound
snd_tick:       defb    0   ; Sound variable
bit_irqstatus:  defw    0
ENDIF

; ZXMMC SD/MMC interface
IF DEFINED_NEED_ZXMMC
    PUBLIC card_select
card_select:    defb    0    ; Currently selected MMC/SD slot for ZXMMC
ENDIF

; Default block size for "gendos.lib"
; every single block (up to 36) is written in a separate file
; the bigger RND_BLOCKSIZE, bigger can be the output file size
; but this comes at cost of the malloc'd space for the internal buffer
; Current block size is kept in a control block (just a structure saved
; in a separate file, so changing this value
; at runtime before creating a file is perfectly legal.
_RND_BLOCKSIZE: defw    1000

; std* streams
__sgoioblk:     defs    40

;-----------------------
; Floating point support
;-----------------------
IF NEED_floatpack
fp_seed:        defb    $80,$80,0,0,0,0 ;FP seed (unused ATM)
extra:          defs    6               ;FP register
fa:             defs    6               ;FP Accumulator
fasign:         defb    0               ;FP register
ENDIF

IF startup=2
                PUBLIC  romsvc
romsvc:         defs    10  ; Pointer to the end of the sysdefvars
                            ; used by the ROM version of some library
ENDIF

    SECTION bss_compiler
IF (startup=2) | (startup=3) ; ROM or moved system variables
        IF !DEFINED_defvarsaddr
             defc defvarsaddr = 24576   ; Just before the ZX system variables
        ENDIF
        org defvarsaddr
ENDIF

    SECTION bss_clib
    SECTION bss_error





