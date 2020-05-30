<a id=top></a>

# z80asm - Z80 module assembler, linker, librarian

**z80asm** is part of the [z88dk](http://www.z88dk.org/) project and is used as the back-end of the z88dk C compilers. It is not to be confused with other non-**z88dk** related projects with the same name.

**z80asm** is a relocatable assembler, linker and librarian that can assemble Intel 8080/8085 and  [Z80](#8_) -family assembly files into a relocatable object format, can manage sets of object files in libraries and can build binary images by linking these object files together. The binary images can be defined in different sections, to match the target architecture.


 

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

When called with the ```-E``` option, **z80asm** pre-processes each input file and stores the result in files with the ```.i``` extension, with all macros expanded, include files expanded, and constants converted to hexadecimal. 


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
z80asm  [-b](#6_3_8_)  [options] file...
```

When called with the ``` [-b](#6_3_8_) ``` option, **z80asm** links the object files together into a set of binary files.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=1_4_></a>

### 1.4. ... as librarian

```
z80asm -xlibrary.lib [options] file...
```

When called with the ``` [-x](#6_3_20_) ``` option, **z80asm** builds a library containing all the object files passed as argument. That library can then be used during linking by specifying it with the ``` [-i](#6_3_17_) ``` option.

---

<a id=options></a> [(top)](#top)


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=2_></a>

## 2. Options

---

<a id=inputfiles></a> [(top)](#top)


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=3_></a>

## 3. Input Files

**z80asm** reads text files in the syntax supported by the specific processor being assembled for (see ``` [-m](#6_3_11_) ``` option) and produces the corresponding object files.

An input file with a ```.o``` extension is assumed to be already in object file format and is just read by the linker. Any other extension is considered an assembly source file (conventionally ```.asm```).

A project list file may be supplied in the command line prefixed by the at-sign (e.g. ```@project.lst```). The list file contains one input file name per line, or another project list prefixed with an at-sign, which is opened recursively. Hash signs (```#```) and semi-colons (```;```) may be used at the start of lines in the list files to include comments.

Both the command line and the list files support wild-cards to expand all the files that match the given pattern.  
*Note* that the Operating System may do its own wildcard expansion and the pattern may need to be quoted in the command line.

A single star in a file name (```*```) expands to the list of all files/directories that match the complete pattern, where the star represents any sequence of characters. A double-star in a directory name (```**```) expands to the complete directory sub-tree, when searched recursively.

---

<a id=sourcefileformat></a>


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=4_></a>

## 4. Source File Format

The assembler parses source files with any of the common end-of-line termination sequences (```"\r"```, ```"\n"``` or ```"\r\n"```). Each line starts with an optional label and can contain assembly directives (i.e. instructions to the assembler), assembly instructions (i.e. code to be translated into object code for the specific processor) or blanks and comments.

A single backslash character (```\```) may be used to separate multiple statements in a single line.

    org 0                     ; assembly directive  
    start: push bc\pop hl     ; define a label and add two assembly opcodes              
    ret                       ; assembly opcode can be at the first column    

Differently to most other assemblers, white space is not significant, i.e. a label can be defined after white space, and an opcode can be written at column 1. 

The option ```-labelcol1``` can be used to force any word starting on column 1 to be a label.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=4_1_></a>

### 4.1. Comments

Comments may start with a semi-colon (```;```) or two slashes (```//```) and end at the end of the line, or may start with slash-star (```/*```) and end with star-slash (```*/```), possibly spanning multiple lines.

    ld a, 1                   ; this is a comment
    ld b, 2                   // another comment
    ld c, /* multi-line comment is valid
             in the middle of an instruction 
          */ 3                ; C = 3


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=4_2_></a>

### 4.2. Symbols

All symbols in the code (labels, variables, ...) are named with unique identifiers. Identifiers start with a letter or underscore (```_```), and can contain letters, digits, underscores or periods (```.```). 

Identifiers are case-sensitive. 

The option ```-ucase``` can be used to convert all symbols to upper-case.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=4_3_></a>

### 4.3. Labels

A label is defined at the start of a line by prefixing a symbol with a period (```.```) or suffixing it with a colon (```:```), i.e. either ```.label``` or ```label:```. The symbol may be used to refer to address in the object code.

If the option ```-labelcol1``` is used, then any identifier starting at column 1 is a label.

If the option ```-dotdirective``` is used, then a period is used to start a directive (e.g. ```. [ORG](#11_25_) ```), and a label must be defined with a colon suffix, or the ```-labelcol1``` option and at column 1.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=4_4_></a>

### 4.4. Numbers

The assembler accepts numbers in decimal, hexadecimal, octal and binary. Different syntaxes are allowed to simplify porting code written for other assemblers. Some of the prefix characters are also used as operators; in this case a space may be needed to signal the difference.

    ld a, %10     ; A = 2 (10 binary)
    ld a, 12 % 10 ; A = 2 (remainder of 12 divided by 10)

All expressions are computed as signed integers with the host platform's integer size (32-bit or 64-bit in the most common platforms). Expression evaluation follows the operator precedence of C, except if the option ```-noprec``` is given, in which case the expressions are computed right to left.

The underscore can be used to separate groups of digits to help readability, e.g. ```0xFFFF_FFFF``` is the same as ```0xFFFFFFFF```.

Floating point numbers can be supplied with the ```FLOAT``` directive, that encodes them in the current floating point format in the object code. Floating point numbers must be supplied in decimal base and have an integer part and a fractional part separated by a period, followed by an optional ```e``` (exponent), a plus or minus sign, and the power of 10 to multiply to the base.

**Decimal** numbers are a sequence of decimal digits (```0..9```), optionally followed by a ```d``` or ```D``` - all prefixes and suffixes are case-insensitive. Leading zeros are insignificant - note the difference from C, where a leading zero means octal.

    ld a, 99
    ld a, 99d


**Hexadecimal** numbers are a sequence of hexadecimal digits (```0..9``` and ```A..F```, case-insensitive), either prefixed or suffixed with an hexadecimal marker. If the marker is a suffix, and the number starts with a letter, then a leading zero has to be added.

The hexadecimal prefix ```$``` is also the ```ASMPC``` identifier if not followed by a hexadecimal number, i.e. the address of the instruction being assembled.

The hexadecimal prefix ```#``` is only recognized if the option ```-hashhex``` is given. By default a ```#``` is used to signal that the following value is immediate.

    ld a, $FF
    ld a, #FF          ; only with option -hashhex
    ld a, 0xFF
    ld a, 0FFh

**Octal** numbers are a sequence of octal digits (```0..7```), either prefixed or suffixed with an octal marker. 

The at-sign (```@```) is an octal prefix only if the option ```-atoctal``` is given. By default a ```@``` is a binary prefix.

    ld a, @77          ; only with option -atoctal
    ld a, 0o77
    ld a, 0q77
    ld a, 77o
    ld a, 77q

**Binary** numbers are a sequence of binary digits (```0..1```), either prefixed or suffixed with a binary marker.  

The percent sign (```%```) is also the modulus operator.

The at-sign (```@```) is a binary prefix by default, or an octal prefix if the option ```-atoctal``` is given.

    ld a, %11
    ld a, @11
    ld a, 0b11
    ld a, 11b

Binary numbers can be specified as **bitmaps**, with ```#``` as ```1``` and ```-``` as ```0```, using the binary prefix (```@``` or ```%```) immediately followed by a double-quoted string of hashes and dashes.

    defb @"---##---"
    defb @"-##--##-"
    defb %"-##-----"
    defb %"-##-----"
    defb @"-##--##-"
    defb @"---##---"



----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=4_5_></a>

### 4.5. Keywords

Processor registers (```BC```, ```DE```, ...) and flags (```NZ```, ```Z```, ...), and assembly ```ASMPC```, representing the current assembly location, are reserved keywords. They cannot be used as identifiers, and are case-insensitive.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=4_6_></a>

### 4.6. Directives and Opcodes

Assembler directives (``` [ORG](#11_25_) ```, ``` [INCLUDE](#11_18_) ```, ...) and processor opcodes (```NOP```, ```LD```, ...) are interpreted as directives or opcodes when appearing at the start of the statement or after a label definition, or as regular identifiers otherwise. The directives and opcodes are case-insensitive.

    jr: jr jr  ; silly example, jr is both a label and an opcode
               ; while correct code, it's confusing, don't do it


---

<a id=objectfileformat></a>


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_></a>

## 5. Object File Format

The object and library files are stored in binary form as a set of contiguous objects, i.e. each section follows the previous one without any end-markers. The files are cross-platform compatible, i.e. they can be created in a big-endian architecture and used in a little-endian one. 

The files start with a signature and a version number, that is parsed by the assembler.

A set of file pointers at the start of the object file point to the start of each section existing the in the file, or contain *0xFFFFFFFF* (-1) if that section does not exist.

The following object types exist in the file:

* *char* :	one byte containing an ASCII character;

* *byte* :	one 8-bit value;

* *word* :	one 16-bit value, stored in low byte - high byte order (little-endian format -  [Z80](#8_) /Intel notation);

* *long* :	one 32-bit value, stored in low byte - high byte order;

* *string*:	one byte containing the string length followed by the characters of the string;

* *lstring*: one word containing the string length followed by the characters of the string.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_1_></a>

### 5.1. Object Files

The format of the object files is as follows:

    |Addr | Type   | Object                                                 |  
    +-----+--------+--------------------------------------------------------+  
    |   0 | char[8]| 'Z80RMF14' (file signature and version)                |  
    |   8 | long   | File pointer to *Module Name*, always the last section |  
    |  12 | long   | File pointer to *Expressions*, may be -1               |  
    |  16 | long   | File pointer to *Defined Symbols*, may be -1           |  
    |  20 | long   | File pointer to *External Symbols*, may be -1          |  
    |  24 | long   | File pointer to *Binary Code Sections*, may be -1      |  
    |  28 |        | *Expressions*                                          |  
    |     |        | ...                                                    |  
    |     |        | *Defined Symbols*                                      |  
    |     |        | ...                                                    |  
    |     |        | *External Symbols*                                     |  
    |     |        | ...                                                    |  
    |     |        | *Module Name*                                          |  
    |     |        | *Binary Code Sections*                                 |  


* *Expressions* : contains a set of expressions up to an end marker (*type* = 0). Each expression has the following format:

  * *type* (char) : defines the resulting evaluation type range:   
     *  0  : end marker 
     * 'U' : 8-bit integer (0 to 255)  
     * 'S' : 8-bit signed integer (-128 to 127)  
     * 'u' : 8-bit integer (0 to 255) extended to 16 bits (appending a 0)  
     * 's' : 8-bit signed integer (-128 to 127) sign-extended to 16 bits (appending a 0xff if negative or 0 otherwise)  
     * 'C' : 16-bit integer, little-endian (-32768 to 65535)  
     * 'B' : 16-bit integer, big-endian (-32768 to 65535)  
     * 'P' : 24-bit signed integer     
     * 'L' : 32-bit signed integer     
     * 'J' : 8-bit jump relative offset
     * '=' : computed name at link time   

  * *sourcefile* (lstring) : source file name where expression was defined, to be used in error messages. May be an empty string, signaling that the previous *sourcefile* of the previous expression should be used.

  * *line_number* (long) : line number in source file where expression was defined, to be used in error messages.

  * *section* (string) : source file section name where expression was defined. 

  * *ASMPC* (word) : defines the relative module code address of the start of the assembly instruction to be used as *ASMPC* during expression evaluation.

  * *patchptr* (word) : defines the relative module code patch pointer to store the result of evaluating the expression.

  * *target_name* (string) : contains the name of the symbol that receives the result of evaluating the expression, only used for '=' type expressions, empty string for the other types.
	
  * *expression* (lstring) : contains the expression text as parsed from the source file, in the standard C-like expression syntax.

* *Defined Symbols* : contains a set of names defined in this module up to the next existing section. Each name has the following format:

  * *scope* (char) : defines the scope of the name:
     *  0  : end marker
     * 'L' is local,  
     * 'G' is global,  

  * *type (char) : defines whether it is a: 
     * 'A' relocatable address,   
     * 'C' a constant.
     * '=' a symbol computed at link time, the corresponding expression is in the *Expressions* section.

  * *section* (string) : source file section name where name was defined. 

  * *value* (long) : contains the absolute value for a constant, or the relative address to the start of the code  lock for a relocatable address.
	
  * *name* (string) : contains the name.

  * *filename* (string) : contains the source file name where the symbol was defined.

  * *line_nr* (long) : contains the source line number where the symbols was defined.

* *External Symbols* : contains a set of external symbols referred by this module. Each name has the following format:
  
	* *name* (string) : contains the name.   

* *Module Name* (string) : contains the module name.

* *Binary Code Sections* : contains the binary code of the module with the following format:

  * *length* (long) : defines the total code length, and contains -1 to signal the end.

  * *section* (string) : source file section name. 
 
  * * [ORG](#11_25_)  Address* (long) : contains the user defined  [ORG](#11_25_)  address for the start of this section, -1 for no  [ORG](#11_25_)  address was defined, or -2 to split section to a different binary file. If multiple sections are given with an  [ORG](#11_25_)  address each, the assembler generates one binary file for each section with a defined  [ORG](#11_25_) , followed by all sections without one.

  * *ALIGN* (long) : defines the address alignment of this section, -1 if not defined. The previous section is padded to align the start address of this section.

  * *code* (byte[length]) : contains the binary code.



----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_2_></a>

### 5.2. Library File Format

The library file format is a sequence of object files with additional
structures.

    |Addr | Type   | Object                                                 |
    +-----+--------+--------------------------------------------------------+
    |   0 | char[8]| 'Z80LMF14' (file signature and version)                |
    |   8 | word   | *Object File Block*                                    |
    |     |        | ...                                                    |

* *Object File Block* : contains the following format: 

  * *next* (long) : contains the file pointer of the next object file in the library, or -1 if this object file is the last in the library.

  * *length* (long) : contains the length of this object file, or 0 if this object files has been marked "deleted" and will not be used.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=5_3_></a>

### 5.3. Format History

* version *01* : original z80asm version
* version *02* : allow expressions to use standard C operators instead of the original (legacy) z80asm specific syntax. 
* version *03* : include the address of the start of the assembly instruction in the object file, so that expressions with ASMPC are correctly computed at link time; remove type 'X' symbols (global library), no longer used.
* version *04* : include the source file location of expressions in order to give meaningful link-time error messages.
* version *05* : include source code sections.
* version *06* : incomplete implementation, fixed in version *07*
* version *07* : include * [DEFC](#11_6_) * symbols that are defined as an expression using other symbols and are computed at link time, after all addresses are allocated.
* version *08* : include a user defined  [ORG](#11_25_)  address per section.
* version *09* : include the file and line number where each symbol was defined.
* version *10* : allow a section alignment to be defined.
* version *11* : allow big-endian 16-bit expressions to be patched; these big-endian values are used in the ZXN coper unit.
* version *12* : allow the target expression of relative jumps to be computed in the link phase
* version *13* : add 8-bit signed and unsigned values extended to 16-bits
* version *14* : add 24-bit pointers



----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_></a>

## 6. z80asm Syntax

THE  [Z80](#8_)  CROSS ASSEMBLER (z88dk)
===============================

Version: v2.6.1 (October 3rd, 2014)

Thank you for purchasing a copy of this cross assembler. We have made an effort to program an easy user interface and efficient assembler source file compiling. The object file and library format is an invention of our own and has also been included in this documentation. We hope that you will enjoy your  [Z80](#8_)  machine code programming with our assembler.

We have made an effort to produce a fairly easy-to-understand documentation. If you have any comments or corrections, please don't hesitate to contact us:

Gunther Strube  
Gl. Kongevej 37, 4.tv.  
DK-1610 Kobenhavn V  
Denmark  
e-mail [gbs@image.dk](mailto:gbs@image.dk)

1\. Running the assembler
-------------------------

Since assemblers are developed for programmers the user interface have been implemented according to the standard command line environment (CLI) used in most operating systems; however, if only a GUI interface is available (as in MacOS) then a command shell must be executed to allow standard command line input. The  [Z80](#8_)  Module Assembler command line interface may change if other programmers wish to implement a different interface to the assembler. You have the opportunity!


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_1_></a>

### 6.1. Help page

When executing the assembler from the command line with the  [-h](#6_3_1_)  option, a help page is displayed. The page shortly explains the syntax of parameter specification and which parameters are available.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_2_></a>

### 6.2. Command line

The syntax of the assembler parameters is a straightforward design. Filenames or a project file are always specified. The options may be left out:

    z80asm [options] <filename {filename}> | <@modulesfile>

As seen above \<options\> must be specified first. Then you enter the names of all files to be assembled. You either choose to specify all file names or a @\<project-file\> containing all file names. File name may be specified with or without the 'asm extension. The correct filename parsing is handled automatically by the assembler. As seen on the syntax at least one source file must be specified and may be repeated with several file names. Only one project file may be specified if no source file names are given.

Many of the parameters are preset with default values which gives an easy user interface when specifying the assembly parameters. Only advanced parameters need to be specified explicitly. The help page displays the default parameter values at the bottom of the page.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_></a>

### 6.3. Command line options

Options are used to control the assembly process and output. They are recognized by the assembler when you specify a leading minus before the option identifier ('-'). Options are always specified before file names or project files.

When the assembler is executed options are all preset with default values and are either switched ON or OFF (active or not). All options have a single letter identification. Upper and lower case letters are distinguished, which means that 'a' might be different command than 'A'. If an option is to be turned off, you simply specify a 'n' before the identification, e.g. -nl which selects listing files not to be created by the assembler.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_1_></a>

#### 6.3.1. -h : Show help screen

Shows a help screen with the available options.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_2_></a>

#### 6.3.2. -v : Information during assembly

The assembler writes a certain amount of information to its program window (or on the command line screen) to let the user know what is happening. For each source file both the current assembler pass and include file names are displayed. During linking all object file names and specified names of library modules are displayed. When all files have been processed and the necessary output generated, a status message is displayed which informs you of how many errors occurred (or none). You will also be reported with a message of how many source lines that have been parsed during a compilation. This information can be very convenient during a large compilation project, especially on relatively smaller computers (with less powerful processors) and no hard disk. If you own a powerful 32-bit computer equipped with a fast hard disk, assembly information is displayed at high speed (and almost impossible to read); it may then be useful to turn this information off specifying a -nv option at the command line.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_3_></a>

#### 6.3.3. -e\<ext\> : Use alternative source file extension

The default assembler source file extension is ".asm". Using this option, you force the assembler to use another default extension, like ".opt" or ".as" for the source file.

The extension is specified without the ".". Only three letters are accepted - the rest is discarded.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_4_></a>

#### 6.3.4. -M\<ext\> : Use alternative object file extension

The default assembler object file extension is ".obj". Using this option, you force the assembler to use another default extension, like ".o" as the object file name extension.

The extension is specified without the ".". Only three letters are accepted - the rest is discarded.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_5_></a>

#### 6.3.5. -l : Create listing file output

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
<a id=6_3_6_></a>

#### 6.3.6. -s : Create symbol table

Symbol tables contains the integer form of symbolical names and constants that has been parsed and generated during a compilation of a source file. The structure of the symbol table is divided into two columns. The first contains the parsed symbol names, converted to uppercase. The second column contains the generated value of the symbol name.

All symbol values are displayed in signed 32-bit hexadecimal notation.

The two columns are separated by tabulators which represents a default value of 8 spaces per tabulator. The width of the symbol name column is defined as the tabulator distance multiplied by 4. The default width of the name column is 4 \* 8 = 32 spaces.

The symbol table will be written to the end of the appropriate listing file, if listing file and symbol table output is enabled. If no listing file output is enabled, the symbol table will be written to a separate file, identified with the base name of the source file module and given the 'sym' extension.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_7_></a>

#### 6.3.7. -d : Assemble only updated files

Assemblers usually force compiles all specified files. This is also possible (as default) for the  [Z80](#8_)  Module Assembler. In large application project with 15 modules or more it can be quite frustrating to compile all every time. The solution is to only assemble updated files and leave the rest (since they have been compiled to the programmers knowledge).

But in a long term view it is better to just compile a project without thinking of which files need to be compiled or not. That can be done with the  [Z80](#8_)  Module Assembler. By simply specifying the  [-d](#6_3_7_)  parameter at the command line, only updated source files are assembled into object files - the rest are ignored.

Using the  [-d](#6_3_7_)  option in combination with a project file gives the best project setup for a large compilation; compile your projects without worrying about which module is updated or not.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_8_></a>

#### 6.3.8. -b : Link/relocate object files

The  [-b](#6_3_8_)  option must be used if you want to create an executable  [Z80](#8_)  machine code output file of your previously created object files. You may also use the  [-a](#6_3_9_)  option which is identical in functionality but also includes the  [-d](#6_3_7_)  option. In other words assemble only updated source modules and perform linking/relocation of the code afterwards.

*   Pass 1:  
    When the linking process begins with the first object module, it is examined for an  [ORG](#11_25_)  address to perform the absolute address relocation of all the object module machine code. The  [ORG](#11_25_)  (loading address for memory) will have to be defined in the first source file module. If not, the assembler will prompt you for it on the command line. The  [ORG](#11_25_)  address must be typed in hexadecimal notation. If you never use the  [ORG](#11_25_)  directive in your source files, you can always explicitly define one at the command line with the  [-r](#6_3_12_)  option.  
      
    The next step in the linking process is loading of the machine code from each object module, in the order of the specified modules. Pass 1 is completed with loading all local and global symbol definitions of the object modules. All relocatable address symbols are assigned the correct absolute memory location (based on  [ORG](#11_25_) ).  
    
*   Pass 2:  
    The address patching process. All expressions are now read and evaluated, then patched into the appropriate positions of the linked machine code.  
      
    When all expressions have been evaluated the machine code is completed and saved to a file named as the first source file module, and assigned the 'bin' extension.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_9_></a>

#### 6.3.9. -a : Combine -d and -b

Same as providing both  [-b](#6_3_8_)  (link/relocate object files) and  [-d](#6_3_7_)  (assemble only updated files).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_10_></a>

#### 6.3.10. -o\<binary-filename\> : Binary filename

Define another filename for the compiled binary output than the default source filename of the project, appended with the ".bin" extension.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_11_></a>

#### 6.3.11. -m : Create address map information file

When the linking process has been completed and the machine code saved to a file, the address map file is produced. This file contains the information of all relocated address symbols with their assigned absolute addresses. Further, an id is written that displays the scope of the symbols; local ('L') or global ('G'). The last item of each address map line, identified after the colon, is the name of the module which the symbol name belongs to.

The address map file is divided into two sections; the first produces the symbol list in alphabetical order (fast lookup of symbol names), the second in order of their address counterparts. The section is more used during a debugging session when an address in a disassembly needs to be identified with a name. The second list also gives a chronological view (composition) of the linked modules.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_12_></a>

#### 6.3.12. -r\<hex-address\> : Re-define the ORG relocation address

During the linking phase of the assembler the  [ORG](#11_25_)  address that defines the position in memory where the code is to be loaded and executed, is fetched from the first object module file. You can override this by specifying an explicit address origin by entering the  [-r](#6_3_12_)  option followed by an address in hexadecimal notation at the command line, e.g.:

    z80asm  [-b](#6_3_8_)  -r4000 file.asm

which specifies that your code is to be relocated for address 4000h (16384) onwards.

Using the  [-r](#6_3_12_)  option supersedes a defined  [ORG](#11_25_)  in the object file. You could for example have defined the  [ORG](#11_25_)  to 8000h (32768) in your first source file, then compiled the project. This would have generated machine code for memory location 8000h (segment 2 in the Cambridge  [Z88](#6_6_) ). Since the object files are generated it is easy to link them again with another  [ORG](#11_25_)  address by just using the  [-r](#6_3_12_)  option. The linking process does not alter the information in object files - they are only read. The same project could then easily be re-linked to another address, e.g.

    z80asm  [-b](#6_3_8_)  -r2000 file.asm


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_13_></a>

#### 6.3.13. -R : Generate address independent code

The  [Z80](#8_)  processor instruction set allows only relative jumps in maximum +/- 128 bytes using the JR and DJNZ instructions. Further, there is no program counter relative call-to-subroutine or jump-to-subroutine instruction. If you want a program to be address-independent no absolute address references may be used in jump or call instructions. If you want to program  [Z80](#8_)  address independent code you can only write small routines using the JR and DJNZ instructions. With a restricted interval of 128 bytes you can imagine the size of those routines! Programming of large applications using address independency is simply impossible on the  [Z80](#8_)  processor with the basic instruction set available. You can only define a fixed address origin ( [ORG](#11_25_) ) for your machine code to be loaded and executed from. However, there is one solution: before the code is executed an automatical address-relocation is performed to the current position in memory. This is done only once. The penalty is that the program fills more space in memory. This is unavoidable since information must be available to define where the address relocation has to be performed in the program. Further, a small routine must be included with the program to read the relocation information and patch it into the specified locations of the program. It is impossible to determine the extra size generated with a relocation table. We assume an extra size of 3 - 3.5K for a typical 16K application program.

You can generate address independent code using the  [-R](#6_3_13_)  option accompanied with the  [-a](#6_3_9_)  or  [-b](#6_3_8_)  option. There is no other requirements. The relocatable code may be useful for programmers using the Cambridge  [Z88](#6_6_)  who want to use machine code in the BBC BASIC application environment. This can easily be interfaced with the DIM statement to allocate memory for the machine code program, and issue a  [CALL](#11_2_)  or USR() to execute the machine code.

Please note that the linking process with the  [-R](#6_3_13_)  option addresses your code from 0 onwards. This is necessary when the runtime relocation is performed by the relocator (just before your program is executed). This can be examined by loading the address map file into a text editor.

The principle of relocation is in fact a self-modifying program. You cannot relocate a program that has been blown into an EPROM (cannot be modified). You may only execute relocatable programs in dynamic memory (RAM).

The relocator is built into the  [Z80](#8_)  Module Assembler. The relocation table is created during the linking phase. When all object modules have been linked and the appropriate machine code generated, the process is ended with first copying the relocator routine into the executable file, then a relocation table and finally the compiled machine code program. Any defined  [ORG](#11_25_)  in your code is superseded - this is not necessary in a relocatable program!

Two rules must be obeyed when using relocatable programs:

1.  The IY register must have been set up to point at the address where your program resides. The first code is in fact the relocator which manipulates your code on the basis of the IY register. If IY is not setup properly your machine code program will be relocated for an address it is not resided at. On execution your might then call a random address (on the basis of the random IY register).  
    
2.  Don't use the alternate register set for parameter passing between the caller (of your code) in the main code and the relocated program. The following registers are affected by the initial relocation process:

        AFBCDEHL/IXIY/........ same  
        ......../..../afbcdehl different

You still have all the main registers for parameter passing which is more than sufficient for average programming.

When your address-independent code is stored to the file, a message is displayed which informs the user of how many bytes the relocation header consists of. This constant is useful since it informs you of the distance between the relocation header and the start of your code. The map file automatically reflects the relocation header. All addresses of your code has been modified to include the relocation header. Please note that all addresses in the map file are defined from address 0. When your code is placed in an absolute memory address, and you need to perform a debugging session, you can find your specific label address by adding the constant from the map file to the memory origin of your code. The inbuilt relocator routine may be examined by extracting the "relocate.asm" file from the " [Z80](#8_) src.zip" compressed file resource.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_14_></a>

#### 6.3.14. -g : Create global address definition file

With this option it is possible to generate a  [DEFC](#11_6_)  directive definition file of all globally declared identifiers in a file project (declared with the  [XDEF](#11_27_)  directive). These global definitions are calculated from the specified  [ORG](#11_25_)  address (from first module or the  [-r](#6_3_12_)  option). This feature is very useful, if you want to get access to routines from a separate compilation. If the two code compilation were placed in different banks of the  [Z88](#6_6_) , it would be possible to know the correct address of a routine just by using the  [DEFC](#11_6_)  address definition previously compiled. We used this facility to access routines in the two 8K halves of the segment 0 debugged version. This debugger actually swaps one of the two 8K blocks in and out of segment 0 when needed to call an 'external' routine. Applications on the  [Z88](#6_6_)  may only access the upper 8K of segment 0. A 16K application therefore needs to be split in 8K halves and paged in when needed to be run in this area. Tuff!


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_15_></a>

#### 6.3.15. -D\<symbol\> : Define a static symbol

This option is useful if you want to produce conditional compilations. The symbol defined here will be active throughout the compilation. We used this feature to compile machine code on different computer platforms (QL, IBM, LINUX or  [Z88](#6_6_) ). Specify the symbol immediately after the option identifier, i.e.

    z80asm  [-b](#6_3_8_)  -DZ88 file.asm


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_16_></a>

#### 6.3.16. @\<project-file\> :1.3.16. Using a project file

Project files defines all file names of a project. The file name standard stored in a project file obeys the operating system notation.

Instead of specifying every module file name at the command line, a simple reference of a project file can be made instead. According to the rules of the specification of parameters you specify either your source file modules or use a project file. The project file specification is of course much faster. An example:

    z80asm  [-a](#6_3_9_)  main pass1 pass2 link asmdrctv z80instr

This command line will compile all specified module file names into a single executable file called "main.bin". However if a project file 'assembler' were created already containing the same file names, the command line would have been:

    z80asm  [-a](#6_3_9_)  @assembler

\- much easier!

A project file only contains file names. Each file name is separated by a newline character \\n. The new line character may be different on various computer platforms - but the assembler interprets it correctly. The contents of a project file may look like this:

    z80asm  
    z80pass1  
    z80pass1  
    modlink

Project files are easily created using a simple text editor.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_17_></a>

#### 6.3.17. -i\<library-file\> : Include library modules during linking/relocation

This option allows compilation time linking of external machine code, better known as library routines. Much, much programming time can be saved by producing a set of standard routines compiled into library files. These may then be included later in application project compilations. The command line option allows specification of several library files. For each library reference in an application module, all library files will be scanned for that particular module. The filename (inclusive directory path) of the library may be specified explicitly on the command line immediately after the  [-i](#6_3_17_)  identifier. If you omit the filename, a default library filename will be used by the assembler. This default filename is defined by creating the environment variable " [Z80](#8_) \_STDLIB=\<library-file\>. Please refer to your operating system documentation on how to create environment variables.

Library files are recognised by the ".lib" extension.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_18_></a>

#### 6.3.18. -L\<library-path\> : Add directory to search path for libraries

Tells the assembler where to look for library files.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_19_></a>

#### 6.3.19. -I\<include-path\> : Add directory to search path for INCLUDE

Tells the assembler where to look for  [INCLUDE](#11_18_)  files.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_20_></a>

#### 6.3.20. -x : Create a library

A library file is composed of object files surrounded by a few file structures. The library file format (and object file format) may be found at the end of this documentation. A library is simply a set of independent routines (that may refer to each other) put together in a sequential form. You may only specify a single  [-x](#6_3_20_)  option on the command line. A filename may be explicitly defined (including device and path information) to determine the storage location of the library. As in  [-i](#6_3_17_)  you may omit the filename to use the default filename identified by the " [Z80](#8_) \_STDLIB" environment variable. A library routine must be defined using a simple  [XLIB](#11_28_)  directive with an identical address name label definition. Please refer to further information later in this documentation. The " [Z80](#8_) lib.zip" contains the standard library with all corresponding source files. Have a look at them - they clearly displays how to compose a library routine.

One very important aspect of libraries is the time that the assembler spends searching through them. To optimize the search you should place your routines in a "topological" order, i.e. routines that access other library routines should be placed first. In most situations you avoid redundant sequential searching through the library.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_21_></a>

#### 6.3.21. -t\<number\> : Define tabulator distance for text output files

To save storage space the  [Z80](#8_)  cross assembler output files (listing, map, symbol and  [XDEF](#11_27_)  definition files) uses a tabulator control character instead of spaces. The benefit is about 30% compressed files.

The tabulator distance defines the distance of space between each tabulator it represents. The default value is 8 spaces per tabulator.

The tabulators are used to separate two columns of information. The first column contains a name of some sort. Since names have variable length, a size of the column is defined. The Assembler defines the size of the column by multiplying the current tabulator distance with 4, i.e. giving a default size of 4\*8 = 32 'spaces'. This is usually more than enough for most name definitions parsed from source files.

You may redefine the tabulator distance by using the  [-t](#6_3_21_)  option immediately followed by a decimal number, e.g. -t4 for defining a tabulator distance of 4. The width of the first column will then be 4\*4 = 16 'spaces'.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_22_></a>

#### 6.3.22. -RCMX000 : Support the RCM2000/RCM3000 series of Z80-like CPU's

This option disables assembly opcodes not available in the RCM2000/RCM3000 series of  [Z80](#8_) -like CPU's.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_23_></a>

#### 6.3.23. -plus : Support for the Ti83Plus

Defines how the  [INVOKE](#11_19_)  command is coded: either as a RST 28H instruction (option on) or as a regular  [CALL](#11_2_)  instruction (option off).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_24_></a>

#### 6.3.24. -IXIY : Swap IX and IY registers

Swaps the IX and IY registers; references to IX are coded as IY and vice versa. Useful when the assembler is used as a back-end of a compiler that uses one of the index registers that is used in the target platform.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_3_25_></a>

#### 6.3.25. -C : Enable LINE directive

Enables the  [LINE](#11_21_)  directive to synchronize error message line numbers with the line numbers from the source file.

2\. An overview of assembler features and related files
-------------------------------------------------------


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_4_></a>

### 6.4. The Z88 operating system definition files

You will find header files containing all operating system definitions as defined in the  [Z88](#6_6_)  Developers' Notes V3 in the "OZdefc.zip" file. This makes the operating system interface programming a lot easier.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_5_></a>

### 6.5. The supplied standard library Z80 source files

We have supplied a standard library with useful routines for both beginners and professional machine code programmers. All source files are supplied for having the opportunity to study and improve the routines. However some routines are programmed especially for the  [Z88](#6_6_)  operating system and may not be of use for other  [Z80](#8_)  based computers unless thoroughly rewritten. The standard library source files may be found in the " [Z80](#8_) lib.zip" file.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_6_></a>

### 6.6. Z88 module assembler application source

We have supplied the complete source of the  [Z88](#6_6_)  module assembler application. This allows you to evaluate many aspects of programming applications on the  [Z88](#6_6_) . Further, most features of the assembler are mirrored in these source files; using directives, the free format of  [Z80](#8_)  mnemonics, library routine access, modular file design, labels, using expressions, comments, data structure manipulation and good programming design.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_7_></a>

### 6.7. File based compilation

This assembler is completely file based, i.e. all parsing and code generation is manipulated via files on storage medias such as harddisks or floppy disks (or file based RAM-discs).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_8_></a>

### 6.8. Modular source file design

A compilation may be split into individual source files that either can be linked together during assembly as a single module or assembled as separate source file modules. Separate source file modules saves compilation time and memory. Further, this design is much more straightforward and much more logically clear in a design phase of a large compilation project than one huge kludge of a source file.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_9_></a>

### 6.9. Scope of symbols in source modules

All source modules may refer to each others symbols by using  [EXTERN](#11_12_)  directives. This means that you refer to external information outside the current source module. The opposite of an external module reference is to declare symbols globally available using a  [PUBLIC](#11_26_)  directive, i.e. making symbols available to other source modules. Finally it is possible to have local symbols that are not known to other source modules than the current. A label or constant that has not been declared with  [EXTERN](#11_12_) ,  [PUBLIC](#11_26_)  or  [GLOBAL](#11_14_)  is local to the module.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_10_></a>

### 6.10. Using arithmetic and relational expressions

All directives that require a numerical parameter or  [Z80](#8_)  mnemonics that use an integer argument may use expressions. Expressions may be formed by all standard arithmetic operators and relational operators. Even binary operators are implemented. All expressions may contain external identifiers and is automatically resolved during the linking phase. Only certain directives require compilation time evaluable expressions.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_11_></a>

### 6.11. Source file manipulation

To allow the Assembler to execute in multitasking environments such as LINUX and QDOS, all source input files are opened as shared I/O. This allows other programs to access the source files while the assembler is working. All output files (both text and binary files) are opened/created for exclusive use; other programs will have no access to those files until they have been closed.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_12_></a>

### 6.12. Free format of assembler source files

The source files may be written in a free format. No fixed position columns as needed as in the COBOL programming language. All text may be typed in mixed case (the assembler converts all text input to uppercase). Tabulators may be used freely (instead of spaces which also saves source file space) to suit the programmers own habits of structured text layouts. However, one rule must be obeyed: syntax of  [Z80](#8_)  assembler mnemonics and most directives must be completed on individual lines. Text files using different OS dependant line feed standard are parsed properly; line feed types CR, LF or CRLF are automatically recognized. So you can easily compile your sources from Linux/UNIX on an MSDOS platform.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_13_></a>

### 6.13. Specification of filenames

Specification of file names in source files are always enclosed in double quotes. The assembler just collects the filename string and uses this to open a file. This way all filename standards may be used as defined on different operating system platforms.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_14_></a>

### 6.14. Including other source files into the current source file

The need for header file information such as operating system constants or data structures is often indispensable for source file modules. Instead of copying the contents of those files into each module, it is possible to include them at run time (during parsing). Infinite include file levels are permitted, i.e. included files calling other files.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_15_></a>

### 6.15. Conditional assembly in source file modules

Large compilation projects often need to compile the application in several variations. This can be achieved with enclosing parts of the source with conditional directives for the different variations of the application. This may also be useful if the assembler source is ported to several platforms, where inclusion of other source files (such as header files) are using different filename standards. The conditional directives  [IF](#11_15_) ,  [IFDEF](#11_16_) ,  [IFNDEF](#11_17_) , ELSE, and ENDIF may be nested into infinite levels.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_16_></a>

### 6.16. Undocumented Z80 instruction code generation

We have included the syntax parsing and code generation of the undocumented  [Z80](#8_)  instructions for the sake of completeness. However, IM 2 interrupts must be disabled before they are executed (an interrupt may otherwise occur in the middle of the instruction execution). Many games on the ZX Spectrum have used them to protect the code from prying eyes. The  [Z88](#6_6_)  native debugger code uses some of the undocumented instructions for fast access to register variables.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_17_></a>

### 6.17. Object file generation

The  [Z80](#8_)  Module Assembler generates object files that contains the compressed version of an assembled source module. The information herein contains declared symbols (local, global and external), expressions, address origin, module name and machine code. The object file modules are much smaller than their source file counterparts (often smaller than 2K).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_18_></a>

### 6.18. Transfer of object files across platforms

The  [Z80](#8_)  Module Assembler is already implemented on several different computer platforms. You may freely transfer the object files and use them as a part of another cross-compilation. There is no system-dependent information in the object files.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_19_></a>

### 6.19. Date stamp controlled assembly

To avoid unnecessary compilation of source file modules, it is possible to let the assembler compile only recently updated source file modules by comparing the date stamp of the source and the object file modules. Source file modules that are older than object file modules are ignored. This facility is indispensable in large compilation projects.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_20_></a>

### 6.20. Listing files

The assembler may generate listing files that contain a copy of the source file with additional code generation information of  [Z80](#8_)  mnemonics dumped in hexadecimal format. The listing files are formatted with page headers containing time of assembly and the filename. Line numbers are included which corresponds to the source file lines.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_21_></a>

### 6.21. Symbol information

All symbol generated values used in source modules may be dumped to the end of the listing file or as a separate symbol file. If the symbol table is dumped into the listing file, each symbol will be written with page references of all occurrences in the listing file. Address symbols (labels) are addressed relative to the start of the module. Symbol constants are written as defined in the source. The symbol table is written in alphabetical order with corresponding values in hexadecimal format.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_22_></a>

### 6.22. Linking and relocation of object modules into executable Z80 machine code

To obtain an executable  [Z80](#8_)  machine code file it is necessary to link all assembled object modules and relocate them at a defined address, where the code is to be executed at in the computers' memory. The linking & relocation is performed automatically after completed assembly of all specified source file modules. The  [ORG](#11_25_)  relocation address is specified in the first object module.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_23_></a>

### 6.23. Address map files

The address map is invaluable information during a debugging session of your compiled program. This file contains all symbolical address labels with their generated address constants after a completed linking/relocation of all modules into executable machine code. The map file is ordered in two groups; the first list contains all symbol names ordered alphabetically with corresponding address constants, the second list contains all symbols ordered by their address value (in chronological order).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_24_></a>

### 6.24. Symbol address definition files

As with address map files this contains information of globally declared symbolical address labels, relocated to their absolute position as for the compiled machine code file. However, the format is completely different; all symbols are created as constant definitions to be included as a header file into another source file and assembled. This is useful if you want to call subroutines compiled separately in another project (originated in a different memory setup).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_25_></a>

### 6.25. Error files

Error files are created by the assembler during processing. If any errors should occur, they will be written to stderr and copied to this file containing information of where the error occurred in the source module. If no errors were found, the error file is automatically closed and deleted.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=6_26_></a>

### 6.26. Creating and using object file libraries for standard routines

Machine programmers often re-use their standard routines. We have implemented a file format for generating libraries using the existing object file modules. Using a simple set of rules makes it very easy to create your own libraries from your source file modules. Documentation of the library file format is included in this documentation. At command line infinite number of libraries may be specified. All will be searched during linking of your object modules for referenced library routines.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_></a>

## 7. Executing the cross assembler and environment variables

The following text describes how to execute the assembler and defining the environment variables used by the assembler.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_1_></a>

### 7.1. The environment variables

The assembler uses two environment variables:

*   " [Z80](#8_) \_STDLIB" define the default filename of the standard library filename.
*   " [Z80](#8_) \_OZFILES" define the default path of the standard  [Z88](#6_6_)  OZ system headers.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_2_></a>

### 7.2. Running in the QDOS/SMSQ operating system environment

z80asm may be compiled using the C68 compiler developed by the Walker brothers. You also need the 'env\_bin' file. This is necessary to install into the operating system before using the assembler. It contains the environment variable facility integrated on UNIX, MS-DOS and many other operating systems. Include the following line into your BOOT upstart program:

    LRESPR win1_ext_env_bin

The device and path 'win1\_ext\_' is just an example of where to store your system extension file. You may have it on your BOOT disk as well. If you don't have ToolKit 2 on your system use the following line in your BOOT program:

    envext=RESPR(1024):LBYTES win1_ext_env_bin,envext: [CALL](#11_2_)  envext

Use the following in your BOOT file to set the environment variables:

    SETENV "Z80_OZFILES=win1_z80_src_oz_"  
    SETENV "Z80_STDLIB=win1_z80_standard_lib"

The example file names are only a demonstration. Change them as necessary.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_3_></a>

### 7.3. Running in the LINUX/UNIX operating system environment

This program can be executed on the LINUX operating system and any other UNIX. The sources are strictly ANSI C and uses only library calls that are described in Brian W. Kernighan and Dennis M. Ritchie C books. The important thing to remember before compiling it under UNIX, is in which direction the integers are stored by the processor architecture; the famous Big Endian and Little endian concept. The config.h file uses a "ENDIAN" definition when z80asm must interpret integers in big endian order. Please set this definition according to your system's processor architecture. Most people uses the Intel processor when running Linux - this is a little endian architecture (so you don't need the ENDIAN define...).

You can compile z80asm using GNU C compiler simply by entering the following on your command line:

    gcc  [-o](#6_3_10_)  z80asm -O2 *.c

The environment variables needed by z80asm are easily added in your accounts login script, ".profile" or ".bash\_profile" in your home directory.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=7_4_></a>

### 7.4. Running in the MS-DOS operating system environment

This program can be executed on all MSDOS operating systems using the INTEL 8086 processor up to high speed 80386/80486/Pentium processors on IBM & compatible computers. Add the source files to your favorite C compiler system in MSDOS or Windows (here you need to specify compilation options o a console application).

The environment variables are easily put into your AUTOEXEC.BAT file. Simply specify:

    SET Z80_OZFILES=C:\ [Z80](#8_) \OZ\
    SET Z80_STDLIB=C:\ [Z80](#8_) \STANDARD. [LIB](#11_20_) 

Choose your own settings if you like.

If you want the assembler to be accessible in any path, add the path location of the assembler in the PATH search line:

    SET PATH=%PATH%;C:\ [Z80](#8_) 


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_></a>

## 8. Z80 module assembler file types


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_1_></a>

### 8.1. The assembler file types and their extension names

The  [Z80](#8_)  Module Assembler uses several different filename extensions to distinguish the type of files processed. The base name of the source file is used to create the various assembler output file types. The following chapters explains the available files.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_2_></a>

### 8.2. The file name extension identifier

The file name extension identifier may be different from platform to platform. UNIX has no defined standard. MSDOS and TOS uses '.'. QDOS uses the '\_' identifier. SMSQ also allows the '.' extension identifier.

The Assembler implemented on the supplied platforms is defined with the correct extension identifier. You can see this on the Assembler help page (executing the program with no parameters).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_3_></a>

### 8.3. File types


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_3_1_></a>

#### 8.3.1. The source file extension, asm

The extension for assembler mnemonic source files is 'asm'. Source files are specified by the user with or without the extension - whatever chosen, the assembler will investigate automatically what is needed to read the source files.

You may override the default extension with the  [-e](#6_3_3_)  option.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_3_2_></a>

#### 8.3.2. The object file extension, obj

The extension for object files is 'obj'. The base file name is taken from the corresponding source file name. This file is generated by the assembler from parsing the source file and contains intermediate generated machine code, an address origin of the machine code, symbol information and expressions.

You may override the default extension with the  [-M](#6_3_4_)  option.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_3_3_></a>

#### 8.3.3. The error file extension, err

The extension for error files is 'err'. Before beginning processing the source files, an error file is created. If any errors should occur, they will be written to this file containing information of where the error occurred. If no error were found, the error file is automatically closed and deleted.

Error files are simple text files that can be loaded by any text editor for evaluation.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_3_4_></a>

#### 8.3.4. The listing file extension, lst

The extension for listing files is 'lst'. The base file name is taken from the corresponding source file name. This file is generated by the assembler and contains a hexadecimal output of the generated machine code that corresponds to the  [Z80](#8_)  mnemonic instruction or directive, followed by a copy of the original source line. If selected, the symbol table is dumped at the end of the listing file.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_3_5_></a>

#### 8.3.5. The symbol file extension, sym

The extension for symbol table files is 'sym'. The base file name is taken from the corresponding source file name. The symbol table file contains information about the defined and used symbols of the source file and their generated values (labels and constants). The symbol file is only created if listing file output is disabled.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_3_6_></a>

#### 8.3.6. The executable file extension, bin

The extension for executable  [Z80](#8_)  machine code files is 'bin'. The base file name is taken from the first specified source file name at the command line (or project file). This is the linked and relocated output of object files and may be executed by the  [Z80](#8_)  processor. You may override this default behaviour by using the  [-o](#6_3_10_)  option and specify your own output filename (and extension).

You may override this default behavior by using the  [-o](#6_3_10_)  option and specify your own output filename and extension.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_3_7_></a>

#### 8.3.7. The address map file extension, map

The extension for address map files is 'map'. The base file name is taken from the first specified source file name at the command line (or project file). This file is generated by the assembler and contains a list of all defined address labels from all linked/relocated modules with their calculated (absolute) address in memory.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_3_8_></a>

#### 8.3.8. The definition file extension, def

The extension for global address label definition files is 'def'. The base file name is taken from the first specified source file name at the command line (or project file). This file is generated by the assembler and contains a list of all globally declared address labels with their calculated (absolute) origin address, fetched only during assembly of source file modules. The format of the list contains constant definitions (addresses) and may be parsed e.g. as include files for other projects.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=8_3_9_></a>

#### 8.3.9. The library file extension, lib

Library files are identified with the 'lib' extension. Library files may be created using the  [-x](#6_3_20_)  option. Library may be included into application code during linking of object modules with the  [-i](#6_3_17_)  option.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_></a>

## 9. Compiling files


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_1_></a>

### 9.1. The assembler compiling process

The  [Z80](#8_)  Module Assembler uses a two stage compilation process; stage 1 parses source files and generates object files. Stage 2 reads the object files and links the object file code, completes with address patching and finishes with storing the executable code.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_1_1_></a>

#### 9.1.1. Stage 1, parsing and code generation of all source files, object file generation

A source file is being parsed for  [Z80](#8_)  mnemonics and directives. An object file is created to hold information of module name, local, global and external symbol identifiers, expressions and the intermediate code generation (but address and other constant information). During pass 1 all  [Z80](#8_)  mnemonics are parsed and code is generated appropriately. All expressions are evaluated; expressions that contain relocatable address symbols or external symbol are automatically stored into the object file. Expressions that didn't evaluate are preserved for pass 2. When a source file has been read successfully to the end, pass 2 is started. During pass 2 all non-evaluated expressions from pass 1 are re-evaluated and stored to the object file if necessary. Errors are reported if symbols are still missing in expressions. When all expressions are evaluated and no errors occurred, all "touched" symbols (used in expressions) are stored into the object file, with scope, type and value. Then, the module name and generated code is stored to the object file. Various file pointers to sub-sections of the object file is resolved. The completion of stage 1 is to produce the symbol table output (either appended to listing file if selected or as a separate file).

This process is performed for all specified source modules in a project.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_1_2_></a>

#### 9.1.2. Stage 2, linking object files and library modules, producing executable code

Pass 1 of the linking loads information from each object file in the project; the  [ORG](#11_25_)  address is fetched, identifiers (resolving scope, and absolute values) loaded, and machine code linked. During this pass all external library modules are fetched and linked with the object modules (if a library is specified from the command line). When all modules have been loaded, pass 2 begins. Pass 2 then reads each expression section from all object modules (including library modules), evaluates them and patches the value into the appropriate position of the linked machine code. When all expressions have been evaluated successfully the executable code is stored. If selected, the address map file is produced from the current symbol table resided in the data structures of the assembler's memory is stored to a text file.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_2_></a>

### 9.2. File names

Specification of file names follows the convention used on the various platforms that the assembler is ported to. Please read your operating systems manual for more information.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_3_></a>

### 9.3. Portability of assembler file names

If you are going to port your  [Z80](#8_)  Module Assembler files across several platforms a few hints may be worth considering:

Avoid special symbols in file names like '\_', '#' and '.' . They may have special meaning on some operating system platforms. Use only 7-bit standard ASCII letters in file names ('A' to 'z'). Non English language letters are not always allowed, and further they may not be interpreted correctly when ported to another platform. Avoid too long file names. Some operating systems have boundaries for length of filenames in a directory path. For example MS-DOS only allows 8 characters in a file name (followed by an extension). Others may have no boundaries.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_4_></a>

### 9.4. Source file structure

The composition of a source file module is completely free to the programmer. How he chooses to place the source code on a text line has no effect of the parsing process of the assembler. The linefeed interpretation is also handled by z80asm - it understands the following formats:

*   \<LF\> (used by QDOS/SMSQ/UNIX/AMIGA);
*   \<CR\>\<LF\> (used by MSDOS);
*   \<CR\> (used by  [Z88](#6_6_) /MacIntosh).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_5_></a>

### 9.5. Using local, global and external symbols

In larger application projects it is unavoidable to use a modular programming design, i.e. splitting the source into several individual files. This approaches the popular top - down design where you can isolate the problem solving into single modules. The outside world just needs to know where the routine is to be called by linking the modules with a few directives.

In the  [Z80](#8_)  Module Assembler you only need two directives to accomplish just that: the  [XREF](#11_29_)  and  [XDEF](#11_27_)  directives.

 [XREF](#11_29_)  declares a symbol to be external to the current source file module. This tells the assembler that all expressions using that symbol is not to be evaluated until the compiled object modules are to linked and relocated together. An expression that contains this symbol is simply stored into the object file.

 [XDEF](#11_27_)  declares a symbol to be created in this module and made globally available to other modules during the linking/relocation phase. All expressions that contain a globally declared symbol is automatically stored into the object file.

When a symbol is created and is neither declared external or global, it is implicitly defined as local to the current source module. The symbol is then only available to the current module during linking/relocation.

If you want to access (external) library modules from a library, use the  [LIB](#11_20_)  directive followed by the name of the routine. Several routine names may be specified separated by a comma.

During the linking process all external and corresponding global symbols are resolved. If two identical global identifiers are loaded by the linking process, the most recently loaded identifier is used by the linker.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_6_></a>

### 9.6. Defining symbol names

Good programming involves a structured approach to mnemonic identification of names in subroutines, variables, data structures and other constants. The  [Z80](#8_)  Module Assembler gives you several possibilities. The easiest and frequently used one is  [DEFC](#11_6_)  (Define Constant). We have supplied a complete set of header files (the "OZdefc.zip" file) containing the  [Z88](#6_6_)  operating system manifests as defined in the Developers' Notes V3 (the "devnotes.zip" file) which just contains  [DEFC](#11_6_)  directives.

Each  [DEFC](#11_6_)  directive is followed by an identifier name, followed by a = symbol and then an evaluable constant expression (usually just a constant). Constant definitions are usually operating system manifest or other frequently used items. They are put into separate source files and later inserted into main source files using the  [INCLUDE](#11_18_)  directive.

Though  [DEFC](#11_6_)  resolves most needs, it may be necessary to define variable areas or templates containing names with an appropriate size tag (byte, word or double word). This is possible using the  [DEFVARS](#11_11_)  directive. Here you may specify as many names as needed in the group. Then, it is easy to add, rearrange or delete any of the variable names - only a few modifications and then just re-compiling the necessary source files that use the templates. This would be a nightmare with  [DEFC](#11_6_) , since you have to keep track of the previous and next name in the group in addition to count the size of all names. All this is managed by  [DEFVARS](#11_11_)  automatically. Have a look at the syntax in the Directive Reference section.

With advanced  [Z80](#8_)  programming you cannot avoid dynamic data structures like linked lists or binary trees. The fundamentals for this are known as records in PASCAL or structures in C.  [DEFVARS](#11_11_)  is well suited for this purpose. Defining each  [DEFVARS](#11_11_)  group with 0 automatically generates offset variables. The last name then automatically defines the size of the data structure. Again, refer to the directive reference for a good example.

A third possibility for an easy definition of symbols is to use the  [DEFGROUP](#11_8_)  directive. Here you're able to define a set of symbols equal to an enumeration. It follows the same principles as for C's ENUM facility. The default starts at 0 and increases by 1. If you choose, a specific identifier may be set to a value, which then can set the next enumeration sequence. Again, this directive has been made to implement an easy way of defining symbols and providing a simple method to alter the identifier group. Please refer to the directive reference for an example.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_7_></a>

### 9.7. Comments in source files

As always, good programming requires good documentation. Without comments your programs lose overview and logic. Machine code is especially hard to follow - have you tried to look at a piece of code 2 years after implementation AND without any comments? HORRIBLE! There is never too many comments in machine code - we especially like to use high level language as comments - it avoids unnecessary text and logic is much more clear.

Comments in  [Z80](#8_)  source files are possible using a semicolon. When the assembler meets a semicolon the rest of the current source line is ignored until the linefeed. Parsing will then commence from the beginning of the line. The semicolon may be placed anywhere in a source line. As stated you cannot place mnemonics after the semicolon - they will be ignored. The  [Z80](#8_)  parser will in many places accept comments without a semicolon has been set - but don't rely on it. Better use a semicolon. The context is much clearer. The following is an example on how to use comments in  [Z80](#8_)  source files:

    ; **********************  
    ; main menu  
    ;  
    .mainmenu   call window   ; display menu  
                call getkey   ; read keyboard  
                ret           ; action in register A


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_8_></a>

### 9.8. Defining symbolic address labels

The main reason for using an assembler is to be able to determine symbolical addresses and use them as reference in the code. These are defined by a name preceded with a full stop, or followed by a colon. It is allowed to place a mnemonic or directive after an address label. An address label may be left as a single statement on a line - you may of course use comment after the label. The following is a label definition:

    ; *****************  
    ; routine definition  
    .mainmenu call xxx   ; a label is preceded with '.'
    endmain:  ret                ; or followed by ':'

It is not allowed to position two labels on the same line. However, you may place as many label after each other - even though no code is between them. They just receive the same assembler address.

It is not allowed to specify two identical address labels in the same source file.

If you want to declare an address globally accessible to other modules, then use  [PUBLIC](#11_26_)  for the address label definition, otherwise the label definition will be interpreted as a local address label.

     [PUBLIC](#11_26_)  mainmenu  
    ...  
    .mainmenu ; label accessible from other modules with  [EXTERN](#11_12_) 

You may use before or after the label - z80asm automatically handles the scope resolution as long as you use  [PUBLIC](#11_26_)  to define it as globally accessible.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_9_></a>

### 9.9. Writing Z80 mnemonic instructions

All  [Z80](#8_)  instructions may be written in mixed case, lower or upper case - you decide! How you separate opcode words, register names and operands is your choice. Only a few rules must be obeyed:

1.  Each instruction mnemonic must be completed on a single line.
2.  The instruction identifier must be a word, i.e. don't use space between  [CALL](#11_2_) .
3.  Register identifiers must be a word, ie. HL not H L.

A few examples which all are legal syntax:

    Ld HL   , 0       ; comment  
    ld       hl, $fFFF;comment  
    caLL ttttt


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_10_></a>

### 9.10. Optional Z80 mnemonic instruction syntax

The instructions below allow additional specification of the accumulator register. Zilog standard convention is to only specify the source operand. Sometimes it is better to specify the accumulator to make the instruction look more clear, and to avoid confusion. After all, they specified it for "add", "adc" and "sbc".

    sub a,r  
    sub a,n  
    sub a,(hl)  
    sub a,(ix+0)  
    sub a,(iy+0)

this syntax applies also to "and", "or", "xor" & "cp"


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_11_></a>

### 9.11. The undocumented Z80 instructions

We have included the parsing and code generation of the undocumented  [Z80](#8_)  instructions. They are as follows:

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

Except for the SLL instruction all have bugs related to an interrupt being able to occur while the instructions are decoded by the processor. They are implemented on the chip, but are reported to be unreliable. We have used some of them in our debugger software for the  [Z88](#6_6_) . Until now the code has been running successfully on all our  [Z88](#6_6_)  computers.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_12_></a>

### 9.12. Referencing library routines

When you need to use a library routine in your application code, you need to do two things; include a library file at the assembler command line with the  [-i](#6_3_17_)  option and refer to the library routine in your source file using the  [LIB](#11_20_)  directive followed by the name of the library routine, e.g.

     [LIB](#11_20_)  malloc, free

which will declare the two names "malloc" and "free" as external identifiers to the current source file module. Please note that you can declare the names before or after they actually are referred in your program source,. Failing to use the  [LIB](#11_20_)  directive will interpret labels as local symbols to that source file module. When the parser meets the instruction that uses one of the above names in a parameter, the parameter "expression" is automatically saved to the object file for later processing.

During the linking phase of all the object files the specified library file is automatically scanned for "malloc" and "free" and included into the application code when found.

Much application programming can be saved in "re-inventing the wheel" if you store frequently used standard routines in libraries. We have supplied a comprehensive set of library routines that were created along the development of the Assembler Workbench application EPROM. Use them as appropriate and add new ones. We are interested in your work - if you feel that you have made some good routines that others may benefit from, e-mail them to us and we will include them into the standard library.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_13_></a>

### 9.13. Creating/updating libraries

Creating libraries is an inbuilt feature of the assembler. The following steps are necessary to create a library:

1.  Define a project file containing all filenames (without extensions) in your directory that contains all library routines (the easiest method since you later can move all files to another directory). Remember to arrange the filename in a topological order, i.e. library routines that access other routines must be placed first. This optimizes the searching through a library during the linking process.  
    
2.  Each library source module uses the  [XLIB](#11_28_)  directive to define the name of the routine. The same name must be used for the address label definition. If your library uses other library routines then declare them with the  [LIB](#11_20_)  directive. Please note that a library routine creates the module name itself (performed by  [XLIB](#11_28_)  automatically). The module name is used to search for routines in a library.  
    
3.  The command line contains the  [-x](#6_3_20_)  option immediately followed by your filename. If you don't specify a filename, the default standard filename is used (defined by the  [Z80](#8_) \_STDLIB environment variable). Then you need to specify your project filename preceded by '@'.

For example:

    z80asm -xiofunctions @iofunctions

will create a library "iofunctions.lib" in the current directory (also containing all library source files). The project file is "iofunctions" also in the current directory.

Please note that no binary file is created (a library is NOT an executable file), but a collection of object files sequentially organized in a file.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=9_14_></a>

### 9.14. Referencing routines in other compiled projects

It may be necessary in some situations to get access to routines previously compiled in another project. This implies however a knowledge of their absolute addresses during linking. This information is stored in the map file, but not accessible in a form suitable to be parsed by the assembler. However, this is possible in using the  [-g](#6_3_14_)  option at the assembler command line. The action performed creates a  [DEFC](#11_6_)  list file of address labels that have been declared as globally available (using the  [XDEF](#11_27_)  directive). Only compiled source files are included in the list. If you were using the  [-a](#6_3_9_)  option (compile only updated source files) and no files were updated then the  [-g](#6_3_14_)  file would be empty. If you would like a complete list of all global routines then it is needed to compile the whole project (using the  [-b](#6_3_8_)  command line option).

When the file is generated, it can easily be  [INCLUDE](#11_18_) 'd in another project where your routines may access the external routines. You might do this in two ways:

1.  Including the file in every source module that needs to access external routines. This may be the easiest solution if you're only going to need external access in one or two source modules. With many external calls in different module of the current project it requires much altering of files.  
    
2.  Creating a new source file that is part of your project. This file could easily be the first file in your project but could just as well be placed anywhere in your project. Declare each external name that is needed somewhere in your project as  [XDEF](#11_27_) , meaning that all names to be included are globally accessible from this module. Then specify the  [INCLUDE](#11_18_)  of the  [DEFC](#11_6_)  list of the other project file. As the names get loaded, they become global definitions. All other definitions will be ignored and not stored to the object file (they are not referred in the source module). All other modules just need to specify the external names as  [XREF](#11_29_) . During linking they all get resolved and your code has access to external routines from a previously compiled project.

Whenever the previous project has been re-compiled (and issued with  [-g](#6_3_14_)  option) there is a possibility that routine addresses has changed. You therefore need to recompile the extra source module in your project to get the new identifier values - the rest of your compilation is unaffected (due to the  [XREF](#11_29_)  directives). Only the linking process gets the new proper addresses. In example 1) you had to recompile all source files that would have used an  [INCLUDE](#11_18_)  of the  [DEFC](#11_6_)  list file. In example 2) only one file had to be recompiled.

The principle of external addresses was used to compile the debugger version to be resided in segment 0 (into the upper 8K). The actual size of the debugger code uses 16K, but was split into two separate halves to fit into the upper 8K of segment 0. Each of the 8K code-segments had to get access to the other 8K block. The solution was the  [-g](#6_3_14_)  option and cross referencing using  [XREF](#11_29_)  and an additional source module (containing the  [XDEF](#11_27_)  declarations) that included the  [-g](#6_3_14_)  list file of the other project compilation.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_></a>

## 10. Using expressions

Expressions are almost unavoidable in source files. They define and explain things much clearer than just using a constant. The  [Z80](#8_)  Module Assembler allows expressions wherever a parameter is needed. This applies to  [Z80](#8_)  mnemonic instructions, directives and even in character strings. The resulting value from an evaluated expression is always an integer. All expressions are calculated internally as 32-bit signed integers. However, the parameter type defines the true range of the expression. E.g. you cannot store a 32-bit signed integer at an 8-bit LD instruction like LD A, \<n\> . If a parameter is outside an allowed integer range an assembly error is reported. Finally, no floating point operations are needed by the assembler. There is no real standard on  [Z80](#8_)  based computers.

Whenever an integer is stored in a  [Z80](#8_)  file, the standard Zilog notation is used, i.e. storing the integer in low byte, high byte order (this also applies to 32-bit integers). This standard is also known as little endian notation (also used by INTEL processors).

Expressions may be formed as arithmetic and relational expressions. Several components are supported: symbol names (identifiers), constants, ASCII characters and various arithmetic operators.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_1_></a>

### 10.1. Constant identifiers

Apart from specifying decimal integer numbers, you are allowed to use hexadecimal constants, binary constants and ASCII characters. The following symbols are put in front of the constant to identify the type:

    $ : hexadecimal constant, e.g. $4000 (16384).  
    @ : binary constant, e.g. @11000000 (192).  
    ' ' : ASCII character, e.g. 'a'.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_2_></a>

### 10.2. Arithmetic operators

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
<a id=10_3_></a>

### 10.3. Relational operators

With relational operators you may form logical expressions resulting in true or false conditions. The resulting value of a true expression is 1. The resulting value of a false expression is 0. These operators are quite handy when you need to perform complex logic for conditional assembly in  [IF](#11_15_) -ELSE-ENDIF statements. The following relational operators are available:

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
<a id=10_4_></a>

### 10.4. The ASMPC standard function

In occasional circumstances it may be necessary to use the current location of the assembler program counter in an expression e.g. calculating a relative distance. This may be done with the help of the ASMPC identifier. An example:

    .errmsg0  [DEFB](#11_3_)  errmsg1 - ASMPC - 1 , "File open error"  
    .errmsg1  [DEFB](#11_3_)  errmsg2 - ASMPC - 1 , "Syntax error"  
    .errmsg2

Here, a length byte of the following string (excluding the length byte) is calculated by using the current ASMPC address value.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=10_5_></a>

### 10.5. Symbol identifiers in expressions

Apart from using integer constants in your expressions, names are allowed as well. This is frequently used for symbolical address label references (both external and local).

Forward referencing of symbols is not really something that is important in evaluating expressions. The logic is built into the assembler parser. If an expression cannot be resolved in pass 1 of source file parsing, it will try to re-evaluate the failed expression in pass 2 of the parsing. If it still fails a symbol has not been found ( [XREF](#11_29_)  and  [LIB](#11_20_)  external symbols are handled during the linking phase).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_></a>

## 11. Directive reference

The  [Z80](#8_)  Module Assembler directives are used to manipulate the  [Z80](#8_)  assembler mnemonics and to generate data structures, variables and constants. You are even permitted to include binary files while code generation is performed.

As the name imply they direct the assembler to perform other tasks than just parsing and compiling  [Z80](#8_)  instruction mnemonics. All directives are treated as mnemonics by the assembler, i.e. it is necessary that they appear as the first command identifier on the source line (NOT necessarily the first character). Only one directive is allowed at a single source line. Even though they are written as CAPITALS in this documentation they may be written in mixed case letters in your source files.

Since the directives cover very different topics of assembler processing, each directive will be explained in detail, identified with a header description for each text section. The following syntax is used:

    <> : defines an entity, i.e. a number, character or string.  
    {} : defines a an optional repetition of an entity.  
    [] : defines an option that may be left out.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_1_></a>

### 11.1. BINARY "filename"

 [BINARY](#11_1_)  loads a binary file at the current location. This could for example be a static data structure or an executable machine code routine. The loaded binary file information is included into the object file code section. The assembler PC is updated to the end of the loaded binary code.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_2_></a>

### 11.2. CALL\_OZ \<expression\>

The \<expression\> may be a 16-bit expression and must evaluate to a constant. This is an easy interface call to the  [Z88](#6_6_)  operating system. This is an advanced RST 20H instruction which automatically allocates space for the size of the specified parameter (either 8-bit or 16-bit). Code is internally generated as follows:

    RST $20  
     [DEFB](#11_3_)  x ; 8-bit parameter

or

    RST $20  
     [DEFW](#11_4_)  x ; 16-bit parameter


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_3_></a>

### 11.3. DEFB \<8-bit expr\>,{\<8-bit expr\>} (-128; 255)

Stores a sequence of bytes (8-bits) at the current location. Expressions may be used to calculate the values.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_4_></a>

### 11.4. DEFW \<16-bit expr\>,{\<16-bit expr\>} (-32768; 65535)

Stores a sequence of words (16-bits) in low byte - high byte order (little endian) at the current location. Expressions may be used to calculate the values.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_5_></a>

### 11.5. DEFL \<32-bit expr\>,{\<32-bit expr\>} (-2147483647; 4294967295)

Stores a sequence of double-words (32-bits) in low byte - high byte order (little endian) at the current location. Expressions may be used to calculate the values.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_6_></a>

### 11.6. DEFC name=\<32-bit expression\>{, name=\<32-bit expression\>}

Define a symbol variable, that may either be a constant or an expression evaluated at link time. The allowed range is a signed 32-bit integer value. All standard  [Z88](#6_6_)  operating system header files use  [DEFC](#11_6_) 


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_7_></a>

### 11.7. DEFM \<string expression\>|\<8-bit expr\>,...

 [DEFM](#11_7_)  stores a string constant at the current location. Strings are enclosed in double quotes, e.g. "abcdefgh". Strings may be concatenated with byte constants using commas. This is useful if control characters need to be a part of the string and cannot be typed from the keyboard. Several strings and byte expressions may be concatenated, e.g.

     [DEFM](#11_7_)  "string_a" , "string_b" , 'X' , CR , LF , 0


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_8_></a>

### 11.8. DEFGROUP '{' name {',' name \['=' \<8-bit expression\>\]} '}'

 [DEFGROUP](#11_8_)  defines a group of identifier symbols with implicit values. This is similar to the enumeration principles used in C and PASCAL. The initial symbol value is 0, increased by 1 for each new symbol in the list. You may include a \<name = expression\> which breaks the linear enumeration from that constant. The  [DEFGROUP](#11_8_)  directive may be spanned across several lines and MUST be enclosed with { and }.  [DEFGROUP](#11_8_)  is just a more easy way than:  [DEFC](#11_6_)  name0 = 0, name1 = name0, ...

The following example illustrates a useful example of defining symbol values:

     [DEFGROUP](#11_8_)   
    {  
       sym_null  
       sym_ten = 10, sym_eleven, sym_twelve  
    }


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_9_></a>

### 11.9. DEFINE name,{name}

Defines a symbol identifier as logically true (integer \<\> 0). The symbol will be created as a local variable and disappears when assembly is finished on the current source file module.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_10_></a>

### 11.10. DEFS \<size\>{, \<fill-byte\>}

 [DEFS](#11_10_)  allocates a storage area of the given size with the given fill-byte. The fill-byte defaults to zero if not supplied. Both expressions need to be constants.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_11_></a>

### 11.11. DEFVARS \<16-bit expression\> '{' \[\<name\>\] \[\<storage\_size\> \<size\_multiplier\>\] '}'

 [DEFVARS](#11_11_)  defines variable address area or offsets. First you define the origin of a variable area. This may be defined using an evaluable 16-bit positive expression. Each variable name is followed by a size specifier which can be  'ds.b' (byte), 'ds.w' (word), 'ds.p' (3-byte pointer) or 'ds.l' (double-word). This is particularly useful for defining dynamic data structures in linked lists and binary search trees. Defining variable areas are only template definitions not allocations. An example:

     [DEFVARS](#11_11_)   [Z80](#8_) asm_vars  
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

     [DEFVARS](#11_11_)  0                    ; 'PfixStack' structure  
    {  
       pfixstack_const     ds.l 1    ; stack item value  
       pfixstack_previtem  ds.p 1    ; pointer to previous  
       SIZEOF_pfixstack              ; size of structure  
    }

This type of variable declaration makes it very easy for modifications, e.g. deleting or inserting variable definitions.

A special logic is available too which can be used throughout individual source files during compilation. If you specify -1 as the starting address, the last offset from the previous  [DEFVARS](#11_11_)  which was not specified as 0 will be used.

This enables you to gradually build up a list of identifier name offsets across  [DEFVARS](#11_11_)  areas in different source files. The following example explains everything:

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
<a id=11_12_></a>

### 11.12. EXTERN name {, name}

This declares symbols as external to the current module. Such a symbol must have been defined as  [PUBLIC](#11_26_)  in another module for the current module to be able to use the symbol (it will be linked during the linking phase).



----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_13_></a>

### 11.13. FPP \<8-bit expression\>

Interface call to the  [Z88](#6_6_)  operating systems' floating point library. This is easier than writing:

    RST $18  
     [DEFB](#11_3_)  mnemonic

This is an advanced RST 18H instruction which automatically allocates space for the specified parameter. All  [Z88](#6_6_)  floating point call mnemonics are defined in the "fpp.def" file.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_14_></a>

### 11.14. GLOBAL name {, name}

The  [GLOBAL](#11_14_)  directive defines a symbol  [PUBLIC](#11_26_)  if it has been defined locally or  [EXTERN](#11_12_)  otherwise.



----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_15_></a>

### 11.15. IF \<logical expression\> ... \[ELSE\] ... ENDIF

This structure evaluates the logical expression as a constant, and compiles the lines up to the ELSE clause if the expression is true (i.e. not zero), or the lines from ELSE to ENDIF if is is false (i.e. zero). The ELSE clause is optional. This structure may be nested.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_16_></a>

### 11.16. IFDEF \<name\> ... \[ELSE\] ... ENDIF

This structure checks if the give symbol name is defined, and compiles the lines up to the ELSE clause if true (i.e. defined), or the lines from ELSE to ENDIF if false (i.e. not defined). The ELSE clause is optional. This structure may be nested.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_17_></a>

### 11.17. IFNDEF \<name\> ... \[ELSE\] ... ENDIF

This structure checks if the give symbol name is not defined, and compiles the lines up to the ELSE clause if true (i.e. not defined), or the lines from ELSE to ENDIF if false (i.e. defined). The ELSE clause is optional. This structure may be nested.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_18_></a>

### 11.18. INCLUDE "filename"

Another component that is frequently used is to 'link' an additional source file together with the current source file. Usually this contains variable definitions that are commonly used by several modules in a project. This makes sense since there is no idea in copying the same information into several files - it simply uses redundant space of your storage media. This is certainly important on the  [Z88](#6_6_)  which not always has huge amounts of installed user/system RAM (usually 128K). The external source file will be included at the position of the  [INCLUDE](#11_18_)  directive.

The format of the filename depends on the operating system platform. As with the current source file, you may also insert files in include files. There is no limit of how many levels (of files) you specify of include files. Recursive or mutually recursive  [INCLUDE](#11_18_)  files (an  [INCLUDE](#11_18_)  file calling itself) is not possible - the assembler program will immediately return an error message back to you!

Include files are usually put at the start of the source file module but may be placed anywhere in the source text. The current source file will be continued after the  [INCLUDE](#11_18_)  directive when the included file has been parsed to the end of file.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_19_></a>

### 11.19. INVOKE \<16-bit expression\>

Special  [CALL](#11_2_)  instruction for the Ti83 calculator; it is coded as a RST 28H followed by the 16-bit expression, if the  [-plus](#6_3_23_)  option is passed on the command line (for the Ti83Plus), or as a normal  [CALL](#11_2_)  instruction if the option is not passed.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_20_></a>

### 11.20. LIB name {,name}

This directive is obsolete. It has been replaced by the  [EXTERN](#11_12_)  directive (See changelog.txt at the root of the z88dk project).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_21_></a>

### 11.21. LINE \<32-bit expr\> \[ , "file-name" \]

Used when the assembler is used as the back-end of a compiler to synchronize the line numbers in error messages to the lines from the compiled source. 


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_22_></a>

### 11.22. LSTOFF

Switches listing output to file off temporarily. The listing file is not closed.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_23_></a>

### 11.23. LSTON

Enables listing output (usually from a previous  [LSTOFF](#11_22_) ). Both directives may be useful when information from  [INCLUDE](#11_18_)  files is redundant in the listing file, e.g. operating system definitions.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_24_></a>

### 11.24. MODULE name

This defines the name of the current module. This may be defined only once for a module. All source file modules contain a module name. This name is used by the assembler when creating address map files and for searching routines in libraries. Further, it allows the programmer to choose a well-defined name for the source file module. The position of the module name is of no importance; it may be placed at the end or the start of the source file. However, it has more sense to put it at the top. The syntax is simple - specify a legal identifier name after the  [MODULE](#11_24_)  directive, e.g.  [MODULE](#11_24_)  main\_module


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_25_></a>

### 11.25. ORG \<16-bit expression\>

Define address origin of compiled machine code - the position in memory where the machine is to be loaded and executed. The expression must be evaluable (containing no forward or external references). All address references will be calculated from the defined  [ORG](#11_25_)  value. The  [ORG](#11_25_)  address will be placed in the current module that is being compiled. However, during linking only the first object module is being read for an  [ORG](#11_25_)  address. The  [ORG](#11_25_)  is ignored during linking if you have specified an  [-r](#6_3_12_)  option on the command line.

When assembling programs with multiple sections, a section without an  [ORG](#11_25_)  will be appended to the end of the previous section. A section with a defined  [ORG](#11_25_)  will generate its own binary file, e.g. file\_CODE.asm.

A section may contain  [ORG](#11_25_)  -1 to tell the linker to split the binary file of this section, but continue the addresses sequence from the previous section.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_26_></a>

### 11.26. PUBLIC name {, name}

This directive declares symbols publicly available for other modules during the linking phase of the compilation process.


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_27_></a>

### 11.27. XDEF name {, name}

This directive is obsolete. It has been replaced by the  [PUBLIC](#11_26_)  directive (See changelog.txt at the root of the z88dk project).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_28_></a>

### 11.28. XLIB name

This directive is obsolete. It has been replaced by the  [PUBLIC](#11_26_)  directive (See changelog.txt at the root of the z88dk project).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=11_29_></a>

### 11.29. XREF name {, name}

This directive is obsolete. It has been replaced by the  [EXTERN](#11_12_)  directive (See changelog.txt at the root of the z88dk project).


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=12_></a>

## 12. Run time error messages

The following error messages will be written toe the error files corresponding to each source file, and also to stderr. Each error message will contain the name of the source file and a line number where the error occurred in the file.

*   "File open/read error"  
    You have tried to access a file that either was not found, already opened by other resources, or the assembler wasn't able to create output files (object file, listing-, symbol-, map- or executable binary file).  
    
*   "Syntax error"  
    This is a message used by many routines - hence the general but descriptive message. You have tried to use illegal registers in  [Z80](#8_)  mnemonic instructions, specified an illegal parameter type (string instead of integer), omitted a parameter ( [DEFB](#11_3_)  without constant).  
    
*   "Symbol not defined"  
    This error is given if you are referring to an identifier (usually in an address reference) that was not declared. Either a label definition is missing in the current module or you have forgotten to declare an identifier as external using the  [XREF](#11_29_)  directive.  
    
*   "Not enough memory" / "No room in  [Z88](#6_6_) "  
    Well, well. It seems that there wasn't enough room to hold the internal data structures during parsing or linking your code. Delete any unnecessary running applications/jobs then try again. If you got the message on the  [Z88](#6_6_) , try also to delete unnecessary files from the filing system of your current RAM card.  
    
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
    The parser expected a legal identifier, i.e. a directive or  [Z80](#8_)  mnemonic. You have probably omitted the '.' in front of a label definition, misspelled a name or used a comment without a leading ';'.  
    
*   "Illegal identifier"  
    You have been trying to use a name that is either not known to the parser or an illegal identifier. This might happen if you try to use a register that is not allowed in a LD instruction, e.g. LD A,F .  
    
*   "Max. code size reached"  
    Is that really possible? Very interesting code of yours!  [Z80](#8_)  machine code program tend to be in the 32K range (at least on the  [Z88](#6_6_) )... Well, the  [Z80](#8_)  processor cannot address more than 64K. Start changing your code to a smaller size!  
    
*   "errors occurred during assembly"  
    Status error message displayed on the screen when the assembler has completed its parsing on all modules. You have one or more errors to correct in your source files before the assembler continues with linking the next time.  
    
*   "Symbol already defined"  
    In the current source file, you have tried to create two identical address label definitions, or other name identifier creators (using  [DEFC](#11_6_) ,  [DEFVARS](#11_11_) ,  [DEFGROUP](#11_8_) ).  
    
*   "Module name already defined"  
    You have used the  [MODULE](#11_24_)  directive more than once in your source file, or used both a  [MODULE](#11_24_)  and  [XLIB](#11_28_)  directive (library modules only need an  [XLIB](#11_28_) ).  
    
*   "Symbol already declared local"  
    You have tried to declare a symbol as  [XREF](#11_29_) , but it was already defined local, eg. using a ".lbaddr" in your source.  
    
*   "Illegal source filename"  
    You have tried to specify an option after having begun to specify filenames. Options must always be specified before filenames or project files.  
    
*   "Symbol declared global in another module"  
    You have two identical  [XDEF](#11_27_)  definitions in separate modules. One of them should probably be an  [XREF](#11_29_)  instead.  
    
*   "Re-declaration not allowed"  
    You are trying to specify an  [XDEF](#11_27_)  for a name that has already been  [XREF](#11_29_) 'ed in the same module (or the other way around).  
    
*   " [ORG](#11_25_)  already defined"  
    Only one  [ORG](#11_25_)  statement is allowed per section.  
    
*   "Relative jump address must be local"  
    You have tried to JR to a label address that is declared as external (with  [XREF](#11_29_)  or  [LIB](#11_20_) ). JR must be performed in the current source file module.  
    
*   "Not a relocatable file" / "Not an object file"  
    The assembler opened a supposed object file (with the proper ".obj" extension) but realised it wasn't conforming to the  [Z80](#8_)  assembler standard object file format.  
    
*   "Couldn't open library file"  
    The library file was found but couldn't be opened (probably already opened by another application resource)  
    
*   "Not a library file"  
    Your library file is not a library file (at least is not of the correct file format used by this assembler). Have you maybe used another "library" file? The  [Z80](#8_)  library file could also be corrupted (at least in the header).  
    
*   "Environment variable not defined"  
    The assembler reports either the " [Z80](#8_) \_STDLIB" environment variable wasn't found when used by options  [-x](#6_3_20_)  and  [-i](#6_3_17_) .  
    
*   "Cannot include file recursively"  
    A file was tried to be included which was already included at a previous include level.  [INCLUDE](#11_18_)  "a.h" cannot contain an  [INCLUDE](#11_18_)  "a.h".


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=13_></a>

## 13. File formats


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=13_1_></a>

### 13.1. Object file format documentation

The structure of the object file format is designed exclusively for the  [Z80](#8_)  Module Assembler. However, it may be useful for other programmers if they wish to use the file format for other purposes. We have included the documentation of the object file structure in this document. Improvement suggestions are welcome. We hope to have designed a simple and straightforward file format. The documentation may be necessary if you want to use the object file format in other systems.

I have created the design structure of the object file format to be suitable for future expansion. The primary aim of the design has been to maintain structure simplicity and load efficiency during link processing.

The figure below has deliberately been split into sections to clarify the file structure, although there is no actual physical separation between the sections in the file; each section is immediately followed by the next. 16-bit and 32-bit values are stored in low byte - high byte order (Little endian format -  [Z80](#8_) /Intel notation).

The format of the object file is as follows:

|Addr|Object|Type|Description|
|--- |--- |--- |--- |
||||**File header**|
|0|'Z80RMF01'|8 chars|File type identifier|
|8| [ORG](#11_25_)  Address|16-bit|Contains the  [ORG](#11_25_)  address for the linked machine code or 0xFFFF for no  [ORG](#11_25_)  address defined.|
|10|Module Name pointer|32-bit|Points to the Module Name section in the file.|
|14|Expression Declaration pointer|32-bit|Points to the Expression Declaration section in the file, 0xFFFFFFFF if no expressions exist.|
|18|Name Declaration pointer|32-bit|Points to the Name Declaration section in the file, 0xFFFFFFFF if no symbols exist.|
|22|Library Name Declaration pointer|32-bit|Points to the Library Name Declaration section in the file, 0xFFFFFFFF if no library symbols imported.|
|26|Machine Code Block pointer|32-bit|Points to the Machine Code Block section in the file, 0xFFFFFFFF if no code block exists.|
||||**Expression Declaration section**<br>Defines expressions that need to be computed at link time.<br>The start of the Expression Declaration section is pointed by the address at location 14 in the object file.<br>The end of the section is marked by the first defined pointer of the following list: Name Declaration pointer, Library Name Declaration pointer and Module Name pointer. The last one is always defined.|
||type|char|Type defines the resulting evaluation type range:<br>'U': 8-bit integer (0 to 255)<br>'S': 8-bit signed integer (-128 to 127)<br>'C': 16-bit integer (-32768 to 65535)<br>'L': 32-bit signed integer|
||patchptr|16-bit|Defines the code patch pointer to store result, relative to the start of the code section.|
||length|byte|Defines byte length of expression string.|
||expression string|length, chars|Contains the infix expression parsed from the source file.|
||end marker|byte|Zero byte end marker.|
||...|...|...|
||||**Name Declaration section**<br>Defines all the symbols defined in this module.<br>The start of the Name Declaration section is pointed by the address at location 18 in the object file.<br>The end of the section is marked by the first defined pointer of the following list: Library Name Declaration pointer and Module Name pointer. The last one is always defined.|
||scope|char|Scope defines the scope of the name:<br>'L' is local,<br>'G' is global,<br>'X' is global library|
||type|char|Type defines whether it is a:<br>'A' relocatable address,<br>'C' a constant|
||value|32-bit|Value of the symbol in 32 bit, relative to the start of the code block for type 'A'.|
||length|byte|Defines length byte of the following string.|
||name|length, chars|Name of the symbol.|
||...|...|...|
||||**Library Name Declaration section**<br>Defines all the library reference names.<br>The start of the Library Name Declaration section is pointed by the address at location 22 in the object file.<br>The end of the section is marked by the Module Name pointer.|
||name|length, chars|Name of the symbol.|
||...|...|...|
||||**Module Name section**<br>Defines the module name.<br>The start of the Module Name Section is pointed by the address at location 10 of the object file.|
||name|length, chars|Name of the module.|
||||**Machine Code section**<br>Contains the binary image of the compiled code.<br>The start of the Machine Code Section is pointed by the address at location 26 of the object file.|
||length|16-bit|Total length of the following machine code block, 0x0000 for a 64 KByte block. A zero-length block is not stored in the file, i.e. the Machine Code pointer in the header is set to 0xFFFFFFFF.|
||block|length bytes|Machine code block.|


----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=13_2_></a>

### 13.2. Library file format documentation

The library file format is a sequence of object files with additional structures.

|Addr|Object|Type|Description|
|--- |--- |--- |--- |
||||**File header**|
|0|'Z80LMF01'|8 chars|File type identifier|
||||**Object Modules section**|
||next object file|32-bit|File pointer to the next object file, 0xFFFFFFFF if the current Object File Block is the last in the library file.|
||object file length|32-bit|Length of the Object File. If this integer is 0 then it is indicated that the current \<Object File Block\> has been marked "deleted" and will not be used.|
||object file|length bytes|'Z80RMF01' Object File|

----

[(top)](#top) [(keywords)](#keywords) [(index)](#index)
<a id=14_></a>

## 14. Copyright

The original z80asm module assembler was written by Gunther Strube. 
It was converted from QL SuperBASIC version 0.956, initially ported to Lattice C,
and then to C68 on QDOS.

It has been maintained since 2011 by Paulo Custodio.

Copyright (C) Gunther Strube, InterLogic 1993-99  
Copyright (C) Paulo Custodio, 2011-2019


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
 [-C](#6_3_25_) 
 [-D](#6_3_15_) 
 [-I](#6_3_19_) 
 [-IXIY](#6_3_24_) 
 [-L](#6_3_18_) 
 [-M](#6_3_4_) 
 [-R](#6_3_13_) 
 [-RCMX000](#6_3_22_) 
 [-a](#6_3_9_) 
 [-b](#6_3_8_) 
 [-d](#6_3_7_) 
 [-e](#6_3_3_) 
 [-g](#6_3_14_) 
 [-h](#6_3_1_) 
 [-i](#6_3_17_) 
 [-l](#6_3_5_) 
 [-m](#6_3_11_) 
 [-o](#6_3_10_) 
 [-plus](#6_3_23_) 
 [-r](#6_3_12_) 
 [-s](#6_3_6_) 
 [-t](#6_3_21_) 
 [-v](#6_3_2_) 
 [-x](#6_3_20_) 
 [BINARY](#11_1_) 
 [CALL](#11_2_) 
 [DEFB](#11_3_) 
 [DEFC](#11_6_) 
 [DEFGROUP](#11_8_) 
 [DEFINE](#11_9_) 
 [DEFL](#11_5_) 
 [DEFM](#11_7_) 
 [DEFS](#11_10_) 
 [DEFVARS](#11_11_) 
 [DEFW](#11_4_) 
 [EXTERN](#11_12_) 
 [FPP](#11_13_) 
 [GLOBAL](#11_14_) 
 [IF](#11_15_) 
 [IFDEF](#11_16_) 
 [IFNDEF](#11_17_) 
 [INCLUDE](#11_18_) 
 [INVOKE](#11_19_) 
 [LIB](#11_20_) 
 [LINE](#11_21_) 
 [LSTOFF](#11_22_) 
 [LSTON](#11_23_) 
 [MODULE](#11_24_) 
 [ORG](#11_25_) 
 [PUBLIC](#11_26_) 
 [XDEF](#11_27_) 
 [XLIB](#11_28_) 
 [XREF](#11_29_) 
 [Z80](#8_) 
 [Z88](#6_6_) 
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
- [3.](#3_) Input Files
- [4.](#4_) Source File Format
  - [4.1.](#4_1_) Comments
  - [4.2.](#4_2_) Symbols
  - [4.3.](#4_3_) Labels
  - [4.4.](#4_4_) Numbers
  - [4.5.](#4_5_) Keywords
  - [4.6.](#4_6_) Directives and Opcodes
- [5.](#5_) Object File Format
  - [5.1.](#5_1_) Object Files
  - [5.2.](#5_2_) Library File Format
  - [5.3.](#5_3_) Format History
- [6.](#6_) z80asm Syntax
  - [6.1.](#6_1_) Help page
  - [6.2.](#6_2_) Command line
  - [6.3.](#6_3_) Command line options
    - [6.3.1.](#6_3_1_)  [-h](#6_3_1_)  : Show help screen
    - [6.3.2.](#6_3_2_)  [-v](#6_3_2_)  : Information during assembly
    - [6.3.3.](#6_3_3_)  [-e](#6_3_3_) \<ext\> : Use alternative source file extension
    - [6.3.4.](#6_3_4_)  [-M](#6_3_4_) \<ext\> : Use alternative object file extension
    - [6.3.5.](#6_3_5_)  [-l](#6_3_5_)  : Create listing file output
    - [6.3.6.](#6_3_6_)  [-s](#6_3_6_)  : Create symbol table
    - [6.3.7.](#6_3_7_)  [-d](#6_3_7_)  : Assemble only updated files
    - [6.3.8.](#6_3_8_)  [-b](#6_3_8_)  : Link/relocate object files
    - [6.3.9.](#6_3_9_)  [-a](#6_3_9_)  : Combine  [-d](#6_3_7_)  and  [-b](#6_3_8_) 
    - [6.3.10.](#6_3_10_)  [-o](#6_3_10_) \<binary-filename\> : Binary filename
    - [6.3.11.](#6_3_11_)  [-m](#6_3_11_)  : Create address map information file
    - [6.3.12.](#6_3_12_)  [-r](#6_3_12_) \<hex-address\> : Re-define the  [ORG](#11_25_)  relocation address
    - [6.3.13.](#6_3_13_)  [-R](#6_3_13_)  : Generate address independent code
    - [6.3.14.](#6_3_14_)  [-g](#6_3_14_)  : Create global address definition file
    - [6.3.15.](#6_3_15_)  [-D](#6_3_15_) \<symbol\> : Define a static symbol
    - [6.3.16.](#6_3_16_) @\<project-file\> :1.3.16. Using a project file
    - [6.3.17.](#6_3_17_)  [-i](#6_3_17_) \<library-file\> : Include library modules during linking/relocation
    - [6.3.18.](#6_3_18_)  [-L](#6_3_18_) \<library-path\> : Add directory to search path for libraries
    - [6.3.19.](#6_3_19_)  [-I](#6_3_19_) \<include-path\> : Add directory to search path for  [INCLUDE](#11_18_) 
    - [6.3.20.](#6_3_20_)  [-x](#6_3_20_)  : Create a library
    - [6.3.21.](#6_3_21_)  [-t](#6_3_21_) \<number\> : Define tabulator distance for text output files
    - [6.3.22.](#6_3_22_)  [-RCMX000](#6_3_22_)  : Support the RCM2000/RCM3000 series of  [Z80](#8_) -like CPU's
    - [6.3.23.](#6_3_23_)  [-plus](#6_3_23_)  : Support for the Ti83Plus
    - [6.3.24.](#6_3_24_)  [-IXIY](#6_3_24_)  : Swap IX and IY registers
    - [6.3.25.](#6_3_25_)  [-C](#6_3_25_)  : Enable  [LINE](#11_21_)  directive
  - [6.4.](#6_4_) The  [Z88](#6_6_)  operating system definition files
  - [6.5.](#6_5_) The supplied standard library  [Z80](#8_)  source files
  - [6.6.](#6_6_)  [Z88](#6_6_)  module assembler application source
  - [6.7.](#6_7_) File based compilation
  - [6.8.](#6_8_) Modular source file design
  - [6.9.](#6_9_) Scope of symbols in source modules
  - [6.10.](#6_10_) Using arithmetic and relational expressions
  - [6.11.](#6_11_) Source file manipulation
  - [6.12.](#6_12_) Free format of assembler source files
  - [6.13.](#6_13_) Specification of filenames
  - [6.14.](#6_14_) Including other source files into the current source file
  - [6.15.](#6_15_) Conditional assembly in source file modules
  - [6.16.](#6_16_) Undocumented  [Z80](#8_)  instruction code generation
  - [6.17.](#6_17_) Object file generation
  - [6.18.](#6_18_) Transfer of object files across platforms
  - [6.19.](#6_19_) Date stamp controlled assembly
  - [6.20.](#6_20_) Listing files
  - [6.21.](#6_21_) Symbol information
  - [6.22.](#6_22_) Linking and relocation of object modules into executable  [Z80](#8_)  machine code
  - [6.23.](#6_23_) Address map files
  - [6.24.](#6_24_) Symbol address definition files
  - [6.25.](#6_25_) Error files
  - [6.26.](#6_26_) Creating and using object file libraries for standard routines
- [7.](#7_) Executing the cross assembler and environment variables
  - [7.1.](#7_1_) The environment variables
  - [7.2.](#7_2_) Running in the QDOS/SMSQ operating system environment
  - [7.3.](#7_3_) Running in the LINUX/UNIX operating system environment
  - [7.4.](#7_4_) Running in the MS-DOS operating system environment
- [8.](#8_)  [Z80](#8_)  module assembler file types
  - [8.1.](#8_1_) The assembler file types and their extension names
  - [8.2.](#8_2_) The file name extension identifier
  - [8.3.](#8_3_) File types
    - [8.3.1.](#8_3_1_) The source file extension, asm
    - [8.3.2.](#8_3_2_) The object file extension, obj
    - [8.3.3.](#8_3_3_) The error file extension, err
    - [8.3.4.](#8_3_4_) The listing file extension, lst
    - [8.3.5.](#8_3_5_) The symbol file extension, sym
    - [8.3.6.](#8_3_6_) The executable file extension, bin
    - [8.3.7.](#8_3_7_) The address map file extension, map
    - [8.3.8.](#8_3_8_) The definition file extension, def
    - [8.3.9.](#8_3_9_) The library file extension, lib
- [9.](#9_) Compiling files
  - [9.1.](#9_1_) The assembler compiling process
    - [9.1.1.](#9_1_1_) Stage 1, parsing and code generation of all source files, object file generation
    - [9.1.2.](#9_1_2_) Stage 2, linking object files and library modules, producing executable code
  - [9.2.](#9_2_) File names
  - [9.3.](#9_3_) Portability of assembler file names
  - [9.4.](#9_4_) Source file structure
  - [9.5.](#9_5_) Using local, global and external symbols
  - [9.6.](#9_6_) Defining symbol names
  - [9.7.](#9_7_) Comments in source files
  - [9.8.](#9_8_) Defining symbolic address labels
  - [9.9.](#9_9_) Writing  [Z80](#8_)  mnemonic instructions
  - [9.10.](#9_10_) Optional  [Z80](#8_)  mnemonic instruction syntax
  - [9.11.](#9_11_) The undocumented  [Z80](#8_)  instructions
  - [9.12.](#9_12_) Referencing library routines
  - [9.13.](#9_13_) Creating/updating libraries
  - [9.14.](#9_14_) Referencing routines in other compiled projects
- [10.](#10_) Using expressions
  - [10.1.](#10_1_) Constant identifiers
  - [10.2.](#10_2_) Arithmetic operators
  - [10.3.](#10_3_) Relational operators
  - [10.4.](#10_4_) The ASMPC standard function
  - [10.5.](#10_5_) Symbol identifiers in expressions
- [11.](#11_) Directive reference
  - [11.1.](#11_1_)  [BINARY](#11_1_)  "filename"
  - [11.2.](#11_2_)  [CALL](#11_2_) \_OZ \<expression\>
  - [11.3.](#11_3_)  [DEFB](#11_3_)  \<8-bit expr\>,{\<8-bit expr\>} (-128; 255)
  - [11.4.](#11_4_)  [DEFW](#11_4_)  \<16-bit expr\>,{\<16-bit expr\>} (-32768; 65535)
  - [11.5.](#11_5_)  [DEFL](#11_5_)  \<32-bit expr\>,{\<32-bit expr\>} (-2147483647; 4294967295)
  - [11.6.](#11_6_)  [DEFC](#11_6_)  name=\<32-bit expression\>{, name=\<32-bit expression\>}
  - [11.7.](#11_7_)  [DEFM](#11_7_)  \<string expression\>|\<8-bit expr\>,...
  - [11.8.](#11_8_)  [DEFGROUP](#11_8_)  '{' name {',' name \['=' \<8-bit expression\>\]} '}'
  - [11.9.](#11_9_)  [DEFINE](#11_9_)  name,{name}
  - [11.10.](#11_10_)  [DEFS](#11_10_)  \<size\>{, \<fill-byte\>}
  - [11.11.](#11_11_)  [DEFVARS](#11_11_)  \<16-bit expression\> '{' \[\<name\>\] \[\<storage\_size\> \<size\_multiplier\>\] '}'
  - [11.12.](#11_12_)  [EXTERN](#11_12_)  name {, name}
  - [11.13.](#11_13_)  [FPP](#11_13_)  \<8-bit expression\>
  - [11.14.](#11_14_)  [GLOBAL](#11_14_)  name {, name}
  - [11.15.](#11_15_)  [IF](#11_15_)  \<logical expression\> ... \[ELSE\] ... ENDIF
  - [11.16.](#11_16_)  [IFDEF](#11_16_)  \<name\> ... \[ELSE\] ... ENDIF
  - [11.17.](#11_17_)  [IFNDEF](#11_17_)  \<name\> ... \[ELSE\] ... ENDIF
  - [11.18.](#11_18_)  [INCLUDE](#11_18_)  "filename"
  - [11.19.](#11_19_)  [INVOKE](#11_19_)  \<16-bit expression\>
  - [11.20.](#11_20_)  [LIB](#11_20_)  name {,name}
  - [11.21.](#11_21_)  [LINE](#11_21_)  \<32-bit expr\> \[ , "file-name" \]
  - [11.22.](#11_22_)  [LSTOFF](#11_22_) 
  - [11.23.](#11_23_)  [LSTON](#11_23_) 
  - [11.24.](#11_24_)  [MODULE](#11_24_)  name
  - [11.25.](#11_25_)  [ORG](#11_25_)  \<16-bit expression\>
  - [11.26.](#11_26_)  [PUBLIC](#11_26_)  name {, name}
  - [11.27.](#11_27_)  [XDEF](#11_27_)  name {, name}
  - [11.28.](#11_28_)  [XLIB](#11_28_)  name
  - [11.29.](#11_29_)  [XREF](#11_29_)  name {, name}
- [12.](#12_) Run time error messages
- [13.](#13_) File formats
  - [13.1.](#13_1_) Object file format documentation
  - [13.2.](#13_2_) Library file format documentation
- [14.](#14_) Copyright
- [15.](#15_) License
- [16.](#16_) Keywords
- [17.](#17_) Index
