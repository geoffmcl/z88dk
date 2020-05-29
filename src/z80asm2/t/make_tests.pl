#!/usr/bin/perl

# z80asm restart
# Copyright (c) Paulo Custodio, 2020
# License: http://www.perlfoundation.org/artistic_license_2_0

use strict;
use warnings;

open(my $f, ">:raw", "t/test.def") or die $!;

@ARGV = <t/t_*.c>;
exit 0 unless @ARGV;

while (<>) {
    if (/^ \s* void \s+ (test_\w+) \s* \( \s* void \s* \) /x) {
    print $f <<"END";
TEST_CALL($1);
END
    }
    elsif (/^ \s* int \s+ (exec_\w+) \s* \( \s* void \s* \) /x) {
    print $f <<"END";
TEST_EXEC($1);
END
    }
}

print $f <<"END";
#undef TEST_CALL
#undef TEST_EXEC
END
