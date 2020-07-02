#!/usr/bin/perl

# z80asm restart
# Copyright (c) Paulo Custodio, 2020
# License: http://www.perlfoundation.org/artistic_license_2_0

use strict;
use warnings;

my @lines;
my @chap_nos;
my @index;
my %keywords;

push @lines, "<a id=top></a>\n\n";
my $in_lines;
my $cur_level = 0;
while (<>) {
	$in_lines++;
	if (/^(\#+)\s*(.*?)\s*$/) {
		my $level = length($1) - 1;
		my $title = $2;
		if ($level == 0) {
			if ($in_lines > 1) { die "Level 1 only allowed at top of file"; }
		}
		elsif ($level > $cur_level) {
			add_chap_header($level, $title);
			next;
		}
		elsif ($level == $cur_level) {
			add_chap_header($level, $title);
			next;
		} 
		else { # $level < $cur_level
			add_chap_header($level, $title);
			next;
		}
	}
	push @lines, $_;
}

push @lines, "\n";
push @lines, "<a id=keywords></a>\n\n";
add_chap_header(1, "Keywords");
push @lines, join("\n", sort(keys %keywords));

push @lines, "\n";
push @lines, "<a id=index></a>\n\n";
add_chap_header(1, "Index");
push @lines, @index;

expand_keywords(@lines);

print @lines;


sub chap_list {
	my($sep, @nos) = @_;
	@nos or die;
	my $ret = "";
	for (@nos) { $ret .= $_.$sep; }
	return $ret;
}
	
sub chap_nr {
	return chap_list(".", @_);
}

sub chap_anchor {
	return chap_list("_", @_);
}

sub add_chap_header {
	my($level, $title) = @_;
	
	if ($level == 0) {
		die;
	}
	elsif ($level > $cur_level) {
		if ($cur_level + 1 != $level) { die "jump from level $cur_level to $level"; }
		$cur_level++;
		push @chap_nos, 1;
	}
	elsif ($level == $cur_level) {
		@chap_nos or die;
		$chap_nos[-1]++;
	} 
	else { # $level < $cur_level
		while ($level < $cur_level) {
			$cur_level--;
			pop @chap_nos;
		}
		@chap_nos or die;
		$chap_nos[-1]++;
	}
	
	my $chap_nr = chap_nr(@chap_nos);
	my $chap_anchor = chap_anchor(@chap_nos);

	push @lines, "\n";
	push @lines, "----\n\n";
	push @lines, "[(top)](#top) [(keywords)](#keywords) [(index)](#index)\n";
	push @lines, "<a id=$chap_anchor></a>\n\n";
	push @lines, ("#" x ($level+1))." $chap_nr $title\n";
	
	push @index, ("  " x ($level-1))."- [$chap_nr](#$chap_anchor) $title\n";
	
	if ($title =~ /^([A-Z_0-9]{2,}|-\w+)/) {
		my $keyword = $1;
		defined $keywords{$keyword} and die "duplicate keyword $keyword";
		my $link = " [$keyword](#$chap_anchor) ";
		$keywords{$keyword} = $link;
	}
}

sub expand_keywords {
	my $in_code;
	for (@_) {
		next if /^\#/;
		$in_code = !$in_code if /^```\s*$/;
		if (!$in_code) {
			s/(?:```+)?(\b[A-Z_0-9]{2,}|-\w+)(?:```+)?/ exists($keywords{$1}) ? $keywords{$1} : $1 /ge;
		}
	}
}
