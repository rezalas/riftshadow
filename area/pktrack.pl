#!/usr/bin/perl

$file = "tempdeaths.txt";
$tempfile = $ARGV[0];
$display = $ARGV[1];
$who = $ARGV[2];

open(GDATA,"<$file");
@pkData = <GDATA>;
close(GDATA);

@pkData = reverse @pkData;

open(PDATA,">$tempfile");
$number = 1;
foreach $entry (@pkData) {
	($dead,$killer,$where,$datetime) = split(/ \| /,$entry);
	if (($display eq "pks") && !($killer eq $who)) {
		next;
	} elsif (($display eq "deaths") && !($dead eq $who)) {
		next;
	} else {
		if (!($dead eq $who) && !($killer eq $who)) {
			next;
		}
	}
	($day,$month,$date,$time,$year) = split(/\s+/,$datetime);
	printf PDATA "%+3s\) %s killed by %s (%s %s %s)\n\r",
		$number,$dead,$killer,$month,$date,$time;

	$number++;
}
if ($number == 1) {
	print PDATA " ";
}
close(PDATA);
