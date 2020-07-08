#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#------------------------------------------------------------------------------

use Modern::Perl;
BEGIN { use lib 't2'; use testlib; }

my $test = test_name();

path("$test.asm")->spew(<<END);
	nop
END

unlink $test.bin;
run_ok("z80asm -b $test.asm", '', '');
my $bin = path("$test.bin")->slurp_raw();
ok $bin eq  "\x00", "bin ok";

for my $verbose (qw( -v --verbose )) {
	unlink $test.bin;
	run_ok("z80asm -b $verbose $test.asm", <<"OUT", '');
Reading library 'z80asm-z80-.lib'
Predefined constant: __CPU_Z80__ = \$0001
Predefined constant: __CPU_ZILOG__ = \$0001
Assembling '$test.asm' to '$test.o'
Reading '$test.asm' = '$test.asm'
Writing object file '$test.o'
Module '$test' size: 1 bytes

Code size: 1 bytes (\$0000 to \$0000)
Creating binary '$test.bin'
OUT
	my $bin = path("$test.bin")->slurp_raw();
	ok $bin eq  "\x00", "bin ok";
}

for my $illegal (qw( -vx --vx -v=1 -verbose --verbosex --VERBOSE --verbose=1 )) {
	run_nok("z80asm -b $illegal $test.asm", '', <<"ERR");
Error: illegal option: $illegal
ERR
}

end_test();
