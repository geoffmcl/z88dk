<a id=top></a>

# z80asm - Z80 module assembler, linker, librarian

**z80asm** is part of the [z88dk](http://www.z88dk.org/) project and is used as the back-end of the z88dk C compilers. It is not to be confused with other non-**z88dk** related projects with the same name.

**z80asm** is a relocatable assembler, linker and librarian that can assemble Intel 8080/8085 and Z80-family assembly files into a relocatable object format, can manage sets of object files in libraries and can build binary images by linking these object files together. The binary images can be defined in different sections, to match the target architecture.

**NOTE**: This document is a work-in-porgress. It describes the functionality already working in the assembler in the **z80asm2** branch. This document and the ```z80asm-future-md will converge as **z80asm2** gets more features and converges to the current capability of **z80asm** in the **master** branch.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=1_></a>

## 1. Usage ...


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_></a>

## 2. Options


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_1_></a>

### 2.1. Preprocessor options


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_1_1_></a>

#### 2.1.1. -atoctal (at is octal prefix)

By default the at-character (```@```) is used as a binary number prefix. 

With the option  [-atoctal](#2_1_1_)  it is used as the octal number prefix instead.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_1_2_></a>

#### 2.1.2. -dotdirective (period is directive prefix)

By default the period (```.```) is used to signal that the next identifier is a label. 

With the option  [-dotdirective](#2_1_2_)  it is used instead to signal that the next identifier is an assembler directive.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_1_3_></a>

#### 2.1.3. -hashhex (hash is hex prefix)

By default the hash-character (```#```) is used to signal that the next expression should be compiled as an immediate value. This meaning, although common in assemblers, is a no-operation in **z80asm**. 

With the option  [-hashhex](#2_1_3_)  the hash-character is used as the hexadecimal number prefix instead.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_1_4_></a>

#### 2.1.4. -labelcol1 (labels at column 1)

By default **z80asm** needs either a period (```.```) prefix (but see  [-dotdirective](#2_1_2_) ) or a colon (```:```) suffix to signal that an identifier is a label, and white space at the beginning of a line is not significant.

With the option  [-labelcol1](#2_1_4_)  an identifier is a label if started at column 1, or a directive or opcode if started after white space.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_1_5_></a>

#### 2.1.5. -ucase (upper case)

By default **z80asm** is case-sensitive for identifiers, but case-insensitive for assembly keywords (opcodes, directives, registers and flags).

The option  [-ucase](#2_1_5_)  causes **z80asm** to convert all the symbols to upper-case, so that code that assumes case-insentivity can be assembled.






----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_></a>

## 3. Input Files


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_1_></a>

### 3.1. Source File Format

The assembler parses source files with any of the common end-of-line termination sequences (```"\r"```, ```"\n"``` or ```"\r\n"```). Each line starts with an optional label and can contain assembly directives (i.e. instructions to the assembler), assembly instructions (i.e. code to be translated into object code for the specific processor) or blanks and comments.

A single backslash character (```\```) may be used to separate multiple statements in a single line.

    org 0                     ; assembly directive  
    start: push bc\pop hl     ; define a label and add two assembly opcodes              
    ret                       ; assembly opcode can be at the first column    

Differently to most other assemblers, white space is not significant, i.e. a label can be defined after white space, and an opcode can be written at column 1 (but see option  [-labelcol1](#2_1_4_) ).


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

All symbols in the code (labels, variables, ...) are named with unique identifiers. Identifiers start with a letter or underscore (```_```), and can contain letters, digits, underscores or periods (```.```). Identifiers are case-sensitive (but see option  [-ucase](#2_1_5_) ).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_4_></a>

### 3.4. Labels

A label is a symbol that represents the current assembly address (ASMPC) and is defined at the start of a line by prefixing a symbol with a period (```.```) (but see  [-dotdirective](#2_1_2_) ) or suffixing it with a colon (```:```), i.e. either ```.label``` or ```label:``` (but see  [-labelcol1](#2_1_4_) ).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_5_></a>

### 3.5. Numbers

The assembler accepts numbers in decimal, hexadecimal, octal and binary. Different syntaxes are allowed to simplify porting code written for other assemblers. Some of the prefix characters are also used as operators; in this case a space may be needed to signal the difference, e.g.

    ld a, %10     ; A = 2 (10 binary)
    ld a, 12 % 10 ; A = 2 (remainder of 12 divided by 10)

All expressions are computed as signed integers with the host platform's integer size (32-bit or 64-bit in the most common platforms). Expression evaluation follows the operator precedence of C (but see -noprec).


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

The hexadecimal prefix ```#``` is only recognized with the option  [-hashhex](#2_1_3_) .

    ld a, $FF
    ld a, #FF          ; only with option  [-hashhex](#2_1_3_) 
    ld a, 0xFF
    ld a, 0FFh


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_5_3_></a>

#### 3.5.3. Octal

Octal numbers are a sequence of octal digits (```0..7```), either prefixed or suffixed with an octal marker. 

The octal-prefix ```@``` is only recognized with the option  [-atoctal](#2_1_1_) .

    ld a, @77          ; only with option  [-atoctal](#2_1_1_) 
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

The binary prefix ```@``` is recognized unless the option  [-atoctal](#2_1_1_)  is given.

    ld a, %11
    ld a, @11           ; except with option  [-atoctal](#2_1_1_) 
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

Assembler directives (ORG, INCLUDE, ...) and processor opcodes (NOP, LD, ...) are interpreted as directives or opcodes when appearing at the start of the statement or after a label definition, or as regular identifiers otherwise. The directives and opcodes are case-insensitive.

    jr: jr jr  ; silly example, jr is both a label and an opcode
               ; while correct code, it's confusing, don't do it


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=4_></a>

## 4. Object File Format


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=4_1_></a>

### 4.1. Object Files


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=4_2_></a>

### 4.2. Library File Format


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_></a>

## 5. Copyright

The original z80asm module assembler was written by Gunther Strube. 
It was converted from QL SuperBASIC version 0.956, initially ported to Lattice C,
and then to C68 on QDOS.

It has been maintained since 2011 by Paulo Custodio.

Copyright (C) Gunther Strube, InterLogic 1993-99  
Copyright (C) Paulo Custodio, 2011-2020


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_></a>

## 6. License

Artistic License 2.0 [http://www.perlfoundation.org/artisticlicense2_0](http://www.perlfoundation.org/artisticlicense2_0 "Artistic License 2.0")

<a id=keywords></a>


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_></a>

## 7. Keywords
 [-atoctal](#2_1_1_) 
 [-dotdirective](#2_1_2_) 
 [-hashhex](#2_1_3_) 
 [-labelcol1](#2_1_4_) 
 [-ucase](#2_1_5_) 
<a id=index></a>


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_></a>

## 8. Index
- [1.](#1_) Usage ...
- [2.](#2_) Options
  - [2.1.](#2_1_) Preprocessor options
    - [2.1.1.](#2_1_1_)  [-atoctal](#2_1_1_)  (at is octal prefix)
    - [2.1.2.](#2_1_2_)  [-dotdirective](#2_1_2_)  (period is directive prefix)
    - [2.1.3.](#2_1_3_)  [-hashhex](#2_1_3_)  (hash is hex prefix)
    - [2.1.4.](#2_1_4_)  [-labelcol1](#2_1_4_)  (labels at column 1)
    - [2.1.5.](#2_1_5_)  [-ucase](#2_1_5_)  (upper case)
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
- [5.](#5_) Copyright
- [6.](#6_) License
- [7.](#7_) Keywords
- [8.](#8_) Index
