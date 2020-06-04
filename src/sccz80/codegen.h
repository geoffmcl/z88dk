/* codegen.c */
extern void header(void);
extern void DoLibHeader(void);
extern void trailer(void);
extern void outname(const char *sname, char pref);
extern void StoreTOS(Kind kind);
extern void putmem(SYMBOL *sym);
extern void putstk(LVALUE *lval);
extern void puttos(void);
extern void put2tos(void);
extern void loadargc(int n);
extern void indirect(LVALUE *lval);
extern void swap(void);
extern void lpush(void);
extern void zpush(void);
extern void dpush_under(int val_type);
extern void zpop(void);
extern void zcall(const char *name, SYMBOL *sym)
;extern void zshortcall(int rst, int value) ;
extern void zbankedcall(SYMBOL *sym);
extern int dopref(SYMBOL *sym);
extern void callrts(char *sname);
extern int callstk(Type *type, int n, int isfarptr, int last_argument_size);
extern void jump(int label);
extern void opjump(char *, int);
extern void testjump(LVALUE *,int label);
extern void zerojump(void (*oper)(LVALUE *,int), int label, LVALUE *lval);
extern void defbyte(void);
extern void defstorage(void);
extern void defword(void);
extern void deflong(void);
extern void defmesg(void);
extern void point(void);
extern int modstk(int newsp, Kind save,int saveaf, int usebc);
extern void scale(Kind type, Type *tag);


extern void convUint2long(void);
extern void convSint2long(void);
extern void DoubSwap(void);
extern void vlongconst(double val);
extern void vlongconst_tostack(double val);
extern void vconst(int32_t val);
extern void const2(int32_t val);
extern void jumpc(int);
extern void jumpnc(int);
extern void jumpr(int);
extern void opjumpr(char *, int);
extern void setcond(int);
extern void dummy(LVALUE *);
extern void EmitLine(int);
extern void popframe(void);
extern void pushframe(void);
extern void FrameP(void);
extern void PutFrame(char,int);
extern void RestoreSP(char);
extern void zcarryconv(void);
extern void convUint2char(void);
extern void convSint2char(void);

extern void output_section(const char *section_name);
extern void function_appendix(SYMBOL *func);


extern void zentercritical(void);
extern void zleavecritical(void);
extern int zcriticaloffset(void);
extern void zconvert_constant_to_double(double value, unsigned char isunsigned);
extern void zconvert_to_double(Kind type, unsigned char isunsigned);
extern void zconvert_from_double(Kind type, unsigned char isunsigned);
extern int push_function_argument(Kind expr, Type *type, int push_sdccchar);
extern int push_function_argument_fnptr(Kind expr, Type *type, int push_sdccchar, int is_last_argument);
extern void reset_namespace();
extern void zwiden_stack_to_long(LVALUE *lval);
