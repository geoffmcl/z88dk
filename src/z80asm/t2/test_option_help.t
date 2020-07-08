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

my $config = path("../config.h")->slurp;
my($version) = $config =~ /Z88DK_VERSION\s*"(.*)"/;
ok $version, "version $version";
chomp(my $usage = path("doc/usage.txt")->slurp);

# make sure help fits in 80 columns
ok open(my $fh, "<", "doc/usage.txt"), "open doc/usage.txt";
while (<$fh>) {
	chomp;
	if (length($_) > 80) {
		ok 0, "line $. longer than 80 chars";
	}
}

for my $help (qw( -h --help )) {
	unlink $test.bin;
	run_ok("z80asm -b $help $test.asm", <<"OUT", '');
Z80 Module Assembler $version
(c) InterLogic 1993-2009, Paulo Custodio 2011-2020

$usage
OUT
	ok ! -f "$test.bin", "$test.bin not assembled";
}

for my $illegal (qw( -hx --hx -h=1 -help --helpx --HELP --help=1 )) {
	run_nok("z80asm -b $illegal $test.asm", '', <<"ERR");
Error: illegal option: $illegal
ERR
}

end_test();
