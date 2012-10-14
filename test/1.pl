#!/usr/bin/perl -w

use strict;
use Data::Dumper;
 use Test::More tests => 4;

use Util;

my ($x, $y);

$x = sub1();
print "sub1: $x\n";
$y = sub2();
print "sub2: $y\n";

ok($x eq '1', 'xxxxxxxxx');
ok($y eq '1', 'yyyyyyyyy');

my $aa = [1, 2, 3];
my $hh = {1 => 10, 2 => 20};
isa_ok($aa, 'ARRAY', 'aaaaaa');
isa_ok($hh, 'HASH', 'hhhhhh');

print "\n------------\n\n";

#
#
