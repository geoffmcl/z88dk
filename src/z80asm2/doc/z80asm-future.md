<a id=top></a>

# z80asm - Z80 module assembler, linker, librarian

**z80asm** is part of the [z88dk](http://www.z88dk.org/) project and is used as the back-end of the z88dk C compilers. It is not to be confused with other non-**z88dk** related projects with the same name.

**z80asm** is a relocatable assembler, linker and librarian that can assemble Intel 8080/8085 and  [Z80](#7_) -family assembly files into a relocatable object format, can manage sets of object files in libraries and can build binary images by linking these object files together. The binary images can be defined in different sections, to match the target architecture.


 

----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=1_></a>

## 1. Usage ...


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=1_1_></a>

### 1.1. ... as pre-processor

```
z80asm -E [options] file...
```

When called with the -E option, **z80asm** pre-processes each input file and stores the result in files with the ```.i``` extension, with all macros expanded, include files expanded, and constants converted to hexadecimal. 


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=1_2_></a>

### 1.2. ... as assembler

```
z80asm [options] file...
```

By default, i.e. without any options, **z80asm** assembles each of the listed files into relocatable object files with a ```.o``` extension. It shows a summary of all the options when called without arguments.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=1_3_></a>

### 1.3. ... as linker

```
z80asm -b [options] file...
```

When called with the  [-b](#5_3_7_)  option, **z80asm** links the object files together into a set of binary files.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=1_4_></a>

### 1.4. ... as librarian

```
z80asm -xlibrary.lib [options] file...
```

When called with the  [-x](#5_3_19_)  option, **z80asm** builds a library containing all the object files passed as argument. That library can then be used during linking by specifying it with the  [-i](#5_3_16_)  option.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_></a>

## 2. Options


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_1_></a>

### 2.1. General Options


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_1_1_></a>

#### 2.1.1. -v (verbose)

The  [-v](#2_1_1_)  options causes **z80asm** to tell what is happening by messages sent to ```stdout```.



----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_2_></a>

### 2.2. Preprocessor options


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_2_1_></a>

#### 2.2.1. -atoctal (at is octal prefix)

By default the at-character (```@```) is used as a binary number prefix. 

With the option  [-atoctal](#2_2_1_)  it is used as the octal number prefix instead.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_2_2_></a>

#### 2.2.2. -dotdirective (period is directive prefix)

By default the period (```.```) is used to signal that the next identifier is a label. 

With the option  [-dotdirective](#2_2_2_)  it is used instead to signal that the next identifier is an assembler directive.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_2_3_></a>

#### 2.2.3. -hashhex (hash is hex prefix)

By default the hash-character (```#```) is used to signal that the next expression should be compiled as an immediate value. This meaning, although common in assemblers, is a no-operation in **z80asm**. 

With the option  [-hashhex](#2_2_3_)  the hash-character is used as the hexadecimal number prefix instead.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_2_4_></a>

#### 2.2.4. -labelcol1 (labels at column 1)

By default **z80asm** needs either a period (```.```) prefix (but see  [-dotdirective](#2_2_2_) ) or a colon (```:```) suffix to signal that an identifier is a label, and white space at the beginning of a line is not significant.

With the option  [-labelcol1](#2_2_4_)  an identifier is a label if started at column 1, or a directive or opcode if started after white space.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_2_5_></a>

#### 2.2.5. -ucase (upper case)

By default **z80asm** is case-sensitive for identifiers, but case-insensitive for assembly keywords (opcodes, directives, registers and flags).

The option  [-ucase](#2_2_5_)  causes **z80asm** to convert all the symbols to upper-case, so that code that assumes case-insentivity can be assembled.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_2_6_></a>

#### 2.2.6. -IXIY (swap IX and IY)

This option causes the IX and IY register roles to be swapped, i.e. references to IX are assembled as IY and vice-versa. Useful when the assembler is used as a back-end of a compiler that uses one of the index registers that is used in the target platform.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_3_></a>

### 2.3. Assembly options


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_3_1_></a>

#### 2.3.1. -noprec (no precedence in expression evaluation)

By default **z80asm** follows the C-precedence rules while evaluating expressions:

| Pecedence | Operators  | Description                              | Associativity |
| ---       | ---        | ---                                      | ---           |
| 1         | **         | Power                                    | right-to-left |
| 2         | + - #      | Unary plus, unary minus, immediate       | right-to-left |
| 3         | ! ~        | Logical NOT and bitwise NOT              | right-to-left |
| 4         | * / %      | Multiply, divide and remainder           | left-to-right |
| 5         | + -        | Add and subtract                         | left-to-right |
| 6         | << >>      | Bitwise left-shift and right-shift       | left-to-right |
| 7         | < <= > >=  | Less, Less-equal, greater, greater-equal | left-to-right |
| 8         | = == != <> | Equal (two forms), Not equal (two forms) | left-to-right |
| 9         | &          | Bitwise AND                              | left-to-right |
| 10         | ^          | Bitwise XOR                              | left-to-right |
| 11        | \|         | Bitwise OR                               | left-to-right |
| 12        | &&         | Logical AND                              | left-to-right |
| 13        | ^^         | Logical XOR                              | left-to-right |
| 14        | \|\|       | Logical OR                               | left-to-right |
| 15        | ?:         | Ternary conditional                      | right-to-left |

With the  [-noprec](#2_3_1_)  expressions are evaluated strictly left-to-right.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_></a>

## 3. Input Files

**z80asm** reads text files in the syntax supported by the specific processor being assembled for (see  [-m](#5_3_10_)  option) and produces the corresponding object files.

An input file with a ```.o``` extension is assumed to be already in object file format and is just read by the linker. Any other extension is considered an assembly source file (conventionally ```.asm```).

A project list file may be supplied in the command line prefixed by the at-sign (e.g. ```@project.lst```). The list file contains one input file name per line, or another project list prefixed with an at-sign, which is opened recursively. Hash signs (```#```) and semi-colons (```;```) may be used at the start of lines in the list files to include comments.

Both the command line and the list files support wild-cards to expand all the files that match the given pattern.

**Note** that the Operating System may do its own wildcard expansion and the pattern may need to be quoted in the command line.

A single star in a file name (```*```) expands to the list of all files/directories that match the complete pattern, where the star represents any sequence of characters. A double-star in a directory name (```**```) expands to the complete directory sub-tree, when searched recursively.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_1_></a>

### 3.1. Source File Format

The assembler parses source files with any of the common end-of-line termination sequences (```"\r"```, ```"\n"``` or ```"\r\n"```). Each line starts with an optional label and can contain assembly directives (i.e. instructions to the assembler), assembly instructions (i.e. code to be translated into object code for the specific processor) or blanks and comments.

A single backslash character (```\```) may be used to separate multiple statements in a single line.

    org 0                     ; assembly directive  
    start: push bc\pop hl     ; define a label and add two assembly opcodes              
    ret                       ; assembly opcode can be at the first column    

Differently to most other assemblers, white space is not significant, i.e. a label can be defined after white space, and an opcode can be written at column 1 (but see option  [-labelcol1](#2_2_4_) ).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_2_></a>

### 3.2. Comments

Comments may start with a semi-colon (```;```) or two slashes (```//```) and end at the end of the line, or may start with slash-star (```/*```) and end with star-slash (```*/```), possibly spanning multiple lines.

    ld a, 1                   ; this is a comment
    ld b, 2                   // another comment
    ld c, /* multi-line comment is valid
             in the middle of an instruction 
          */ 3                ; C = 3


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_3_></a>

### 3.3. Symbols

All symbols in the code (labels, variables, ...) are named with unique identifiers. Identifiers start with a letter or underscore (```_```), and can contain letters, digits, underscores or periods (```.```). Identifiers are case-sensitive (but see option  [-ucase](#2_2_5_) ).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_4_></a>

### 3.4. Labels

A label is a symbol that represents the current assembly address (ASMPC) and is defined at the start of a line by prefixing a symbol with a period (```.```) (but see  [-dotdirective](#2_2_2_) ) or suffixing it with a colon (```:```), i.e. either ```.label``` or ```label:``` (but see  [-labelcol1](#2_2_4_) ).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_5_></a>

### 3.5. Numbers

The assembler accepts numbers in decimal, hexadecimal, octal and binary. Different syntaxes are allowed to simplify porting code written for other assemblers. Some of the prefix characters are also used as operators; in this case a space may be needed to signal the difference, e.g.

    ld a, %10     ; A = 2 (10 binary)
    ld a, 12 % 10 ; A = 2 (remainder of 12 divided by 10)

All expressions are computed as signed integers with the host platform's integer size (32-bit or 64-bit in the most common platforms). Expression evaluation follows the operator precedence of C (but see  [-noprec](#2_3_1_) ).

The underscore can be used to separate groups of digits to help readability, e.g. ```0xFFFF_FFFF``` is the same as ```0xFFFFFFFF```.

Floating point numbers can be supplied with the FLOAT directive, that encodes them in the current floating point format in the object code. Floating point numbers must be supplied in decimal base and have an integer part and a fractional part separated by a period, followed by an optional ```e``` (exponent), a plus or minus sign, and the power of 10 to multiply to the base.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_5_1_></a>

#### 3.5.1. Decimal

Decimal numbers are a sequence of decimal digits (```0..9```), optionally followed by a ```d``` or ```D``` - all prefixes and suffixes are case-insensitive. Leading zeros are insignificant - note the difference from C, where a leading zero means octal.

    ld a, 99
    ld a, 99d


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_5_2_></a>

#### 3.5.2. Hexadecimal

Hexadecimal numbers are a sequence of hexadecimal digits (```0..9``` and ```A..F```, case-insensitive), either prefixed or suffixed with an hexadecimal marker. If the marker is a suffix, and the number starts with a letter, then a leading zero has to be added.

The hexadecimal prefix ```$``` is also the ASMPC identifier if not followed by a hexadecimal number, i.e. the address of the instruction being assembled.

The hexadecimal prefix ```#``` is only recognized with the option  [-hashhex](#2_2_3_) .

    ld a, $FF
    ld a, #FF           ; only with option  [-hashhex](#2_2_3_) 
    ld a, 0xFF
    ld a, 0FFh


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_5_3_></a>

#### 3.5.3. Octal

Octal numbers are a sequence of octal digits (```0..7```), either prefixed or suffixed with an octal marker. 

The octal-prefix ```@``` is only recognized with the option  [-atoctal](#2_2_1_) .

    ld a, @77           ; only with option  [-atoctal](#2_2_1_) 
    ld a, 0o77
    ld a, 0q77
    ld a, 77o
    ld a, 77q


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_5_4_></a>

#### 3.5.4. Binary

Binary numbers are a sequence of binary digits (```0..1```), either prefixed or suffixed with a binary marker.  

The binary prefix ```%``` is also the modulus operator, if not followed by a binary digit. 

The binary prefix ```@``` is recognized unless the option  [-atoctal](#2_2_1_)  is given.

    ld a, %11
    ld a, @11           ; except with option  [-atoctal](#2_2_1_) 
    ld a, 0b11
    ld a, 11b


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_5_5_></a>

#### 3.5.5. Bitmaps

Binary numbers can be specified as bitmaps, with ```#``` as ```1``` and ```-``` as ```0```, using the binary prefix (```@``` or ```%```) immediately followed by a double-quoted string of hashes and dashes.

    defb @"---##---"
    defb @"-##--##-"
    defb %"-##-----"
    defb %"-##-----"
    defb @"-##--##-"
    defb @"---##---"



----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_6_></a>

### 3.6. Keywords

Processor registers (BC, DE, ...) and flags (NZ, ```Z```, ...), and assembly ASMPC, representing the current assembly location, are reserved keywords. They cannot be used as identifiers, and are case-insensitive.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_7_></a>

### 3.7. Directives and Opcodes

Assembler directives ( [ORG](#10_25_) ,  [INCLUDE](#10_18_) , ...) and processor opcodes (NOP, LD, ...) are interpreted as directives or opcodes when appearing at the start of the statement or after a label definition, or as regular identifiers otherwise. The directives and opcodes are case-insensitive.

    jr: jr jr  ; silly example, jr is both a label and an opcode
               ; while correct code, it's confusing, don't do it


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=4_></a>

## 4. Object File Format

The object and library files are stored in binary form as a set of contiguous sections. The files are cross-platform compatible, i.e. they can be created in a big-endian architecture and used in a little-endian one. 

The files start with a signature and a version number.

A set of file pointers at the start of the object file point to the start of each section existing the in the file, or contain *0xFFFFFFFF* (-1) if that section does not exist.

The following object types exist in the file:

| Type      | Size (bytes)  | Description                                       |
| ---       | ---           | ---                                               |
| *char*    | 1             | ASCII character                                   |
| *byte*    | 1             | unsigned 8-bit value                              |
| *word*    | 2             | unsigned 16-bit value, little-endian ( [Z80](#7_) /Intel)  |
| *dword*   | 4             | signed 32-bit value, little-endian ( [Z80](#7_) /Intel)    |
| *string*  | 1+length      | one byte with the string length followed by the characters of the string |
| *lstring* | 2+length      | one word with the string length followed by the characters of the string |


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=4_1_></a>

### 4.1. Object Files

The format of the object files is as follows:

| Addr  | Type      | Value     | Description
| ---   | ---       | ---       | ---
|   0   | char[8]   | "Z80RMF14"| File signature and version
|   8   | dword     | pmodule   | File pointer to *Module Name*, always the last section
|  12   | dword     | pexpr     | File pointer to *Expressions*, may be -1
|  16   | dword     | pnames    | File pointer to *Defined Symbols*, may be -1
|  20   | dword     | pextern   | File pointer to *External Symbols*, may be -1
|  24   | dword     | pcode     | File pointer to *Code Sections*, may be -1
| pexpr |           |           | *Expressions*: set of expressions up to end marker
|       | char      | type      | Type of expression
|       |           |  0        | end marker 
|       |           | 'U'       | 8-bit integer (0 to 255)  
|       |           | 'S'       | 8-bit signed integer (-128 to 127)  
|       |           | 'u'       | 8-bit integer (0 to 255) extended to 16 bits
|       |           | 's'       | 8-bit signed integer (-128 to 127) sign-extended to 16 bits
|       |           | 'C'       | 16-bit integer, little-endian (-32768 to 65535)  
|       |           | 'B'       | 16-bit integer, big-endian (-32768 to 65535)  
|       |           | 'P'       | 24-bit signed integer     
|       |           | 'L'       | 32-bit signed integer     
|       |           | 'J'       | 8-bit jump relative offset
|       |           | '='       | Computed name at link time   
|       | lstring   | sourcefile| Source file name of expression definition, empty to reuse same from previous expression
|       | dword     | linenumber| Line number in source file of expression definition
|       | string    | section   | Section name of expression definition 
|       | word      | ASMPC     | Relative module code address of the start of the assembly instruction to be used as *ASMPC* during expression evaluation
|       | word      | patchptr  | Relative module code patch pointer to store the result of evaluating the expression
|       | string    | targetname| Name of the symbol that receives the result of evaluating the expression, only used for '=' type expressions, empty string for the other types
|       | lstring   | expression| Expression text as parsed from the source file
|       |           |           | ... repeat for every expression ...
| pnames|           |           | *Defined Symbols*: set of defined symbols up to end marker
|       | char      | scope     | Scope of the symbol:
|       |           |  0        | end marker
|       |           | 'L'       | is local
|       |           | 'G'       | is global
|       | char      | type      | Type of symbol: 
|       |           | 'A'       | Relocatable address   
|       |           | 'C'       | Constant
|       |           | '='       | Symbol computed at link time, the corresponding expression is in the *Expressions* section
|       | string    | section   | Section name of symbol definition 
|       | dword     | value     | Absolute value for a constant, or the relative address to the start of the code block for a relocatable address
|       | string    | name      | Name of symbol
|       | string    | sourcefile| Source file name of symbol definition
|       | dword     | linenumber| Line number in source file of symnbol definition
|       |           |           | ... repeat for every symbol ...
|pextern|           |           | *External Symbols*: set of external symbols referred in the module, up *Module Name*
|       | string    | name      | Name of external symbol
|       |           |           | ... repeat for every symbol ...
|pmodule| string    | modname   | *Module Name*: Name of the module
| pcode |           |           | *Code Sections*: set of sections of binary code, up to end marker
|       | dword     | length    | Code length, -1 to signal the end
|       | string    | section   | Section name of code
|       | dword     | origin    | User defined  [ORG](#10_25_)  address for the start of this section, -1 for no  [ORG](#10_25_)  address was defined, or -2 to split section to a different binary file. If multiple sections are given with an  [ORG](#10_25_)  address each, the assembler generates one binary file for each section with a defined  [ORG](#10_25_) , followed by all sections without one.
|       | dword     | align     | Address alignment of this section, -1 if not defined. The previous section is padded to align the start address of this section.
|       |byte[length]|code      | Binary code
|       |           |           | ... repeat for every section ...


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=4_2_></a>

### 4.2. Library File Format

The library file format is a sequence of object files with additional
structures.

| Addr  | Type      | Value     | Description
| ---   | ---       | ---       | ---
|   0   | char[8]   | "Z80LMF14"| File signature and version
| obj   |           |           | *Object File Block*, repeats for every object module
|       | dword     | next      | File pointer of the next object file in the library, -1 if this is the last one
|       | dword     | length    | Length of this object file, or 0 if this object files has been marked "deleted" and will not be used
|       |byte[length]| obj      | Object file
|       |           |           | ... repeat for every object file ...


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=4_3_></a>

### 4.3. Format History

| Version   | Commment
| ---       | ---
| 01        | original z80asm version
| 02        | allow expressions to use standard C operators instead of the original (legacy) z80asm specific syntax. 
| 03        | include the address of the start of the assembly instruction in the object file, so that expressions with ASMPC are correctly computed at link time; remove type 'X' symbols (global library), no longer used.
| 04        | include the source file location of expressions in order to give meaningful link-time error messages.
| 05        | include source code sections.
| 06        | incomplete implementation, fixed in version *07*
| 07        | include  [DEFC](#10_6_)  symbols that are defined as an expression using other symbols and are computed at link time, after all addresses are allocated.
| 08        | include a user defined  [ORG](#10_25_)  address per section.
| 09        | include the file and line number where each symbol was defined.
| 10        | allow a section alignment to be defined.
| 11        | allow big-endian 16-bit expressions to be patched; these big-endian values are used in the ZXN coper unit.
| 12        | allow the target expression of relative jumps to be computed in the link phase
| 13        | add 8-bit signed and unsigned values extended to 16-bits
| 14        | add 24-bit pointers


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_></a>

## 5. z80asm Syntax

THE  [Z80](#7_)  CROSS ASSEMBLER (z88dk)
===============================

Version: v2.6.1 (October 3rd, 2014)

Thank you for purchasing a copy of this cross assembler. We have made an effort to program an easy user interface and efficient assembler source file compiling. The object file and library format is an invention of our own and has also been included in this documentation. We hope that you will enjoy your  [Z80](#7_)  machine code programming with our assembler.

We have made an effort to produce a fairly easy-to-understand documentation. If you have any comments or corrections, please don't hesitate to contact us:

Gunther Strube  
Gl. Kongevej 37, 4.tv.  
DK-1610 Kobenhavn V  
Denmark  
e-mail [gbs@image.dk](mailto:gbs@image.dk)

1\. Running the assembler
-------------------------

Since assemblers are developed for programmers the user interface have been implemented according to the standard command line environment (CLI) used in most operating systems; however, if only a GUI interface is available (as in MacOS) then a command shell must be executed to allow standard command line input. The  [Z80](#7_)  Module Assembler command line interface may change if other programmers wish to implement a different interface to the assembler. You have the opportunity!


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_1_></a>

### 5.1. Help page

When executing the assembler from the command line with the  [-h](#5_3_1_)  option, a help page is displayed. The page shortly explains the syntax of parameter specification and which parameters are available.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_2_></a>

### 5.2. Command line

The syntax of the assembler parameters is a straightforward design. Filenames or a project file are always specified. The options may be left out:

    z80asm [options] <filename {filename}> | <@modulesfile>

As seen above \<options\> must be specified first. Then you enter the names of all files to be assembled. You either choose to specify all file names or a @\<project-file\> containing all file names. File name may be specified with or without the 'asm extension. The correct filename parsing is handled automatically by the assembler. As seen on the syntax at least one source file must be specified and may be repeated with several file names. Only one project file may be specified if no source file names are given.

Many of the parameters are preset with default values which gives an easy user interface when specifying the assembly parameters. Only advanced parameters need to be specified explicitly. The help page displays the default parameter values at the bottom of the page.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_></a>

### 5.3. Command line options

Options are used to control the assembly process and output. They are recognized by the assembler when you specify a leading minus before the option identifier ('-'). Options are always specified before file names or project files.

When the assembler is executed options are all preset with default values and are either switched ON or OFF (active or not). All options have a single letter identification. Upper and lower case letters are distinguished, which means that 'a' might be different command than 'A'. If an option is to be turned off, you simply specify a 'n' before the identification, e.g. -nl which selects listing files not to be created by the assembler.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_1_></a>

#### 5.3.1. -h : Show help screen

Shows a help screen with the available options.



----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_2_></a>

#### 5.3.2. -e\<ext\> : Use alternative source file extension

The default assembler source file extension is ".asm". Using this option, you force the assembler to use another default extension, like ".opt" or ".as" for the source file.

The extension is specified without the ".". Only three letters are accepted - the rest is discarded.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_3_></a>

#### 5.3.3. -M\<ext\> : Use alternative object file extension

The default assembler object file extension is ".obj". Using this option, you force the assembler to use another default extension, like ".o" as the object file name extension.

The extension is specified without the ".". Only three letters are accepted - the rest is discarded.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_4_></a>

#### 5.3.4. -l : Create listing file output

The information in listing files is a combination of the original source file and additional information of the generated machine code. Further, the listing file is page formatted with a header specifying the date and time of the compilation, the filename of the listing and a page number.

For each line of the source file the following information is written:

    <source file line number> <assembler address>  
    <machine code hex dump> <source line>

The machine code and assembler address output are written in hexadecimal notation. If the machine code uses more the 4 bytes, the source file line is written on the following line. This usually happens when you have defined a string constant or a relatively large amount of constant definitions.

The assembler address is always beginning at zero, i.e. the beginning of the current modules' machine code. In a relocated context of the machine code, where all code is positioned at fixed addresses, you will have the opportunity to view the addresses of your code relative to the start of individual modules using the assembler output addresses. Further, the last assembler address can be interpreted as the size of the modules' generated machine code.

Listing files also serves the purpose of a hard copy on paper of your programs, and are useful in a debugging phase (identifying opcodes versus the mnemonic representation of instructions).

The creation of listing files imposes much more processing work of the assembler. If you want to compile your code as quickly as possible then don't create listing files. Listing files obtain their file name from the base of the source filename, and is added with the 'lst' extension.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_5_></a>

#### 5.3.5. -s : Create symbol table

Symbol tables contains the integer form of symbolical names and constants that has been parsed and generated during a compilation of a source file. The structure of the symbol table is divided into two columns. The first contains the parsed symbol names, converted to uppercase. The second column contains the generated value of the symbol name.

All symbol values are displayed in signed 32-bit hexadecimal notation.

The two columns are separated by tabulators which represents a default value of 8 spaces per tabulator. The width of the symbol name column is defined as the tabulator distance multiplied by 4. The default width of the name column is 4 \* 8 = 32 spaces.

The symbol table will be written to the end of the appropriate listing file, if listing file and symbol table output is enabled. If no listing file output is enabled, the symbol table will be written to a separate file, identified with the base name of the source file module and given the 'sym' extension.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_6_></a>

#### 5.3.6. -d : Assemble only updated files

Assemblers usually force compiles all specified files. This is also possible (as default) for the  [Z80](#7_)  Module Assembler. In large application project with 15 modules or more it can be quite frustrating to compile all every time. The solution is to only assemble updated files and leave the rest (since they have been compiled to the programmers knowledge).

But in a long term view it is better to just compile a project without thinking of which files need to be compiled or not. That can be done with the  [Z80](#7_)  Module Assembler. By simply specifying the  [-d](#5_3_6_)  parameter at the command line, only updated source files are assembled into object files - the rest are ignored.

Using the  [-d](#5_3_6_)  option in combination with a project file gives the best project setup for a large compilation; compile your projects without worrying about which module is updated or not.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_7_></a>

#### 5.3.7. -b : Link/relocate object files

The  [-b](#5_3_7_)  option must be used if you want to create an executable  [Z80](#7_)  machine code output file of your previously created object files. You may also use the  [-a](#5_3_8_)  option which is identical in functionality but also includes the  [-d](#5_3_6_)  option. In other words assemble only updated source modules and perform linking/relocation of the code afterwards.

*   Pass 1:  
    When the linking process begins with the first object module, it is examined for an  [ORG](#10_25_)  address to perform the absolute address relocation of all the object module machine code. The  [ORG](#10_25_)  (loading address for memory) will have to be defined in the first source file module. If not, the assembler will prompt you for it on the command line. The  [ORG](#10_25_)  address must be typed in hexadecimal notation. If you never use the  [ORG](#10_25_)  directive in your source files, you can always explicitly define one at the command line with the  [-r](#5_3_11_)  option.  
      
    The next step in the linking process is loading of the machine code from each object module, in the order of the specified modules. Pass 1 is completed with loading all local and global symbol definitions of the object modules. All relocatable address symbols are assigned the correct absolute memory location (based on  [ORG](#10_25_) ).  
    
*   Pass 2:  
    The address patching process. All expressions are now read and evaluated, then patched into the appropriate positions of the linked machine code.  
      
    When all expressions have been evaluated the machine code is completed and saved to a file named as the first source file module, and assigned the 'bin' extension.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_8_></a>

#### 5.3.8. -a : Combine -d and -b

Same as providing both  [-b](#5_3_7_)  (link/relocate object files) and  [-d](#5_3_6_)  (assemble only updated files).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_9_></a>

#### 5.3.9. -o\<binary-filename\> : Binary filename

Define another filename for the compiled binary output than the default source filename of the project, appended with the ".bin" extension.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_10_></a>

#### 5.3.10. -m : Create address map information file

When the linking process has been completed and the machine code saved to a file, the address map file is produced. This file contains the information of all relocated address symbols with their assigned absolute addresses. Further, an id is written that displays the scope of the symbols; local ('L') or global ('G'). The last item of each address map line, identified after the colon, is the name of the module which the symbol name belongs to.

The address map file is divided into two sections; the first produces the symbol list in alphabetical order (fast lookup of symbol names), the second in order of their address counterparts. The section is more used during a debugging session when an address in a disassembly needs to be identified with a name. The second list also gives a chronological view (composition) of the linked modules.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_11_></a>

#### 5.3.11. -r\<hex-address\> : Re-define the ORG relocation address

During the linking phase of the assembler the  [ORG](#10_25_)  address that defines the position in memory where the code is to be loaded and executed, is fetched from the first object module file. You can override this by specifying an explicit address origin by entering the  [-r](#5_3_11_)  option followed by an address in hexadecimal notation at the command line, e.g.:

    z80asm  [-b](#5_3_7_)  -r4000 file.asm

which specifies that your code is to be relocated for address 4000h (16384) onwards.

Using the  [-r](#5_3_11_)  option supersedes a defined  [ORG](#10_25_)  in the object file. You could for example have defined the  [ORG](#10_25_)  to 8000h (32768) in your first source file, then compiled the project. This would have generated machine code for memory location 8000h (segment 2 in the Cambridge  [Z88](#5_6_) ). Since the object files are generated it is easy to link them again with another  [ORG](#10_25_)  address by just using the  [-r](#5_3_11_)  option. The linking process does not alter the information in object files - they are only read. The same project could then easily be re-linked to another address, e.g.

    z80asm  [-b](#5_3_7_)  -r2000 file.asm


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_12_></a>

#### 5.3.12. -R : Generate address independent code

The  [Z80](#7_)  processor instruction set allows only relative jumps in maximum +/- 128 bytes using the JR and DJNZ instructions. Further, there is no program counter relative call-to-subroutine or jump-to-subroutine instruction. If you want a program to be address-independent no absolute address references may be used in jump or call instructions. If you want to program  [Z80](#7_)  address independent code you can only write small routines using the JR and DJNZ instructions. With a restricted interval of 128 bytes you can imagine the size of those routines! Programming of large applications using address independency is simply impossible on the  [Z80](#7_)  processor with the basic instruction set available. You can only define a fixed address origin ( [ORG](#10_25_) ) for your machine code to be loaded and executed from. However, there is one solution: before the code is executed an automatical address-relocation is performed to the current position in memory. This is done only once. The penalty is that the program fills more space in memory. This is unavoidable since information must be available to define where the address relocation has to be performed in the program. Further, a small routine must be included with the program to read the relocation information and patch it into the specified locations of the program. It is impossible to determine the extra size generated with a relocation table. We assume an extra size of 3 - 3.5K for a typical 16K application program.

You can generate address independent code using the  [-R](#5_3_12_)  option accompanied with the  [-a](#5_3_8_)  or  [-b](#5_3_7_)  option. There is no other requirements. The relocatable code may be useful for programmers using the Cambridge  [Z88](#5_6_)  who want to use machine code in the BBC BASIC application environment. This can easily be interfaced with the DIM statement to allocate memory for the machine code program, and issue a  [CALL](#10_2_)  or USR() to execute the machine code.

Please note that the linking process with the  [-R](#5_3_12_)  option addresses your code from 0 onwards. This is necessary when the runtime relocation is performed by the relocator (just before your program is executed). This can be examined by loading the address map file into a text editor.

The principle of relocation is in fact a self-modifying program. You cannot relocate a program that has been blown into an EPROM (cannot be modified). You may only execute relocatable programs in dynamic memory (RAM).

The relocator is built into the  [Z80](#7_)  Module Assembler. The relocation table is created during the linking phase. When all object modules have been linked and the appropriate machine code generated, the process is ended with first copying the relocator routine into the executable file, then a relocation table and finally the compiled machine code program. Any defined  [ORG](#10_25_)  in your code is superseded - this is not necessary in a relocatable program!

Two rules must be obeyed when using relocatable programs:

1.  The IY register must have been set up to point at the address where your program resides. The first code is in fact the relocator which manipulates your code on the basis of the IY register. If IY is not setup properly your machine code program will be relocated for an address it is not resided at. On execution your might then call a random address (on the basis of the random IY register).  
    
2.  Don't use the alternate register set for parameter passing between the caller (of your code) in the main code and the relocated program. The following registers are affected by the initial relocation process:

        AFBCDEHL/IXIY/........ same  
        ......../..../afbcdehl different

You still have all the main registers for parameter passing which is more than sufficient for average programming.

When your address-independent code is stored to the file, a message is displayed which informs the user of how many bytes the relocation header consists of. This constant is useful since it informs you of the distance between the relocation header and the start of your code. The map file automatically reflects the relocation header. All addresses of your code has been modified to include the relocation header. Please note that all addresses in the map file are defined from address 0. When your code is placed in an absolute memory address, and you need to perform a debugging session, you can find your specific label address by adding the constant from the map file to the memory origin of your code. The inbuilt relocator routine may be examined by extracting the "relocate.asm" file from the " [Z80](#7_) src.zip" compressed file resource.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_13_></a>

#### 5.3.13. -g : Create global address definition file

With this option it is possible to generate a  [DEFC](#10_6_)  directive definition file of all globally declared identifiers in a file project (declared with the  [XDEF](#10_27_)  directive). These global definitions are calculated from the specified  [ORG](#10_25_)  address (from first module or the  [-r](#5_3_11_)  option). This feature is very useful, if you want to get access to routines from a separate compilation. If the two code compilation were placed in different banks of the  [Z88](#5_6_) , it would be possible to know the correct address of a routine just by using the  [DEFC](#10_6_)  address definition previously compiled. We used this facility to access routines in the two 8K halves of the segment 0 debugged version. This debugger actually swaps one of the two 8K blocks in and out of segment 0 when needed to call an 'external' routine. Applications on the  [Z88](#5_6_)  may only access the upper 8K of segment 0. A 16K application therefore needs to be split in 8K halves and paged in when needed to be run in this area. Tuff!


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_14_></a>

#### 5.3.14. -D\<symbol\> : Define a static symbol

This option is useful if you want to produce conditional compilations. The symbol defined here will be active throughout the compilation. We used this feature to compile machine code on different computer platforms (QL, IBM, LINUX or  [Z88](#5_6_) ). Specify the symbol immediately after the option identifier, i.e.

    z80asm  [-b](#5_3_7_)  -DZ88 file.asm


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_15_></a>

#### 5.3.15. @\<project-file\> :1.3.16. Using a project file

Project files defines all file names of a project. The file name standard stored in a project file obeys the operating system notation.

Instead of specifying every module file name at the command line, a simple reference of a project file can be made instead. According to the rules of the specification of parameters you specify either your source file modules or use a project file. The project file specification is of course much faster. An example:

    z80asm  [-a](#5_3_8_)  main pass1 pass2 link asmdrctv z80instr

This command line will compile all specified module file names into a single executable file called "main.bin". However if a project file 'assembler' were created already containing the same file names, the command line would have been:

    z80asm  [-a](#5_3_8_)  @assembler

\- much easier!

A project file only contains file names. Each file name is separated by a newline character \\n. The new line character may be different on various computer platforms - but the assembler interprets it correctly. The contents of a project file may look like this:

    z80asm  
    z80pass1  
    z80pass1  
    modlink

Project files are easily created using a simple text editor.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_16_></a>

#### 5.3.16. -i\<library-file\> : Include library modules during linking/relocation

This option allows compilation time linking of external machine code, better known as library routines. Much, much programming time can be saved by producing a set of standard routines compiled into library files. These may then be included later in application project compilations. The command line option allows specification of several library files. For each library reference in an application module, all library files will be scanned for that particular module. The filename (inclusive directory path) of the library may be specified explicitly on the command line immediately after the  [-i](#5_3_16_)  identifier. If you omit the filename, a default library filename will be used by the assembler. This default filename is defined by creating the environment variable " [Z80](#7_) \_STDLIB=\<library-file\>. Please refer to your operating system documentation on how to create environment variables.

Library files are recognised by the ".lib" extension.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_17_></a>

#### 5.3.17. -L\<library-path\> : Add directory to search path for libraries

Tells the assembler where to look for library files.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_18_></a>

#### 5.3.18. -I\<include-path\> : Add directory to search path for INCLUDE

Tells the assembler where to look for  [INCLUDE](#10_18_)  files.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_19_></a>

#### 5.3.19. -x : Create a library

A library file is composed of object files surrounded by a few file structures. The library file format (and object file format) may be found at the end of this documentation. A library is simply a set of independent routines (that may refer to each other) put together in a sequential form. You may only specify a single  [-x](#5_3_19_)  option on the command line. A filename may be explicitly defined (including device and path information) to determine the storage location of the library. As in  [-i](#5_3_16_)  you may omit the filename to use the default filename identified by the " [Z80](#7_) \_STDLIB" environment variable. A library routine must be defined using a simple  [XLIB](#10_28_)  directive with an identical address name label definition. Please refer to further information later in this documentation. The " [Z80](#7_) lib.zip" contains the standard library with all corresponding source files. Have a look at them - they clearly displays how to compose a library routine.

One very important aspect of libraries is the time that the assembler spends searching through them. To optimize the search you should place your routines in a "topological" order, i.e. routines that access other library routines should be placed first. In most situations you avoid redundant sequential searching through the library.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_20_></a>

#### 5.3.20. -t\<number\> : Define tabulator distance for text output files

To save storage space the  [Z80](#7_)  cross assembler output files (listing, map, symbol and  [XDEF](#10_27_)  definition files) uses a tabulator control character instead of spaces. The benefit is about 30% compressed files.

The tabulator distance defines the distance of space between each tabulator it represents. The default value is 8 spaces per tabulator.

The tabulators are used to separate two columns of information. The first column contains a name of some sort. Since names have variable length, a size of the column is defined. The Assembler defines the size of the column by multiplying the current tabulator distance with 4, i.e. giving a default size of 4\*8 = 32 'spaces'. This is usually more than enough for most name definitions parsed from source files.

You may redefine the tabulator distance by using the  [-t](#5_3_20_)  option immediately followed by a decimal number, e.g. -t4 for defining a tabulator distance of 4. The width of the first column will then be 4\*4 = 16 'spaces'.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_21_></a>

#### 5.3.21. -RCMX000 : Support the RCM2000/RCM3000 series of Z80-like CPU's

This option disables assembly opcodes not available in the RCM2000/RCM3000 series of  [Z80](#7_) -like CPU's.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_22_></a>

#### 5.3.22. -plus : Support for the Ti83Plus

Defines how the  [INVOKE](#10_19_)  command is coded: either as a RST 28H instruction (option on) or as a regular  [CALL](#10_2_)  instruction (option off).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_23_></a>

#### 5.3.23. -C : Enable LINE directive

Enables the  [LINE](#10_21_)  directive to synchronize error message line numbers with the line numbers from the source file.

2\. An overview of assembler features and related files
-------------------------------------------------------


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_4_></a>

### 5.4. The Z88 operating system definition files

You will find header files containing all operating system definitions as defined in the  [Z88](#5_6_)  Developers' Notes V3 in the "OZdefc.zip" file. This makes the operating system interface programming a lot easier.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_5_></a>

### 5.5. The supplied standard library Z80 source files

We have supplied a standard library with useful routines for both beginners and professional machine code programmers. All source files are supplied for having the opportunity to study and improve the routines. However some routines are programmed especially for the  [Z88](#5_6_)  operating system and may not be of use for other  [Z80](#7_)  based computers unless thoroughly rewritten. The standard library source files may be found in the " [Z80](#7_) lib.zip" file.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_6_></a>

### 5.6. Z88 module assembler application source

We have supplied the complete source of the  [Z88](#5_6_)  module assembler application. This allows you to evaluate many aspects of programming applications on the  [Z88](#5_6_) . Further, most features of the assembler are mirrored in these source files; using directives, the free format of  [Z80](#7_)  mnemonics, library routine access, modular file design, labels, using expressions, comments, data structure manipulation and good programming design.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_7_></a>

### 5.7. File based compilation

This assembler is completely file based, i.e. all parsing and code generation is manipulated via files on storage medias such as harddisks or floppy disks (or file based RAM-discs).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_8_></a>

### 5.8. Modular source file design

A compilation may be split into individual source files that either can be linked together during assembly as a single module or assembled as separate source file modules. Separate source file modules saves compilation time and memory. Further, this design is much more straightforward and much more logically clear in a design phase of a large compilation project than one huge kludge of a source file.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_9_></a>

### 5.9. Scope of symbols in source modules

All source modules may refer to each others symbols by using  [EXTERN](#10_12_)  directives. This means that you refer to external information outside the current source module. The opposite of an external module reference is to declare symbols globally available using a  [PUBLIC](#10_26_)  directive, i.e. making symbols available to other source modules. Finally it is possible to have local symbols that are not known to other source modules than the current. A label or constant that has not been declared with  [EXTERN](#10_12_) ,  [PUBLIC](#10_26_)  or  [GLOBAL](#10_14_)  is local to the module.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_10_></a>

### 5.10. Using arithmetic and relational expressions

All directives that require a numerical parameter or  [Z80](#7_)  mnemonics that use an integer argument may use expressions. Expressions may be formed by all standard arithmetic operators and relational operators. Even binary operators are implemented. All expressions may contain external identifiers and is automatically resolved during the linking phase. Only certain directives require compilation time evaluable expressions.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_11_></a>

### 5.11. Source file manipulation

To allow the Assembler to execute in multitasking environments such as LINUX and QDOS, all source input files are opened as shared I/O. This allows other programs to access the source files while the assembler is working. All output files (both text and binary files) are opened/created for exclusive use; other programs will have no access to those files until they have been closed.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_12_></a>

### 5.12. Free format of assembler source files

The source files may be written in a free format. No fixed position columns as needed as in the COBOL programming language. All text may be typed in mixed case (the assembler converts all text input to uppercase). Tabulators may be used freely (instead of spaces which also saves source file space) to suit the programmers own habits of structured text layouts. However, one rule must be obeyed: syntax of  [Z80](#7_)  assembler mnemonics and most directives must be completed on individual lines. Text files using different OS dependant line feed standard are parsed properly; line feed types CR, LF or CRLF are automatically recognized. So you can easily compile your sources from Linux/UNIX on an MSDOS platform.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_13_></a>

### 5.13. Specification of filenames

Specification of file names in source files are always enclosed in double quotes. The assembler just collects the filename string and uses this to open a file. This way all filename standards may be used as defined on different operating system platforms.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_14_></a>

### 5.14. Including other source files into the current source file

The need for header file information such as operating system constants or data structures is often indispensable for source file modules. Instead of copying the contents of those files into each module, it is possible to include them at run time (during parsing). Infinite include file levels are permitted, i.e. included files calling other files.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_15_></a>

### 5.15. Conditional assembly in source file modules

Large compilation projects often need to compile the application in several variations. This can be achieved with enclosing parts of the source with conditional directives for the different variations of the application. This may also be useful if the assembler source is ported to several platforms, where inclusion of other source files (such as header files) are using different filename standards. The conditional directives  [IF](#10_15_) ,  [IFDEF](#10_16_) ,  [IFNDEF](#10_17_) , ELSE, and ENDIF may be nested into infinite levels.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_16_></a>

### 5.16. Undocumented Z80 instruction code generation

We have included the syntax parsing and code generation of the undocumented  [Z80](#7_)  instructions for the sake of completeness. However, IM 2 interrupts must be disabled before they are executed (an interrupt may otherwise occur in the middle of the instruction execution). Many games on the ZX Spectrum have used them to protect the code from prying eyes. The  [Z88](#5_6_)  native debugger code uses some of the undocumented instructions for fast access to register variables.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_17_></a>

### 5.17. Object file generation

The  [Z80](#7_)  Module Assembler generates object files that contains the compressed version of an assembled source module. The information herein contains declared symbols (local, global and external), expressions, address origin, module name and machine code. The object file modules are much smaller than their source file counterparts (often smaller than 2K).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_18_></a>

### 5.18. Transfer of object files across platforms

The  [Z80](#7_)  Module Assembler is already implemented on several different computer platforms. You may freely transfer the object files and use them as a part of another cross-compilation. There is no system-dependent information in the object files.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_19_></a>

### 5.19. Date stamp controlled assembly

To avoid unnecessary compilation of source file modules, it is possible to let the assembler compile only recently updated source file modules by comparing the date stamp of the source and the object file modules. Source file modules that are older than object file modules are ignored. This facility is indispensable in large compilation projects.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_20_></a>

### 5.20. Listing files

The assembler may generate listing files that contain a copy of the source file with additional code generation information of  [Z80](#7_)  mnemonics dumped in hexadecimal format. The listing files are formatted with page headers containing time of assembly and the filename. Line numbers are included which corresponds to the source file lines.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_21_></a>

### 5.21. Symbol information

All symbol generated values used in source modules may be dumped to the end of the listing file or as a separate symbol file. If the symbol table is dumped into the listing file, each symbol will be written with page references of all occurrences in the listing file. Address symbols (labels) are addressed relative to the start of the module. Symbol constants are written as defined in the source. The symbol table is written in alphabetical order with corresponding values in hexadecimal format.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_22_></a>

### 5.22. Linking and relocation of object modules into executable Z80 machine code

To obtain an executable  [Z80](#7_)  machine code file it is necessary to link all assembled object modules and relocate them at a defined address, where the code is to be executed at in the computers' memory. The linking & relocation is performed automatically after completed assembly of all specified source file modules. The  [ORG](#10_25_)  relocation address is specified in the first object module.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_23_></a>

### 5.23. Address map files

The address map is invaluable information during a debugging session of your compiled program. This file contains all symbolical address labels with their generated address constants after a completed linking/relocation of all modules into executable machine code. The map file is ordered in two groups; the first list contains all symbol names ordered alphabetically with corresponding address constants, the second list contains all symbols ordered by their address value (in chronological order).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_24_></a>

### 5.24. Symbol address definition files

As with address map files this contains information of globally declared symbolical address labels, relocated to their absolute position as for the compiled machine code file. However, the format is completely different; all symbols are created as constant definitions to be included as a header file into another source file and assembled. This is useful if you want to call subroutines compiled separately in another project (originated in a different memory setup).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_25_></a>

### 5.25. Error files

Error files are created by the assembler during processing. If any errors should occur, they will be written to stderr and copied to this file containing information of where the error occurred in the source module. If no errors were found, the error file is automatically closed and deleted.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_26_></a>

### 5.26. Creating and using object file libraries for standard routines

Machine programmers often re-use their standard routines. We have implemented a file format for generating libraries using the existing object file modules. Using a simple set of rules makes it very easy to create your own libraries from your source file modules. Documentation of the library file format is included in this documentation. At command line infinite number of libraries may be specified. All will be searched during linking of your object modules for referenced library routines.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_></a>

## 6. Executing the cross assembler and environment variables

The following text describes how to execute the assembler and defining the environment variables used by the assembler.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_1_></a>

### 6.1. The environment variables

The assembler uses two environment variables:

*   " [Z80](#7_) \_STDLIB" define the default filename of the standard library filename.
*   " [Z80](#7_) \_OZFILES" define the default path of the standard  [Z88](#5_6_)  OZ system headers.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_2_></a>

### 6.2. Running in the QDOS/SMSQ operating system environment

z80asm may be compiled using the C68 compiler developed by the Walker brothers. You also need the 'env\_bin' file. This is necessary to install into the operating system before using the assembler. It contains the environment variable facility integrated on UNIX, MS-DOS and many other operating systems. Include the following line into your BOOT upstart program:

    LRESPR win1_ext_env_bin

The device and path 'win1\_ext\_' is just an example of where to store your system extension file. You may have it on your BOOT disk as well. If you don't have ToolKit 2 on your system use the following line in your BOOT program:

    envext=RESPR(1024):LBYTES win1_ext_env_bin,envext: [CALL](#10_2_)  envext

Use the following in your BOOT file to set the environment variables:

    SETENV "Z80_OZFILES=win1_z80_src_oz_"  
    SETENV "Z80_STDLIB=win1_z80_standard_lib"

The example file names are only a demonstration. Change them as necessary.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_></a>

### 6.3. Running in the LINUX/UNIX operating system environment

This program can be executed on the LINUX operating system and any other UNIX. The sources are strictly ANSI C and uses only library calls that are described in Brian W. Kernighan and Dennis M. Ritchie C books. The important thing to remember before compiling it under UNIX, is in which direction the integers are stored by the processor architecture; the famous Big Endian and Little endian concept. The config.h file uses a "ENDIAN" definition when z80asm must interpret integers in big endian order. Please set this definition according to your system's processor architecture. Most people uses the Intel processor when running Linux - this is a little endian architecture (so you don't need the ENDIAN define...).

You can compile z80asm using GNU C compiler simply by entering the following on your command line:

    gcc  [-o](#5_3_9_)  z80asm -O2 *.c

The environment variables needed by z80asm are easily added in your accounts login script, ".profile" or ".bash\_profile" in your home directory.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_4_></a>

### 6.4. Running in the MS-DOS operating system environment

This program can be executed on all MSDOS operating systems using the INTEL 8086 processor up to high speed 80386/80486/Pentium processors on IBM & compatible computers. Add the source files to your favorite C compiler system in MSDOS or Windows (here you need to specify compilation options o a console application).

The environment variables are easily put into your AUTOEXEC.BAT file. Simply specify:

    SET Z80_OZFILES=C:\ [Z80](#7_) \OZ\
    SET Z80_STDLIB=C:\ [Z80](#7_) \STANDARD. [LIB](#10_20_) 

Choose your own settings if you like.

If you want the assembler to be accessible in any path, add the path location of the assembler in the PATH search line:

    SET PATH=%PATH%;C:\ [Z80](#7_) 


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_></a>

## 7. Z80 module assembler file types


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_1_></a>

### 7.1. The assembler file types and their extension names

The  [Z80](#7_)  Module Assembler uses several different filename extensions to distinguish the type of files processed. The base name of the source file is used to create the various assembler output file types. The following chapters explains the available files.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_2_></a>

### 7.2. The file name extension identifier

The file name extension identifier may be different from platform to platform. UNIX has no defined standard. MSDOS and TOS uses '.'. QDOS uses the '\_' identifier. SMSQ also allows the '.' extension identifier.

The Assembler implemented on the supplied platforms is defined with the correct extension identifier. You can see this on the Assembler help page (executing the program with no parameters).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_3_></a>

### 7.3. File types


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_3_1_></a>

#### 7.3.1. The source file extension, asm

The extension for assembler mnemonic source files is 'asm'. Source files are specified by the user with or without the extension - whatever chosen, the assembler will investigate automatically what is needed to read the source files.

You may override the default extension with the  [-e](#5_3_2_)  option.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_3_2_></a>

#### 7.3.2. The object file extension, obj

The extension for object files is 'obj'. The base file name is taken from the corresponding source file name. This file is generated by the assembler from parsing the source file and contains intermediate generated machine code, an address origin of the machine code, symbol information and expressions.

You may override the default extension with the  [-M](#5_3_3_)  option.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_3_3_></a>

#### 7.3.3. The error file extension, err

The extension for error files is 'err'. Before beginning processing the source files, an error file is created. If any errors should occur, they will be written to this file containing information of where the error occurred. If no error were found, the error file is automatically closed and deleted.

Error files are simple text files that can be loaded by any text editor for evaluation.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_3_4_></a>

#### 7.3.4. The listing file extension, lst

The extension for listing files is 'lst'. The base file name is taken from the corresponding source file name. This file is generated by the assembler and contains a hexadecimal output of the generated machine code that corresponds to the  [Z80](#7_)  mnemonic instruction or directive, followed by a copy of the original source line. If selected, the symbol table is dumped at the end of the listing file.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_3_5_></a>

#### 7.3.5. The symbol file extension, sym

The extension for symbol table files is 'sym'. The base file name is taken from the corresponding source file name. The symbol table file contains information about the defined and used symbols of the source file and their generated values (labels and constants). The symbol file is only created if listing file output is disabled.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_3_6_></a>

#### 7.3.6. The executable file extension, bin

The extension for executable  [Z80](#7_)  machine code files is 'bin'. The base file name is taken from the first specified source file name at the command line (or project file). This is the linked and relocated output of object files and may be executed by the  [Z80](#7_)  processor. You may override this default behaviour by using the  [-o](#5_3_9_)  option and specify your own output filename (and extension).

You may override this default behavior by using the  [-o](#5_3_9_)  option and specify your own output filename and extension.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_3_7_></a>

#### 7.3.7. The address map file extension, map

The extension for address map files is 'map'. The base file name is taken from the first specified source file name at the command line (or project file). This file is generated by the assembler and contains a list of all defined address labels from all linked/relocated modules with their calculated (absolute) address in memory.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_3_8_></a>

#### 7.3.8. The definition file extension, def

The extension for global address label definition files is 'def'. The base file name is taken from the first specified source file name at the command line (or project file). This file is generated by the assembler and contains a list of all globally declared address labels with their calculated (absolute) origin address, fetched only during assembly of source file modules. The format of the list contains constant definitions (addresses) and may be parsed e.g. as include files for other projects.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_3_9_></a>

#### 7.3.9. The library file extension, lib

Library files are identified with the 'lib' extension. Library files may be created using the  [-x](#5_3_19_)  option. Library may be included into application code during linking of object modules with the  [-i](#5_3_16_)  option.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_></a>

## 8. Compiling files


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_1_></a>

### 8.1. The assembler compiling process

The  [Z80](#7_)  Module Assembler uses a two stage compilation process; stage 1 parses source files and generates object files. Stage 2 reads the object files and links the object file code, completes with address patching and finishes with storing the executable code.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_1_1_></a>

#### 8.1.1. Stage 1, parsing and code generation of all source files, object file generation

A source file is being parsed for  [Z80](#7_)  mnemonics and directives. An object file is created to hold information of module name, local, global and external symbol identifiers, expressions and the intermediate code generation (but address and other constant information). During pass 1 all  [Z80](#7_)  mnemonics are parsed and code is generated appropriately. All expressions are evaluated; expressions that contain relocatable address symbols or external symbol are automatically stored into the object file. Expressions that didn't evaluate are preserved for pass 2. When a source file has been read successfully to the end, pass 2 is started. During pass 2 all non-evaluated expressions from pass 1 are re-evaluated and stored to the object file if necessary. Errors are reported if symbols are still missing in expressions. When all expressions are evaluated and no errors occurred, all "touched" symbols (used in expressions) are stored into the object file, with scope, type and value. Then, the module name and generated code is stored to the object file. Various file pointers to sub-sections of the object file is resolved. The completion of stage 1 is to produce the symbol table output (either appended to listing file if selected or as a separate file).

This process is performed for all specified source modules in a project.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_1_2_></a>

#### 8.1.2. Stage 2, linking object files and library modules, producing executable code

Pass 1 of the linking loads information from each object file in the project; the  [ORG](#10_25_)  address is fetched, identifiers (resolving scope, and absolute values) loaded, and machine code linked. During this pass all external library modules are fetched and linked with the object modules (if a library is specified from the command line). When all modules have been loaded, pass 2 begins. Pass 2 then reads each expression section from all object modules (including library modules), evaluates them and patches the value into the appropriate position of the linked machine code. When all expressions have been evaluated successfully the executable code is stored. If selected, the address map file is produced from the current symbol table resided in the data structures of the assembler's memory is stored to a text file.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_2_></a>

### 8.2. File names

Specification of file names follows the convention used on the various platforms that the assembler is ported to. Please read your operating systems manual for more information.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_3_></a>

### 8.3. Portability of assembler file names

If you are going to port your  [Z80](#7_)  Module Assembler files across several platforms a few hints may be worth considering:

Avoid special symbols in file names like '\_', '#' and '.' . They may have special meaning on some operating system platforms. Use only 7-bit standard ASCII letters in file names ('A' to 'z'). Non English language letters are not always allowed, and further they may not be interpreted correctly when ported to another platform. Avoid too long file names. Some operating systems have boundaries for length of filenames in a directory path. For example MS-DOS only allows 8 characters in a file name (followed by an extension). Others may have no boundaries.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_4_></a>

### 8.4. Source file structure

The composition of a source file module is completely free to the programmer. How he chooses to place the source code on a text line has no effect of the parsing process of the assembler. The linefeed interpretation is also handled by z80asm - it understands the following formats:

*   \<LF\> (used by QDOS/SMSQ/UNIX/AMIGA);
*   \<CR\>\<LF\> (used by MSDOS);
*   \<CR\> (used by  [Z88](#5_6_) /MacIntosh).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_5_></a>

### 8.5. Using local, global and external symbols

In larger application projects it is unavoidable to use a modular programming design, i.e. splitting the source into several individual files. This approaches the popular top - down design where you can isolate the problem solving into single modules. The outside world just needs to know where the routine is to be called by linking the modules with a few directives.

In the  [Z80](#7_)  Module Assembler you only need two directives to accomplish just that: the  [XREF](#10_29_)  and  [XDEF](#10_27_)  directives.

 [XREF](#10_29_)  declares a symbol to be external to the current source file module. This tells the assembler that all expressions using that symbol is not to be evaluated until the compiled object modules are to linked and relocated together. An expression that contains this symbol is simply stored into the object file.

 [XDEF](#10_27_)  declares a symbol to be created in this module and made globally available to other modules during the linking/relocation phase. All expressions that contain a globally declared symbol is automatically stored into the object file.

When a symbol is created and is neither declared external or global, it is implicitly defined as local to the current source module. The symbol is then only available to the current module during linking/relocation.

If you want to access (external) library modules from a library, use the  [LIB](#10_20_)  directive followed by the name of the routine. Several routine names may be specified separated by a comma.

During the linking process all external and corresponding global symbols are resolved. If two identical global identifiers are loaded by the linking process, the most recently loaded identifier is used by the linker.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_6_></a>

### 8.6. Defining symbol names

Good programming involves a structured approach to mnemonic identification of names in subroutines, variables, data structures and other constants. The  [Z80](#7_)  Module Assembler gives you several possibilities. The easiest and frequently used one is  [DEFC](#10_6_)  (Define Constant). We have supplied a complete set of header files (the "OZdefc.zip" file) containing the  [Z88](#5_6_)  operating system manifests as defined in the Developers' Notes V3 (the "devnotes.zip" file) which just contains  [DEFC](#10_6_)  directives.

Each  [DEFC](#10_6_)  directive is followed by an identifier name, followed by a = symbol and then an evaluable constant expression (usually just a constant). Constant definitions are usually operating system manifest or other frequently used items. They are put into separate source files and later inserted into main source files using the  [INCLUDE](#10_18_)  directive.

Though  [DEFC](#10_6_)  resolves most needs, it may be necessary to define variable areas or templates containing names with an appropriate size tag (byte, word or double word). This is possible using the  [DEFVARS](#10_11_)  directive. Here you may specify as many names as needed in the group. Then, it is easy to add, rearrange or delete any of the variable names - only a few modifications and then just re-compiling the necessary source files that use the templates. This would be a nightmare with  [DEFC](#10_6_) , since you have to keep track of the previous and next name in the group in addition to count the size of all names. All this is managed by  [DEFVARS](#10_11_)  automatically. Have a look at the syntax in the Directive Reference section.

With advanced  [Z80](#7_)  programming you cannot avoid dynamic data structures like linked lists or binary trees. The fundamentals for this are known as records in PASCAL or structures in C.  [DEFVARS](#10_11_)  is well suited for this purpose. Defining each  [DEFVARS](#10_11_)  group with 0 automatically generates offset variables. The last name then automatically defines the size of the data structure. Again, refer to the directive reference for a good example.

A third possibility for an easy definition of symbols is to use the  [DEFGROUP](#10_8_)  directive. Here you're able to define a set of symbols equal to an enumeration. It follows the same principles as for C's ENUM facility. The default starts at 0 and increases by 1. If you choose, a specific identifier may be set to a value, which then can set the next enumeration sequence. Again, this directive has been made to implement an easy way of defining symbols and providing a simple method to alter the identifier group. Please refer to the directive reference for an example.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_7_></a>

### 8.7. Comments in source files

As always, good programming requires good documentation. Without comments your programs lose overview and logic. Machine code is especially hard to follow - have you tried to look at a piece of code 2 years after implementation AND without any comments? HORRIBLE! There is never too many comments in machine code - we especially like to use high level language as comments - it avoids unnecessary text and logic is much more clear.

Comments in  [Z80](#7_)  source files are possible using a semicolon. When the assembler meets a semicolon the rest of the current source line is ignored until the linefeed. Parsing will then commence from the beginning of the line. The semicolon may be placed anywhere in a source line. As stated you cannot place mnemonics after the semicolon - they will be ignored. The  [Z80](#7_)  parser will in many places accept comments without a semicolon has been set - but don't rely on it. Better use a semicolon. The context is much clearer. The following is an example on how to use comments in  [Z80](#7_)  source files:

    ; **********************  
    ; main menu  
    ;  
    .mainmenu   call window   ; display menu  
                call getkey   ; read keyboard  
                ret           ; action in register A


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_8_></a>

### 8.8. Defining symbolic address labels

The main reason for using an assembler is to be able to determine symbolical addresses and use them as reference in the code. These are defined by a name preceded with a full stop, or followed by a colon. It is allowed to place a mnemonic or directive after an address label. An address label may be left as a single statement on a line - you may of course use comment after the label. The following is a label definition:

    ; *****************  
    ; routine definition  
    .mainmenu call xxx   ; a label is preceded with '.'
    endmain:  ret                ; or followed by ':'

It is not allowed to position two labels on the same line. However, you may place as many label after each other - even though no code is between them. They just receive the same assembler address.

It is not allowed to specify two identical address labels in the same source file.

If you want to declare an address globally accessible to other modules, then use  [PUBLIC](#10_26_)  for the address label definition, otherwise the label definition will be interpreted as a local address label.

     [PUBLIC](#10_26_)  mainmenu  
    ...  
    .mainmenu ; label accessible from other modules with  [EXTERN](#10_12_) 

You may use before or after the label - z80asm automatically handles the scope resolution as long as you use  [PUBLIC](#10_26_)  to define it as globally accessible.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_9_></a>

### 8.9. Writing Z80 mnemonic instructions

All  [Z80](#7_)  instructions may be written in mixed case, lower or upper case - you decide! How you separate opcode words, register names and operands is your choice. Only a few rules must be obeyed:

1.  Each instruction mnemonic must be completed on a single line.
2.  The instruction identifier must be a word, i.e. don't use space between  [CALL](#10_2_) .
3.  Register identifiers must be a word, ie. HL not H L.

A few examples which all are legal syntax:

    Ld HL   , 0       ; comment  
    ld       hl, $fFFF;comment  
    caLL ttttt


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_10_></a>

### 8.10. Optional Z80 mnemonic instruction syntax

The instructions below allow additional specification of the accumulator register. Zilog standard convention is to only specify the source operand. Sometimes it is better to specify the accumulator to make the instruction look more clear, and to avoid confusion. After all, they specified it for "add", "adc" and "sbc".

    sub a,r  
    sub a,n  
    sub a,(hl)  
    sub a,(ix+0)  
    sub a,(iy+0)

this syntax applies also to "and", "or", "xor" & "cp"


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_11_></a>

### 8.11. The undocumented Z80 instructions

We have included the parsing and code generation of the undocumented  [Z80](#7_)  instructions. They are as follows:

    LD   r,IXL  ; r = A,B,C,D,E,IXL,IXH  
    LD   r,IXH  
    LD   IXL,n  ; n = 8-bit operand  
    LD   IXH,n  
  
    ADC  A,IXL  
    ADC  A,IXH  
    ADD, AND, CP, DEC, INC, OR, SBC, SUB, XOR ...  
  
    SLL  r   ; r = A,B,C,D,E,H,L  
    SLL  (HL)
    SLL  (IX+d)
    SLL  (IY+d)

SLL (Shift Logical Left)

SLL does shift leftwards but insert a '1' in bit 0 instead of a '0'.

Except for the SLL instruction all have bugs related to an interrupt being able to occur while the instructions are decoded by the processor. They are implemented on the chip, but are reported to be unreliable. We have used some of them in our debugger software for the  [Z88](#5_6_) . Until now the code has been running successfully on all our  [Z88](#5_6_)  computers.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_12_></a>

### 8.12. Referencing library routines

When you need to use a library routine in your application code, you need to do two things; include a library file at the assembler command line with the  [-i](#5_3_16_)  option and refer to the library routine in your source file using the  [LIB](#10_20_)  directive followed by the name of the library routine, e.g.

     [LIB](#10_20_)  malloc, free

which will declare the two names "malloc" and "free" as external identifiers to the current source file module. Please note that you can declare the names before or after they actually are referred in your program source,. Failing to use the  [LIB](#10_20_)  directive will interpret labels as local symbols to that source file module. When the parser meets the instruction that uses one of the above names in a parameter, the parameter "expression" is automatically saved to the object file for later processing.

During the linking phase of all the object files the specified library file is automatically scanned for "malloc" and "free" and included into the application code when found.

Much application programming can be saved in "re-inventing the wheel" if you store frequently used standard routines in libraries. We have supplied a comprehensive set of library routines that were created along the development of the Assembler Workbench application EPROM. Use them as appropriate and add new ones. We are interested in your work - if you feel that you have made some good routines that others may benefit from, e-mail them to us and we will include them into the standard library.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_13_></a>

### 8.13. Creating/updating libraries

Creating libraries is an inbuilt feature of the assembler. The following steps are necessary to create a library:

1.  Define a project file containing all filenames (without extensions) in your directory that contains all library routines (the easiest method since you later can move all files to another directory). Remember to arrange the filename in a topological order, i.e. library routines that access other routines must be placed first. This optimizes the searching through a library during the linking process.  
    
2.  Each library source module uses the  [XLIB](#10_28_)  directive to define the name of the routine. The same name must be used for the address label definition. If your library uses other library routines then declare them with the  [LIB](#10_20_)  directive. Please note that a library routine creates the module name itself (performed by  [XLIB](#10_28_)  automatically). The module name is used to search for routines in a library.  
    
3.  The command line contains the  [-x](#5_3_19_)  option immediately followed by your filename. If you don't specify a filename, the default standard filename is used (defined by the  [Z80](#7_) \_STDLIB environment variable). Then you need to specify your project filename preceded by '@'.

For example:

    z80asm -xiofunctions @iofunctions

will create a library "iofunctions.lib" in the current directory (also containing all library source files). The project file is "iofunctions" also in the current directory.

Please note that no binary file is created (a library is NOT an executable file), but a collection of object files sequentially organized in a file.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_14_></a>

### 8.14. Referencing routines in other compiled projects

It may be necessary in some situations to get access to routines previously compiled in another project. This implies however a knowledge of their absolute addresses during linking. This information is stored in the map file, but not accessible in a form suitable to be parsed by the assembler. However, this is possible in using the  [-g](#5_3_13_)  option at the assembler command line. The action performed creates a  [DEFC](#10_6_)  list file of address labels that have been declared as globally available (using the  [XDEF](#10_27_)  directive). Only compiled source files are included in the list. If you were using the  [-a](#5_3_8_)  option (compile only updated source files) and no files were updated then the  [-g](#5_3_13_)  file would be empty. If you would like a complete list of all global routines then it is needed to compile the whole project (using the  [-b](#5_3_7_)  command line option).

When the file is generated, it can easily be  [INCLUDE](#10_18_) 'd in another project where your routines may access the external routines. You might do this in two ways:

1.  Including the file in every source module that needs to access external routines. This may be the easiest solution if you're only going to need external access in one or two source modules. With many external calls in different module of the current project it requires much altering of files.  
    
2.  Creating a new source file that is part of your project. This file could easily be the first file in your project but could just as well be placed anywhere in your project. Declare each external name that is needed somewhere in your project as  [XDEF](#10_27_) , meaning that all names to be included are globally accessible from this module. Then specify the  [INCLUDE](#10_18_)  of the  [DEFC](#10_6_)  list of the other project file. As the names get loaded, they become global definitions. All other definitions will be ignored and not stored to the object file (they are not referred in the source module). All other modules just need to specify the external names as  [XREF](#10_29_) . During linking they all get resolved and your code has access to external routines from a previously compiled project.

Whenever the previous project has been re-compiled (and issued with  [-g](#5_3_13_)  option) there is a possibility that routine addresses has changed. You therefore need to recompile the extra source module in your project to get the new identifier values - the rest of your compilation is unaffected (due to the  [XREF](#10_29_)  directives). Only the linking process gets the new proper addresses. In example 1) you had to recompile all source files that would have used an  [INCLUDE](#10_18_)  of the  [DEFC](#10_6_)  list file. In example 2) only one file had to be recompiled.

The principle of external addresses was used to compile the debugger version to be resided in segment 0 (into the upper 8K). The actual size of the debugger code uses 16K, but was split into two separate halves to fit into the upper 8K of segment 0. Each of the 8K code-segments had to get access to the other 8K block. The solution was the  [-g](#5_3_13_)  option and cross referencing using  [XREF](#10_29_)  and an additional source module (containing the  [XDEF](#10_27_)  declarations) that included the  [-g](#5_3_13_)  list file of the other project compilation.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_></a>

## 9. Using expressions

Expressions are almost unavoidable in source files. They define and explain things much clearer than just using a constant. The  [Z80](#7_)  Module Assembler allows expressions wherever a parameter is needed. This applies to  [Z80](#7_)  mnemonic instructions, directives and even in character strings. The resulting value from an evaluated expression is always an integer. All expressions are calculated internally as 32-bit signed integers. However, the parameter type defines the true range of the expression. E.g. you cannot store a 32-bit signed integer at an 8-bit LD instruction like LD A, \<n\> . If a parameter is outside an allowed integer range an assembly error is reported. Finally, no floating point operations are needed by the assembler. There is no real standard on  [Z80](#7_)  based computers.

Whenever an integer is stored in a  [Z80](#7_)  file, the standard Zilog notation is used, i.e. storing the integer in low byte, high byte order (this also applies to 32-bit integers). This standard is also known as little endian notation (also used by INTEL processors).

Expressions may be formed as arithmetic and relational expressions. Several components are supported: symbol names (identifiers), constants, ASCII characters and various arithmetic operators.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_1_></a>

### 9.1. Constant identifiers

Apart from specifying decimal integer numbers, you are allowed to use hexadecimal constants, binary constants and ASCII characters. The following symbols are put in front of the constant to identify the type:

    $ : hexadecimal constant, e.g. $4000 (16384).  
    @ : binary constant, e.g. @11000000 (192).  
    ' ' : ASCII character, e.g. 'a'.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_2_></a>

### 9.2. Arithmetic operators

All basic arithmetic operators are supported: addition, subtraction, multiplication, division and modulus. In addition binary logical operators are implemented: binary AND, OR and XOR.

    + : addition, e.g. 12+13  
    - : unary minus, subtraction. e.g. -10, 12-45  
    * : multiplication, e.g. 45*2 (90)  
    / : division, e.g. 256/8 (32)  
    % : modulus, e.g. 256%8 (0)  
    ** : power, e.g. 2**7 (128)  
    & : binary AND, e.g. 255 & 7 (7)  
    | : binary OR, e.g. 128 | 64 (192)  
    ^ : binary XOR, e.g. 128 ^ 128 (0)  
    ~ : binary NOT, e.g. (~0xAA) & 0xFF (0x55)

Arithmetic operators use the standard operator precedence, shown from highest to lowest:

    constant identifiers  
    () ** */% +-&|^~

If you want to override the default operator precedence rules, use brackets ().


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_3_></a>

### 9.3. Relational operators

With relational operators you may form logical expressions resulting in true or false conditions. The resulting value of a true expression is 1. The resulting value of a false expression is 0. These operators are quite handy when you need to perform complex logic for conditional assembly in  [IF](#10_15_) -ELSE-ENDIF statements. The following relational operators are available:

    = : equal to  
    <> :not equal to  
    < : less than  
    > : larger than  
    <= : less than or equal to  
    >= : larger than or equal to  
    ! : not

You may link several relational expressions with the binary operators AND, OR and XOR. You have all the possibilities available!

It is perfectly legal to use relational expressions in parameters requiring an arithmetic value. For example:

    LD A, (USING_IBM = 1) | RTMFLAGS


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_4_></a>

### 9.4. The ASMPC standard function

In occasional circumstances it may be necessary to use the current location of the assembler program counter in an expression e.g. calculating a relative distance. This may be done with the help of the ASMPC identifier. An example:

    .errmsg0  [DEFB](#10_3_)  errmsg1 - ASMPC - 1 , "File open error"  
    .errmsg1  [DEFB](#10_3_)  errmsg2 - ASMPC - 1 , "Syntax error"  
    .errmsg2

Here, a length byte of the following string (excluding the length byte) is calculated by using the current ASMPC address value.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_5_></a>

### 9.5. Symbol identifiers in expressions

Apart from using integer constants in your expressions, names are allowed as well. This is frequently used for symbolical address label references (both external and local).

Forward referencing of symbols is not really something that is important in evaluating expressions. The logic is built into the assembler parser. If an expression cannot be resolved in pass 1 of source file parsing, it will try to re-evaluate the failed expression in pass 2 of the parsing. If it still fails a symbol has not been found ( [XREF](#10_29_)  and  [LIB](#10_20_)  external symbols are handled during the linking phase).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_></a>

## 10. Directive reference

The  [Z80](#7_)  Module Assembler directives are used to manipulate the  [Z80](#7_)  assembler mnemonics and to generate data structures, variables and constants. You are even permitted to include binary files while code generation is performed.

As the name imply they direct the assembler to perform other tasks than just parsing and compiling  [Z80](#7_)  instruction mnemonics. All directives are treated as mnemonics by the assembler, i.e. it is necessary that they appear as the first command identifier on the source line (NOT necessarily the first character). Only one directive is allowed at a single source line. Even though they are written as CAPITALS in this documentation they may be written in mixed case letters in your source files.

Since the directives cover very different topics of assembler processing, each directive will be explained in detail, identified with a header description for each text section. The following syntax is used:

    <> : defines an entity, i.e. a number, character or string.  
    {} : defines a an optional repetition of an entity.  
    [] : defines an option that may be left out.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_1_></a>

### 10.1. BINARY "filename"

 [BINARY](#10_1_)  loads a binary file at the current location. This could for example be a static data structure or an executable machine code routine. The loaded binary file information is included into the object file code section. The assembler PC is updated to the end of the loaded binary code.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_2_></a>

### 10.2. CALL\_OZ \<expression\>

The \<expression\> may be a 16-bit expression and must evaluate to a constant. This is an easy interface call to the  [Z88](#5_6_)  operating system. This is an advanced RST 20H instruction which automatically allocates space for the size of the specified parameter (either 8-bit or 16-bit). Code is internally generated as follows:

    RST $20  
     [DEFB](#10_3_)  x ; 8-bit parameter

or

    RST $20  
     [DEFW](#10_4_)  x ; 16-bit parameter


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_3_></a>

### 10.3. DEFB \<8-bit expr\>,{\<8-bit expr\>} (-128; 255)

Stores a sequence of bytes (8-bits) at the current location. Expressions may be used to calculate the values.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_4_></a>

### 10.4. DEFW \<16-bit expr\>,{\<16-bit expr\>} (-32768; 65535)

Stores a sequence of words (16-bits) in low byte - high byte order (little endian) at the current location. Expressions may be used to calculate the values.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_5_></a>

### 10.5. DEFL \<32-bit expr\>,{\<32-bit expr\>} (-2147483647; 4294967295)

Stores a sequence of double-words (32-bits) in low byte - high byte order (little endian) at the current location. Expressions may be used to calculate the values.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_6_></a>

### 10.6. DEFC name=\<32-bit expression\>{, name=\<32-bit expression\>}

Define a symbol variable, that may either be a constant or an expression evaluated at link time. The allowed range is a signed 32-bit integer value. All standard  [Z88](#5_6_)  operating system header files use  [DEFC](#10_6_) 


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_7_></a>

### 10.7. DEFM \<string expression\>|\<8-bit expr\>,...

 [DEFM](#10_7_)  stores a string constant at the current location. Strings are enclosed in double quotes, e.g. "abcdefgh". Strings may be concatenated with byte constants using commas. This is useful if control characters need to be a part of the string and cannot be typed from the keyboard. Several strings and byte expressions may be concatenated, e.g.

     [DEFM](#10_7_)  "string_a" , "string_b" , 'X' , CR , LF , 0


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_8_></a>

### 10.8. DEFGROUP '{' name {',' name \['=' \<8-bit expression\>\]} '}'

 [DEFGROUP](#10_8_)  defines a group of identifier symbols with implicit values. This is similar to the enumeration principles used in C and PASCAL. The initial symbol value is 0, increased by 1 for each new symbol in the list. You may include a \<name = expression\> which breaks the linear enumeration from that constant. The  [DEFGROUP](#10_8_)  directive may be spanned across several lines and MUST be enclosed with { and }.  [DEFGROUP](#10_8_)  is just a more easy way than:  [DEFC](#10_6_)  name0 = 0, name1 = name0, ...

The following example illustrates a useful example of defining symbol values:

     [DEFGROUP](#10_8_)   
    {  
       sym_null  
       sym_ten = 10, sym_eleven, sym_twelve  
    }


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_9_></a>

### 10.9. DEFINE name,{name}

Defines a symbol identifier as logically true (integer \<\> 0). The symbol will be created as a local variable and disappears when assembly is finished on the current source file module.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_10_></a>

### 10.10. DEFS \<size\>{, \<fill-byte\>}

 [DEFS](#10_10_)  allocates a storage area of the given size with the given fill-byte. The fill-byte defaults to zero if not supplied. Both expressions need to be constants.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_11_></a>

### 10.11. DEFVARS \<16-bit expression\> '{' \[\<name\>\] \[\<storage\_size\> \<size\_multiplier\>\] '}'

 [DEFVARS](#10_11_)  defines variable address area or offsets. First you define the origin of a variable area. This may be defined using an evaluable 16-bit positive expression. Each variable name is followed by a size specifier which can be  'ds.b' (byte), 'ds.w' (word), 'ds.p' (3-byte pointer) or 'ds.l' (double-word). This is particularly useful for defining dynamic data structures in linked lists and binary search trees. Defining variable areas are only template definitions not allocations. An example:

     [DEFVARS](#10_11_)   [Z80](#7_) asm_vars  
    {  
       RuntimeFlags1 ds.b 1     ; reserve 1 byte  
       RuntimeFlags2 ds.b 1  
       RuntimeFlags3 ds.b 1  
                     ds.w 1     ; space not named  
       explicitORIG  ds.w 1     ; reserve 2 bytes  
       asmtime       ds.b 3     ; reserve 3 bytes  
       datestamp_src ds.b 6     ; reserve 6 bytes  
       datestamp_obj ds.b 6  
       TOTALERRORS   ds.l 1     ; reserve 4 bytes  
    }

the following is useful for defining dynamic data structures:

     [DEFVARS](#10_11_)  0                    ; 'PfixStack' structure  
    {  
       pfixstack_const     ds.l 1    ; stack item value  
       pfixstack_previtem  ds.p 1    ; pointer to previous  
       SIZEOF_pfixstack              ; size of structure  
    }

This type of variable declaration makes it very easy for modifications, e.g. deleting or inserting variable definitions.

A special logic is available too which can be used throughout individual source files during compilation. If you specify -1 as the starting address, the last offset from the previous  [DEFVARS](#10_11_)  which was not specified as 0 will be used.

This enables you to gradually build up a list of identifier name offsets across  [DEFVARS](#10_11_)  areas in different source files. The following example explains everything:

    defvars $4000  
    {  
       aaa ds.b 1  
       bbb ds.b 100  
    }  
    defvars -1  
    {  
       ccc ds.b 100  
       ddd ds.b 1  
       eee ds.b 10  
    }  
    defvars 0  
    {  
       fff ds.p 1  
       ggg ds.b 1  
       hhh ds.w 1  
       iii ds.p 1  
    }  
    defvars -1  
    {  
       jjj ds.b 100

    }

Some of the symbols will look like this:

    BBB = $4001  
    CCC = $4065  
    GGG = $0003  
    JJJ = $40D4


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_12_></a>

### 10.12. EXTERN name {, name}

This declares symbols as external to the current module. Such a symbol must have been defined as  [PUBLIC](#10_26_)  in another module for the current module to be able to use the symbol (it will be linked during the linking phase).



----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_13_></a>

### 10.13. FPP \<8-bit expression\>

Interface call to the  [Z88](#5_6_)  operating systems' floating point library. This is easier than writing:

    RST $18  
     [DEFB](#10_3_)  mnemonic

This is an advanced RST 18H instruction which automatically allocates space for the specified parameter. All  [Z88](#5_6_)  floating point call mnemonics are defined in the "fpp.def" file.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_14_></a>

### 10.14. GLOBAL name {, name}

The  [GLOBAL](#10_14_)  directive defines a symbol  [PUBLIC](#10_26_)  if it has been defined locally or  [EXTERN](#10_12_)  otherwise.



----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_15_></a>

### 10.15. IF \<logical expression\> ... \[ELSE\] ... ENDIF

This structure evaluates the logical expression as a constant, and compiles the lines up to the ELSE clause if the expression is true (i.e. not zero), or the lines from ELSE to ENDIF if is is false (i.e. zero). The ELSE clause is optional. This structure may be nested.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_16_></a>

### 10.16. IFDEF \<name\> ... \[ELSE\] ... ENDIF

This structure checks if the give symbol name is defined, and compiles the lines up to the ELSE clause if true (i.e. defined), or the lines from ELSE to ENDIF if false (i.e. not defined). The ELSE clause is optional. This structure may be nested.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_17_></a>

### 10.17. IFNDEF \<name\> ... \[ELSE\] ... ENDIF

This structure checks if the give symbol name is not defined, and compiles the lines up to the ELSE clause if true (i.e. not defined), or the lines from ELSE to ENDIF if false (i.e. defined). The ELSE clause is optional. This structure may be nested.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_18_></a>

### 10.18. INCLUDE "filename"

Another component that is frequently used is to 'link' an additional source file together with the current source file. Usually this contains variable definitions that are commonly used by several modules in a project. This makes sense since there is no idea in copying the same information into several files - it simply uses redundant space of your storage media. This is certainly important on the  [Z88](#5_6_)  which not always has huge amounts of installed user/system RAM (usually 128K). The external source file will be included at the position of the  [INCLUDE](#10_18_)  directive.

The format of the filename depends on the operating system platform. As with the current source file, you may also insert files in include files. There is no limit of how many levels (of files) you specify of include files. Recursive or mutually recursive  [INCLUDE](#10_18_)  files (an  [INCLUDE](#10_18_)  file calling itself) is not possible - the assembler program will immediately return an error message back to you!

Include files are usually put at the start of the source file module but may be placed anywhere in the source text. The current source file will be continued after the  [INCLUDE](#10_18_)  directive when the included file has been parsed to the end of file.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_19_></a>

### 10.19. INVOKE \<16-bit expression\>

Special  [CALL](#10_2_)  instruction for the Ti83 calculator; it is coded as a RST 28H followed by the 16-bit expression, if the  [-plus](#5_3_22_)  option is passed on the command line (for the Ti83Plus), or as a normal  [CALL](#10_2_)  instruction if the option is not passed.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_20_></a>

### 10.20. LIB name {,name}

This directive is obsolete. It has been replaced by the  [EXTERN](#10_12_)  directive (See changelog.txt at the root of the z88dk project).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_21_></a>

### 10.21. LINE \<32-bit expr\> \[ , "file-name" \]

Used when the assembler is used as the back-end of a compiler to synchronize the line numbers in error messages to the lines from the compiled source. 


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_22_></a>

### 10.22. LSTOFF

Switches listing output to file off temporarily. The listing file is not closed.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_23_></a>

### 10.23. LSTON

Enables listing output (usually from a previous  [LSTOFF](#10_22_) ). Both directives may be useful when information from  [INCLUDE](#10_18_)  files is redundant in the listing file, e.g. operating system definitions.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_24_></a>

### 10.24. MODULE name

This defines the name of the current module. This may be defined only once for a module. All source file modules contain a module name. This name is used by the assembler when creating address map files and for searching routines in libraries. Further, it allows the programmer to choose a well-defined name for the source file module. The position of the module name is of no importance; it may be placed at the end or the start of the source file. However, it has more sense to put it at the top. The syntax is simple - specify a legal identifier name after the  [MODULE](#10_24_)  directive, e.g.  [MODULE](#10_24_)  main\_module


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_25_></a>

### 10.25. ORG \<16-bit expression\>

Define address origin of compiled machine code - the position in memory where the machine is to be loaded and executed. The expression must be evaluable (containing no forward or external references). All address references will be calculated from the defined  [ORG](#10_25_)  value. The  [ORG](#10_25_)  address will be placed in the current module that is being compiled. However, during linking only the first object module is being read for an  [ORG](#10_25_)  address. The  [ORG](#10_25_)  is ignored during linking if you have specified an  [-r](#5_3_11_)  option on the command line.

When assembling programs with multiple sections, a section without an  [ORG](#10_25_)  will be appended to the end of the previous section. A section with a defined  [ORG](#10_25_)  will generate its own binary file, e.g. file\_CODE.asm.

A section may contain  [ORG](#10_25_)  -1 to tell the linker to split the binary file of this section, but continue the addresses sequence from the previous section.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_26_></a>

### 10.26. PUBLIC name {, name}

This directive declares symbols publicly available for other modules during the linking phase of the compilation process.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_27_></a>

### 10.27. XDEF name {, name}

This directive is obsolete. It has been replaced by the  [PUBLIC](#10_26_)  directive (See changelog.txt at the root of the z88dk project).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_28_></a>

### 10.28. XLIB name

This directive is obsolete. It has been replaced by the  [PUBLIC](#10_26_)  directive (See changelog.txt at the root of the z88dk project).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_29_></a>

### 10.29. XREF name {, name}

This directive is obsolete. It has been replaced by the  [EXTERN](#10_12_)  directive (See changelog.txt at the root of the z88dk project).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_></a>

## 11. Run time error messages

The following error messages will be written toe the error files corresponding to each source file, and also to stderr. Each error message will contain the name of the source file and a line number where the error occurred in the file.

*   "File open/read error"  
    You have tried to access a file that either was not found, already opened by other resources, or the assembler wasn't able to create output files (object file, listing-, symbol-, map- or executable binary file).  
    
*   "Syntax error"  
    This is a message used by many routines - hence the general but descriptive message. You have tried to use illegal registers in  [Z80](#7_)  mnemonic instructions, specified an illegal parameter type (string instead of integer), omitted a parameter ( [DEFB](#10_3_)  without constant).  
    
*   "Symbol not defined"  
    This error is given if you are referring to an identifier (usually in an address reference) that was not declared. Either a label definition is missing in the current module or you have forgotten to declare an identifier as external using the  [XREF](#10_29_)  directive.  
    
*   "Not enough memory" / "No room in  [Z88](#5_6_) "  
    Well, well. It seems that there wasn't enough room to hold the internal data structures during parsing or linking your code. Delete any unnecessary running applications/jobs then try again. If you got the message on the  [Z88](#5_6_) , try also to delete unnecessary files from the filing system of your current RAM card.  
    
*   "Integer out of range"  
    You have an expression which evaluates into a constant that are beyond the legal integer range (e.g. trying to store a 16-bit value into an 8-bit parameter).  
    
*   "Syntax error in expression"  
    Quite clearly you have made an illegal expression, e.g. specifying two following operands without an operator to separate them, used an illegal constant specifier or using illegal characters that aren't a legal identifier.  
    
*   "Right bracket missing"  
    Your expression is using brackets and is not properly balanced, i.e. too many beginning brackets or too few ending brackets.  
    
*   "Source filename missing"  
    There has not been specified any source file modules or project file to start a compilation.  
    
*   "Illegal option"  
    The command line parser couldn't recognise the -option. Remember to specify your option in EXACT case size. You have probably used a space between an option and a filename parameter.  
    
*   "Unknown identifier"  
    The parser expected a legal identifier, i.e. a directive or  [Z80](#7_)  mnemonic. You have probably omitted the '.' in front of a label definition, misspelled a name or used a comment without a leading ';'.  
    
*   "Illegal identifier"  
    You have been trying to use a name that is either not known to the parser or an illegal identifier. This might happen if you try to use a register that is not allowed in a LD instruction, e.g. LD A,F .  
    
*   "Max. code size reached"  
    Is that really possible? Very interesting code of yours!  [Z80](#7_)  machine code program tend to be in the 32K range (at least on the  [Z88](#5_6_) )... Well, the  [Z80](#7_)  processor cannot address more than 64K. Start changing your code to a smaller size!  
    
*   "errors occurred during assembly"  
    Status error message displayed on the screen when the assembler has completed its parsing on all modules. You have one or more errors to correct in your source files before the assembler continues with linking the next time.  
    
*   "Symbol already defined"  
    In the current source file, you have tried to create two identical address label definitions, or other name identifier creators (using  [DEFC](#10_6_) ,  [DEFVARS](#10_11_) ,  [DEFGROUP](#10_8_) ).  
    
*   "Module name already defined"  
    You have used the  [MODULE](#10_24_)  directive more than once in your source file, or used both a  [MODULE](#10_24_)  and  [XLIB](#10_28_)  directive (library modules only need an  [XLIB](#10_28_) ).  
    
*   "Symbol already declared local"  
    You have tried to declare a symbol as  [XREF](#10_29_) , but it was already defined local, eg. using a ".lbaddr" in your source.  
    
*   "Illegal source filename"  
    You have tried to specify an option after having begun to specify filenames. Options must always be specified before filenames or project files.  
    
*   "Symbol declared global in another module"  
    You have two identical  [XDEF](#10_27_)  definitions in separate modules. One of them should probably be an  [XREF](#10_29_)  instead.  
    
*   "Re-declaration not allowed"  
    You are trying to specify an  [XDEF](#10_27_)  for a name that has already been  [XREF](#10_29_) 'ed in the same module (or the other way around).  
    
*   " [ORG](#10_25_)  already defined"  
    Only one  [ORG](#10_25_)  statement is allowed per section.  
    
*   "Relative jump address must be local"  
    You have tried to JR to a label address that is declared as external (with  [XREF](#10_29_)  or  [LIB](#10_20_) ). JR must be performed in the current source file module.  
    
*   "Not a relocatable file" / "Not an object file"  
    The assembler opened a supposed object file (with the proper ".obj" extension) but realised it wasn't conforming to the  [Z80](#7_)  assembler standard object file format.  
    
*   "Couldn't open library file"  
    The library file was found but couldn't be opened (probably already opened by another application resource)  
    
*   "Not a library file"  
    Your library file is not a library file (at least is not of the correct file format used by this assembler). Have you maybe used another "library" file? The  [Z80](#7_)  library file could also be corrupted (at least in the header).  
    
*   "Environment variable not defined"  
    The assembler reports either the " [Z80](#7_) \_STDLIB" environment variable wasn't found when used by options  [-x](#5_3_19_)  and  [-i](#5_3_16_) .  
    
*   "Cannot include file recursively"  
    A file was tried to be included which was already included at a previous include level.  [INCLUDE](#10_18_)  "a.h" cannot contain an  [INCLUDE](#10_18_)  "a.h".


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=12_></a>

## 12. Backlog

- Doing
    - Implement -E to generate pre-processed output in a .i file

- Preprocessor
    - Parse TASM input
    - Parse Z80MR input
    - high level structures
    - macros
    - architecture-specific macro-like opcodes
    - architecture-specific character encoding
    - JR to JP optimization for speed
    - preprocessor
    - separate scanner from preprocessor  

- Assembler
    - JP to JR optimization for size
    - compile expressions
    - object files store list of input files and command line so that  [-d](#5_3_6_)  can decide if it is necessary to recompile
    - object files can store same code for multiple architectures
    - recursive parsing of @lists
    - environment variable to pass options to assembler
    - parse expressions  

- Linker
    - section groups
    - overlays
    - absolute memory areas with holes

- Librarian
    - object files and library files have an index of all defined symbols

- Unsorted Backlog
    - compute compile-time constants  
    - execute compile-time  [IF](#10_15_) /ELSE/ENDIF  
    - do not process preprocessor directives in the false branch of  [IF](#10_15_)   
    fails building z88dk because  [INCLUDE](#10_18_)  of unexistent file guarded by
     [IF](#10_15_)  FALSE is processed and assembly fails  
    - process C_LINE  
    - fix list files when parsing a .i
    - move all directives from z80asm to z80asm2  
    - handle  [-m](#5_3_10_)  for architecture specific code  
    - handle  [-D](#5_3_14_)  and -U for top-level defines
    - process  [INCLUDE](#10_18_)  / INCBIN /  [BINARY](#10_1_)   
    - generate .i file  
    - indicate syntax error location  
    - process labels  
    - process  [INCLUDE](#10_18_)   
    - process  [BINARY](#10_1_) /INCBIN  
    - bug rst label
    - Implement recursive includes in Coco/R parser/scanner
    - Separate standard  [Z80](#7_)  assembly from extensions; add macro files for extensions.
    - Port to C++
    - Parse command line
    - Input source
    - Preprocess
    - Lexing
    - Parsing
    - Symbol Table
    - Object Files
    - Assembling 
    - Linking
    - List file
    - allow EQU as synonym to  [DEFC](#10_6_) 
    - finish the split between front-end and back-end;
    - implement an expression parser with a parser generator, to get rid of
    the need to write a '#' to tell the assembler something it should know:
    a difference between two addresses is a constant;
    - add an additional step to automatically change JR into JP if the
    distance is too far;
    - implement macros inside the assembler
    - add high level constructs ( [IF](#10_15_)  flag / ELSE / ENDIF, 
    DO WHILE flag, ...)
    - add a rule based optimizer based on RAGEL as a state machine generator
    - --icase option
    - --c-strings or --asm-strings
    - architectire-dependent character mapping
    - Test: jr: jr jr
    - Need option to debug pre-processing and macro expansion - -E
    - Add one opcode
    - Separate standard  [Z80](#7_)  assembly from extensions; add macro files for extensions
    - Preprocess
    - Parsing
    - Symbol Table
    - Object Files
    - Assembling
    - Linking
    - List file
    - allow EQU as synonym to  [DEFC](#10_6_) 
    - finish the split between front-end and back-end;
    - implement an expression parser with a parser generator, to get rid of the need to write a '#' to tell the assembler something it should know: a difference between two addresses is a constant;
    - add an additional step to automatically change JR into JP if the distance is too far;
    - cleanup the symbol table to implement the  [GLOBAL](#10_14_)  suggestion: declare a symbol  [GLOBAL](#10_14_)  and it is  [PUBLIC](#10_26_)  if defined, or  [EXTERN](#10_12_)  otherwise
    - implement macros inside the assembler
    - add high level constructs ( [IF](#10_15_)  flag / ELSE / ENDIF, DO WHILE flag, ...)
    - add a rule based optimizer based on RAGEL as a state machine generator
    - BUG_0038: library modules not loaded in sequence: The library modules loaded to the linked binary file should respect the order given on the command line.
    - generate .err files with errors
    - show source line and syntax error location 
    - section name: introduces a new section at the end of the current list
    - section name before name: introduces a new section before the given section
    - section name after name: introduces a new section after the given name
    - org:
    1. Any section with an  [ORG](#10_25_)  address starts a new area of code and generates 
        a new binary output file, named <first_module_name>_<section_name>.bin
    2. Any section that has no  [ORG](#10_25_)  address gets packed at the end of the previous section.
    - The null section is only special in the sense that the code is output 
    to <first_module_name>.bin, and that it's  [ORG](#10_25_)  address can be defined in 
    the command line via -rhhhh.
    - simplify expressions to tranform LABEL1-LABEL2 into a constant
    - --icase option
    - --c-strings or --asm-strings
    - architecture-dependent character mapping
    - Test: jr: jr jr
    - Need option to debug pre-processing and macro expansion - -E
    - Add one opcode
    - Separate standard  [Z80](#7_)  assembly from extensions; add macro files for extensions
    - Preprocess
    - Parsing
    - Symbol Table
    - Object Files
    - Assembling
    - Linking
    - List file
    - allow EQU as synonym to  [DEFC](#10_6_) 
    - finish the split between front-end and back-end;
    - implement an expression parser with a parser generator, to get rid of the need to write a '#' to tell the assembler something it should know: a difference between two addresses is a constant;
    - add an additional step to automatically change JR into JP if the distance is too far;
    - cleanup the symbol table to implement the  [GLOBAL](#10_14_)  suggestion: declare a symbol  [GLOBAL](#10_14_)  and it is  [PUBLIC](#10_26_)  if defined, or  [EXTERN](#10_12_)  otherwise
    - implement macros inside the assembler
    - add high level constructs ( [IF](#10_15_)  flag / ELSE / ENDIF, DO WHILE flag, ...)
    - add a rule based optimizer based on RAGEL as a state machine generator
    - BUG_0038: library modules not loaded in sequence: The library modules loaded to the linked binary file should respect the order given on the command line.
    - generate .err files with errors
    - show source line and syntax error location 
    - Manage libraries
    - Link object files and libraries
    - Split object files in modules, link each module separately
    - :: to define public symbol
    - ## to declare external symbol
    - #include
    - #define
    - #macro



----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=13_></a>

## 13. References

- [The Telemark Assembler](http://www.cpcalive.com/docs/TASMMAN.HTM)


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=14_></a>

## 14. Copyright

The original z80asm module assembler was written by Gunther Strube. 
It was converted from QL SuperBASIC version 0.956, initially ported to Lattice C,
and then to C68 on QDOS.

It has been maintained since 2011 by Paulo Custodio.

Copyright (C) Gunther Strube, InterLogic 1993-99  
Copyright (C) Paulo Custodio, 2011-2020


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=15_></a>

## 15. License

Artistic License 2.0 [http://www.perlfoundation.org/artisticlicense2_0](http://www.perlfoundation.org/artisticlicense2_0 "Artistic License 2.0")

<a id=keywords></a>


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=16_></a>

## 16. Keywords
 [-C](#5_3_23_) 
 [-D](#5_3_14_) 
 [-I](#5_3_18_) 
 [-IXIY](#2_2_6_) 
 [-L](#5_3_17_) 
 [-M](#5_3_3_) 
 [-R](#5_3_12_) 
 [-RCMX000](#5_3_21_) 
 [-a](#5_3_8_) 
 [-atoctal](#2_2_1_) 
 [-b](#5_3_7_) 
 [-d](#5_3_6_) 
 [-dotdirective](#2_2_2_) 
 [-e](#5_3_2_) 
 [-g](#5_3_13_) 
 [-h](#5_3_1_) 
 [-hashhex](#2_2_3_) 
 [-i](#5_3_16_) 
 [-l](#5_3_4_) 
 [-labelcol1](#2_2_4_) 
 [-m](#5_3_10_) 
 [-noprec](#2_3_1_) 
 [-o](#5_3_9_) 
 [-plus](#5_3_22_) 
 [-r](#5_3_11_) 
 [-s](#5_3_5_) 
 [-t](#5_3_20_) 
 [-ucase](#2_2_5_) 
 [-v](#2_1_1_) 
 [-x](#5_3_19_) 
 [BINARY](#10_1_) 
 [CALL](#10_2_) 
 [DEFB](#10_3_) 
 [DEFC](#10_6_) 
 [DEFGROUP](#10_8_) 
 [DEFINE](#10_9_) 
 [DEFL](#10_5_) 
 [DEFM](#10_7_) 
 [DEFS](#10_10_) 
 [DEFVARS](#10_11_) 
 [DEFW](#10_4_) 
 [EXTERN](#10_12_) 
 [FPP](#10_13_) 
 [GLOBAL](#10_14_) 
 [IF](#10_15_) 
 [IFDEF](#10_16_) 
 [IFNDEF](#10_17_) 
 [INCLUDE](#10_18_) 
 [INVOKE](#10_19_) 
 [LIB](#10_20_) 
 [LINE](#10_21_) 
 [LSTOFF](#10_22_) 
 [LSTON](#10_23_) 
 [MODULE](#10_24_) 
 [ORG](#10_25_) 
 [PUBLIC](#10_26_) 
 [XDEF](#10_27_) 
 [XLIB](#10_28_) 
 [XREF](#10_29_) 
 [Z80](#7_) 
 [Z88](#5_6_) 
<a id=index></a>


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=17_></a>

## 17. Index
- [1.](#1_) Usage ...
  - [1.1.](#1_1_) ... as pre-processor
  - [1.2.](#1_2_) ... as assembler
  - [1.3.](#1_3_) ... as linker
  - [1.4.](#1_4_) ... as librarian
- [2.](#2_) Options
  - [2.1.](#2_1_) General Options
    - [2.1.1.](#2_1_1_)  [-v](#2_1_1_)  (verbose)
  - [2.2.](#2_2_) Preprocessor options
    - [2.2.1.](#2_2_1_)  [-atoctal](#2_2_1_)  (at is octal prefix)
    - [2.2.2.](#2_2_2_)  [-dotdirective](#2_2_2_)  (period is directive prefix)
    - [2.2.3.](#2_2_3_)  [-hashhex](#2_2_3_)  (hash is hex prefix)
    - [2.2.4.](#2_2_4_)  [-labelcol1](#2_2_4_)  (labels at column 1)
    - [2.2.5.](#2_2_5_)  [-ucase](#2_2_5_)  (upper case)
    - [2.2.6.](#2_2_6_)  [-IXIY](#2_2_6_)  (swap IX and IY)
  - [2.3.](#2_3_) Assembly options
    - [2.3.1.](#2_3_1_)  [-noprec](#2_3_1_)  (no precedence in expression evaluation)
- [3.](#3_) Input Files
  - [3.1.](#3_1_) Source File Format
  - [3.2.](#3_2_) Comments
  - [3.3.](#3_3_) Symbols
  - [3.4.](#3_4_) Labels
  - [3.5.](#3_5_) Numbers
    - [3.5.1.](#3_5_1_) Decimal
    - [3.5.2.](#3_5_2_) Hexadecimal
    - [3.5.3.](#3_5_3_) Octal
    - [3.5.4.](#3_5_4_) Binary
    - [3.5.5.](#3_5_5_) Bitmaps
  - [3.6.](#3_6_) Keywords
  - [3.7.](#3_7_) Directives and Opcodes
- [4.](#4_) Object File Format
  - [4.1.](#4_1_) Object Files
  - [4.2.](#4_2_) Library File Format
  - [4.3.](#4_3_) Format History
- [5.](#5_) z80asm Syntax
  - [5.1.](#5_1_) Help page
  - [5.2.](#5_2_) Command line
  - [5.3.](#5_3_) Command line options
    - [5.3.1.](#5_3_1_)  [-h](#5_3_1_)  : Show help screen
    - [5.3.2.](#5_3_2_)  [-e](#5_3_2_) \<ext\> : Use alternative source file extension
    - [5.3.3.](#5_3_3_)  [-M](#5_3_3_) \<ext\> : Use alternative object file extension
    - [5.3.4.](#5_3_4_)  [-l](#5_3_4_)  : Create listing file output
    - [5.3.5.](#5_3_5_)  [-s](#5_3_5_)  : Create symbol table
    - [5.3.6.](#5_3_6_)  [-d](#5_3_6_)  : Assemble only updated files
    - [5.3.7.](#5_3_7_)  [-b](#5_3_7_)  : Link/relocate object files
    - [5.3.8.](#5_3_8_)  [-a](#5_3_8_)  : Combine  [-d](#5_3_6_)  and  [-b](#5_3_7_) 
    - [5.3.9.](#5_3_9_)  [-o](#5_3_9_) \<binary-filename\> : Binary filename
    - [5.3.10.](#5_3_10_)  [-m](#5_3_10_)  : Create address map information file
    - [5.3.11.](#5_3_11_)  [-r](#5_3_11_) \<hex-address\> : Re-define the  [ORG](#10_25_)  relocation address
    - [5.3.12.](#5_3_12_)  [-R](#5_3_12_)  : Generate address independent code
    - [5.3.13.](#5_3_13_)  [-g](#5_3_13_)  : Create global address definition file
    - [5.3.14.](#5_3_14_)  [-D](#5_3_14_) \<symbol\> : Define a static symbol
    - [5.3.15.](#5_3_15_) @\<project-file\> :1.3.16. Using a project file
    - [5.3.16.](#5_3_16_)  [-i](#5_3_16_) \<library-file\> : Include library modules during linking/relocation
    - [5.3.17.](#5_3_17_)  [-L](#5_3_17_) \<library-path\> : Add directory to search path for libraries
    - [5.3.18.](#5_3_18_)  [-I](#5_3_18_) \<include-path\> : Add directory to search path for  [INCLUDE](#10_18_) 
    - [5.3.19.](#5_3_19_)  [-x](#5_3_19_)  : Create a library
    - [5.3.20.](#5_3_20_)  [-t](#5_3_20_) \<number\> : Define tabulator distance for text output files
    - [5.3.21.](#5_3_21_)  [-RCMX000](#5_3_21_)  : Support the RCM2000/RCM3000 series of  [Z80](#7_) -like CPU's
    - [5.3.22.](#5_3_22_)  [-plus](#5_3_22_)  : Support for the Ti83Plus
    - [5.3.23.](#5_3_23_)  [-C](#5_3_23_)  : Enable  [LINE](#10_21_)  directive
  - [5.4.](#5_4_) The  [Z88](#5_6_)  operating system definition files
  - [5.5.](#5_5_) The supplied standard library  [Z80](#7_)  source files
  - [5.6.](#5_6_)  [Z88](#5_6_)  module assembler application source
  - [5.7.](#5_7_) File based compilation
  - [5.8.](#5_8_) Modular source file design
  - [5.9.](#5_9_) Scope of symbols in source modules
  - [5.10.](#5_10_) Using arithmetic and relational expressions
  - [5.11.](#5_11_) Source file manipulation
  - [5.12.](#5_12_) Free format of assembler source files
  - [5.13.](#5_13_) Specification of filenames
  - [5.14.](#5_14_) Including other source files into the current source file
  - [5.15.](#5_15_) Conditional assembly in source file modules
  - [5.16.](#5_16_) Undocumented  [Z80](#7_)  instruction code generation
  - [5.17.](#5_17_) Object file generation
  - [5.18.](#5_18_) Transfer of object files across platforms
  - [5.19.](#5_19_) Date stamp controlled assembly
  - [5.20.](#5_20_) Listing files
  - [5.21.](#5_21_) Symbol information
  - [5.22.](#5_22_) Linking and relocation of object modules into executable  [Z80](#7_)  machine code
  - [5.23.](#5_23_) Address map files
  - [5.24.](#5_24_) Symbol address definition files
  - [5.25.](#5_25_) Error files
  - [5.26.](#5_26_) Creating and using object file libraries for standard routines
- [6.](#6_) Executing the cross assembler and environment variables
  - [6.1.](#6_1_) The environment variables
  - [6.2.](#6_2_) Running in the QDOS/SMSQ operating system environment
  - [6.3.](#6_3_) Running in the LINUX/UNIX operating system environment
  - [6.4.](#6_4_) Running in the MS-DOS operating system environment
- [7.](#7_)  [Z80](#7_)  module assembler file types
  - [7.1.](#7_1_) The assembler file types and their extension names
  - [7.2.](#7_2_) The file name extension identifier
  - [7.3.](#7_3_) File types
    - [7.3.1.](#7_3_1_) The source file extension, asm
    - [7.3.2.](#7_3_2_) The object file extension, obj
    - [7.3.3.](#7_3_3_) The error file extension, err
    - [7.3.4.](#7_3_4_) The listing file extension, lst
    - [7.3.5.](#7_3_5_) The symbol file extension, sym
    - [7.3.6.](#7_3_6_) The executable file extension, bin
    - [7.3.7.](#7_3_7_) The address map file extension, map
    - [7.3.8.](#7_3_8_) The definition file extension, def
    - [7.3.9.](#7_3_9_) The library file extension, lib
- [8.](#8_) Compiling files
  - [8.1.](#8_1_) The assembler compiling process
    - [8.1.1.](#8_1_1_) Stage 1, parsing and code generation of all source files, object file generation
    - [8.1.2.](#8_1_2_) Stage 2, linking object files and library modules, producing executable code
  - [8.2.](#8_2_) File names
  - [8.3.](#8_3_) Portability of assembler file names
  - [8.4.](#8_4_) Source file structure
  - [8.5.](#8_5_) Using local, global and external symbols
  - [8.6.](#8_6_) Defining symbol names
  - [8.7.](#8_7_) Comments in source files
  - [8.8.](#8_8_) Defining symbolic address labels
  - [8.9.](#8_9_) Writing  [Z80](#7_)  mnemonic instructions
  - [8.10.](#8_10_) Optional  [Z80](#7_)  mnemonic instruction syntax
  - [8.11.](#8_11_) The undocumented  [Z80](#7_)  instructions
  - [8.12.](#8_12_) Referencing library routines
  - [8.13.](#8_13_) Creating/updating libraries
  - [8.14.](#8_14_) Referencing routines in other compiled projects
- [9.](#9_) Using expressions
  - [9.1.](#9_1_) Constant identifiers
  - [9.2.](#9_2_) Arithmetic operators
  - [9.3.](#9_3_) Relational operators
  - [9.4.](#9_4_) The ASMPC standard function
  - [9.5.](#9_5_) Symbol identifiers in expressions
- [10.](#10_) Directive reference
  - [10.1.](#10_1_)  [BINARY](#10_1_)  "filename"
  - [10.2.](#10_2_)  [CALL](#10_2_) \_OZ \<expression\>
  - [10.3.](#10_3_)  [DEFB](#10_3_)  \<8-bit expr\>,{\<8-bit expr\>} (-128; 255)
  - [10.4.](#10_4_)  [DEFW](#10_4_)  \<16-bit expr\>,{\<16-bit expr\>} (-32768; 65535)
  - [10.5.](#10_5_)  [DEFL](#10_5_)  \<32-bit expr\>,{\<32-bit expr\>} (-2147483647; 4294967295)
  - [10.6.](#10_6_)  [DEFC](#10_6_)  name=\<32-bit expression\>{, name=\<32-bit expression\>}
  - [10.7.](#10_7_)  [DEFM](#10_7_)  \<string expression\>|\<8-bit expr\>,...
  - [10.8.](#10_8_)  [DEFGROUP](#10_8_)  '{' name {',' name \['=' \<8-bit expression\>\]} '}'
  - [10.9.](#10_9_)  [DEFINE](#10_9_)  name,{name}
  - [10.10.](#10_10_)  [DEFS](#10_10_)  \<size\>{, \<fill-byte\>}
  - [10.11.](#10_11_)  [DEFVARS](#10_11_)  \<16-bit expression\> '{' \[\<name\>\] \[\<storage\_size\> \<size\_multiplier\>\] '}'
  - [10.12.](#10_12_)  [EXTERN](#10_12_)  name {, name}
  - [10.13.](#10_13_)  [FPP](#10_13_)  \<8-bit expression\>
  - [10.14.](#10_14_)  [GLOBAL](#10_14_)  name {, name}
  - [10.15.](#10_15_)  [IF](#10_15_)  \<logical expression\> ... \[ELSE\] ... ENDIF
  - [10.16.](#10_16_)  [IFDEF](#10_16_)  \<name\> ... \[ELSE\] ... ENDIF
  - [10.17.](#10_17_)  [IFNDEF](#10_17_)  \<name\> ... \[ELSE\] ... ENDIF
  - [10.18.](#10_18_)  [INCLUDE](#10_18_)  "filename"
  - [10.19.](#10_19_)  [INVOKE](#10_19_)  \<16-bit expression\>
  - [10.20.](#10_20_)  [LIB](#10_20_)  name {,name}
  - [10.21.](#10_21_)  [LINE](#10_21_)  \<32-bit expr\> \[ , "file-name" \]
  - [10.22.](#10_22_)  [LSTOFF](#10_22_) 
  - [10.23.](#10_23_)  [LSTON](#10_23_) 
  - [10.24.](#10_24_)  [MODULE](#10_24_)  name
  - [10.25.](#10_25_)  [ORG](#10_25_)  \<16-bit expression\>
  - [10.26.](#10_26_)  [PUBLIC](#10_26_)  name {, name}
  - [10.27.](#10_27_)  [XDEF](#10_27_)  name {, name}
  - [10.28.](#10_28_)  [XLIB](#10_28_)  name
  - [10.29.](#10_29_)  [XREF](#10_29_)  name {, name}
- [11.](#11_) Run time error messages
- [12.](#12_) Backlog
- [13.](#13_) References
- [14.](#14_) Copyright
- [15.](#15_) License
- [16.](#16_) Keywords
- [17.](#17_) Index
