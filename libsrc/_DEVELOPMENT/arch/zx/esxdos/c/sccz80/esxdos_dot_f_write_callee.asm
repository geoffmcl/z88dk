; int esxdos_f_write(uchar handle, void *src, size_t nbyte)

SECTION code_clib
SECTION code_esxdos

PUBLIC esxdos_dot_f_write_callee

EXTERN asm_esxdos_f_write

esxdos_dot_f_write_callee:

   pop af
   pop bc
   pop hl
   pop de
   push af
   
   ld a,e
   jp asm_esxdos_f_write
