#!/usr/local/bin/perl

$File = "logins.txt";
$MaxShow = 20;
open(GDATA, "$File");
@Logins = <GDATA>;
close(GDATA);

foreach $log (@Logins) {
	if ($log =~ m/(\w+) has quit. \((\d+) minutes played\)/) {
		push(@NewLogins,"$2 $1");
	}
}

@NewLogins = sort {$b <=> $a} @NewLogins;

$count = 1;

foreach $newlog (@NewLogins) {
	($minutes,$name) = split(/ /,$newlog);
	if ($minutes == $prevmin) {
		$scount = $count - 1;
	} else {
		$scount = $count;
	}
	print "$scount. $name: $minutes\n";
	$count++;
	if ($count > $MaxShow) {
		last;
	}
	$prevmin = $minutes;
}
