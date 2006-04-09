
; DRAW LOAD SPRITE 2 BYTE DEFINITION ROTATED, RIGHT BORDER WITH IMPLIED MASK
; 04.2006 aralbrec, Sprite Pack v3.0
; sinclair spectrum version

XLIB SP1Load2RBIMData
LIB SP1Load2LBIMData
XREF SP1RETSPRDRAW, SP1V_ROTTBL, SP1V_PIXELBUFFER

; following data segment copied into struct sp1_cs

.SP1Load2RBIMData

   ld de,0
   nop
   ld hl,0
   call SP1Load2RBIM
   
; following draw code called by way of SP1UpdateNow
;
;  a = hor rot table
; bc = graphic disp
; de = graphic def ptr
; hl = left graphic def ptr
;
; 64 + 8*54 - 6 + 10 = 500 cycles

.SP1Load2RBIM

   cp SP1V_ROTTBL/256
   jp z, SP1RETSPRDRAW

   add hl,bc
   ex de,hl
   ld h,a
   inc h
   ld l,$ff
   ld c,(hl)
   
   ;  h = shift table
   ;  c = constant mask
   ; de = sprite def (graph only)

.SP1Load2RBIMRotate

   jp SP1Load2LBIMData+21              ; SP1Load2LBIMRotate
   