; unsigned char zx_tape_load(void *dst, unsigned int len, unsigned char type)

SECTION code_clib
SECTION code_arch

PUBLIC _zx_tape_load

EXTERN l_zx_tape_load_callee

_zx_tape_load:

   pop hl
   pop bc
   pop de
   dec sp
   pop af
   
   push af
   dec sp
   push bc
   push hl
   
   jp l_zx_tape_load_callee
