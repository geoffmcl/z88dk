#!/usr/bin/perl

# z80asm restart
# Copyright (c) Paulo Custodio, 2020
# License: http://www.perlfoundation.org/artistic_license_2_0

use strict;
use warnings;
use File::Basename;
use Getopt::Std;

our $opt_n;		# name of variable
our $opt_u;		# unsigned
(getopts('un:') && @ARGV) 
	or die "Usage: ",basename($0)," [-u] [-n NAME] FILE...\n";

convert_file($_) for @ARGV;

sub convert_file {
	my($file) = @_;
	defined(my $c = `xxd -i $file`) or die "Tool xxd not found\n";
	# unsigned char usage_txt[] = {
	#   0x5a, 0x38, 0x30, 0x20, 0x4d, 0x6f, 0x64, 0x75, 0x6c, 0x65, 0x20, 0x41,
	# ...
	# 
	#   0x4f, 0x50, 0x0d, 0x0a
	# };
	# unsigned int usage_txt_len = 2920;

	$c =~ s/unsigned char /const unsigned char /;
	
	# replace name
	if ($opt_n) {
		$c =~ /unsigned char (\w+)/ or die;
		my $old_name = $1;
		$c =~ s/$old_name/$opt_n/g;
	}
	
	# in text mode, convert CR-LF to LF and remove unsigned
	if (!$opt_u) {
		$c =~ s/unsigned char /char /g;
		$c =~ s/0x0d,[\s\n]+0x0a,/'\\n',\n /g;	# intermeditate lines
		$c =~ s/0x0d,[\s\n]+0x0a/'\\n'/g;		# last line
		$c =~ s/0x09/'\\t'/g;
		$c =~ s/0x27/'\\''/g;
		$c =~ s/0x5c/'\\\\'/g;
		$c =~ s/(0x([0-9a-f]{2}))/ (hex($2) >= 0x20 && hex($2) < 0x7f) ? " '".chr(hex($2))."'" : $1 /ge;
	}	
	
	my $base = $opt_n // basename($file);
	open(my $out_c, ">:raw", "$base.c") or die "write $base.c: $!\n";		# write with LF only
	print $out_c $c;
	
	open(my $out_h, ">:raw", "$base.h") or die "write $base.h: $!\n";		# write with LF only
	print $out_h "#ifdef __cplusplus\n";
	print $out_h "extern \"C\" {\n";
	print $out_h "#endif\n";
	$c =~ /^(const (?:unsigned )?char \w+\[\])/m 
		and print $out_h "extern $1;\n";
	$c =~ /^(unsigned int \w+)/m 
		and print $out_h "extern $1;\n";
	print $out_h "#ifdef __cplusplus\n";
	print $out_h "};\n";
	print $out_h "#endif\n";
}
