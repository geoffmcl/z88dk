; ulong esxdos_m_getdate(void)

SECTION code_clib
SECTION code_esxdos

PUBLIC _esxdos_ram_m_getdate

EXTERN asm_esxdos_m_getdate

_esxdos_ram_m_getdate:

   push iy
   
   call asm_esxdos_m_getdate
   
   pop iy
   ret
