#!/usr/bin/perl -w

use strict;
use Data::Dumper;
 use Test::More tests => 2;

use Util;

my ($x, $y);

$x = sub1();
print "sub1: $x\n";
$y = sub2();
print "sub2: $y\n";

ok($x eq '1', 'xxxxxxxxx');
ok($y eq '1', 'yyyyyyyyy');
print "\n------------\n\n";

#
#
