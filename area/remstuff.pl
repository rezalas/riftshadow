#!/usr/bin/perl

$looking = "Aetullan";
$replace = "Melcene";
$ls = `ls *.are`;
@ls = split(/\s+/,$ls);

foreach $itemfile (@ls) {
	chomp($itemfile);
	open(GDATA, "$itemfile");
	@tempdata = <GDATA>;
	close(GDATA);
	$data = join("", @tempdata);
	if ($data =~ m/$looking/) {
		$data =~ s/$looking/$replace/g;
		open(PDATA, ">$itemfile");
		print PDATA $data;
		close(PDATA);
		print "$itemfile fixed.\n";
		$counterfixed++;
	}
	$counter++;
}
print "\n\n$counter files searched.";
print "\n$counterfixed files fixed.\n";

