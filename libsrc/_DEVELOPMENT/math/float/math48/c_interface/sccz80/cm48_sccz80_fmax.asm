
; double __CALLEE__ fmax(double x, double y)

SECTION code_fp_math48

PUBLIC cm48_sccz80_fmax

EXTERN lm48_fmax, cm48_sccz80p_collect_2

cm48_sccz80_fmax:

   call cm48_sccz80p_collect_2
   
   ; AC'= x
   ; AC = y
   
   exx
   
   jp lm48_fmax
