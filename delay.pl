#!/usr/bin/perl
# Get the simulator output from stdin and calculate the average packet delay and packet loss rate

use strict;
use warnings;

my %arrival;	# Associative array to calculate packet delay
my $pkts=0;
my $drop=0;
my $sigs=0;
my $totaldelay=0;
my $totalrcvd=0;
my $bytes=0;

# Read from stdin
while(<>) {
	next unless /^. \d+\.\d+ /;
	if (/^([rd\+]) (\d+\.\d+) .* (Packet|CongestSignal) ([0-9a-f]+) .*?( Size=(\d+) .*)?$/) {
		if ("$1" eq "r" && exists($arrival{"$4"})) {
			$totaldelay += $2 - $arrival{"$4"};
			$totalrcvd++;
			$sigs++ if ("$3" eq "CongestSignal");
			delete $arrival{"$4"};
		} elsif ("$1" eq "d" && exists($arrival{"$4"})) {
			delete $arrival{"$4"};
			$drop++;
		} elsif ("$1" eq "+" && !exists($arrival{"$4"})) {
			$arrival{"$4"} = $2;
			$pkts++ if ("$3" eq "Packet");
			$bytes += $6 if defined($6);
		};
	};
};

# Print the summary
print "".($totaldelay/$totalrcvd)."\t".($drop/($drop+$totalrcvd))."\t";			# Average delay
print "$pkts\t$sigs\t$drop\t$totalrcvd\t$bytes\n";	# Packet counts

my @array;
