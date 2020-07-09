#!/usr/bin/perl

#------------------------------------------------------------------------------
# z80asm tests
# Copyright (C) Paulo Custodio, 2011-2020
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
#
# BUG_0049: Making a library with -d and 512 (win32) object files fails
# - Too many open files
# limits very per OS:
# 509 files - when compiled with Visual Studio on Win32
# 3197 files - when compiled with gcc on Cygwin on Win32
# 2045 files - when compiled with gcc on Linux Subsystem for Windows
# 1021 files - when compiled with gcc on Ubuntu
#------------------------------------------------------------------------------

use Modern::Perl;
use Config;
BEGIN { use lib 't2'; use testlib; }
my $test = test_name();

my $cmd;
my @ids = (1..4096);

# build asm files
unlink <${test}*.o ${test}*.bin ${test}*.err>;
open(my $lst, ">", "${test}.lst") or die;
for my $n (@ids) {
	my $id = sprintf("%04d", $n);
	open(my $fh, ">", "$test$id.asm") or die;
	print $fh <<END; 
 public lbl$id
 defw $n
 defc lbl$id = $n
END
	close $fh;
	print $lst "${test}$id\n";
}
close($lst);

# assemble
unlink "${test}0001.bin";
$cmd = "z80asm -b \@${test}.lst";
ok 0==system($cmd), $cmd;

# check binary
ok -f "${test}0001.bin";
ok path("${test}0001.bin")->slurp_raw eq pack("v*", @ids);

# link only
unlink "${test}0001.bin", <${test}*.asm>;
$cmd = "z80asm -b \@${test}.lst";
ok 0==system($cmd), $cmd;

# check binary
ok -f "${test}0001.bin";
ok path("${test}0001.bin")->slurp_raw eq pack("v*", @ids);

# make library
unlink "${test}.lib";
$cmd = "z80asm -b -x${test} \@${test}.lst";
ok 0==system($cmd), $cmd;
ok -f "${test}.lib";

# use library
unlink "${test}0001.bin";
path("${test}0001.asm")->spew(<<'END');
 extern lbl4096;
 defw lbl4096;
END
$cmd = "z80asm -b -i${test} ${test}0001.asm";
ok 0==system($cmd), $cmd;
ok -f "${test}0001.bin";
ok path("${test}0001.bin")->slurp_raw eq pack("vv", 4096, 4096);

# delete test files
unlink <${test}*.asm ${test}*.o ${test}*.bin ${test}*.err ${test}*.lib ${test}*.c ${test}$Config{_exe}>;

end_test();
