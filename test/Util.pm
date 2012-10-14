package Util;

use strict;
use Data::Dumper;

use Exporter;

use vars qw(@ISA @EXPORT);
@ISA = qw(Exporter);
@EXPORT = qw(sub1 sub2);

sub sub1{
	return 1;
}

sub sub2{
	return 2;
}

1;

