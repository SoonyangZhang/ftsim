#!/usr/bin/perl

use strict;
use warnings;

while(<>) {
	my ($delay, $pkt, $sigs, $drop, $rcvd) = split /\t/;
	my $load = ($pkt*1550/2/1000*8)/(54*100000);
	my $lossrate = $drop/($drop+$rcvd);
	print "$load\t$delay\t$lossrate\n";
};
