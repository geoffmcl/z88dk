
; void *p_forward_list_remove(p_forward_list_t *list, void *item)

XDEF p_forward_list_remove_callee

p_forward_list_remove_callee:

   pop hl
   pop bc
   ex (sp),hl
   
   INCLUDE "../../z80/asm_p_forward_list_remove.asm"
