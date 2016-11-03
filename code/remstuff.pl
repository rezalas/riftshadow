#!/usr/bin/perl

$looking = "af.where     = TO_AFFECTS;\n";
$replace = "af.where     = TO_AFFECTS;\naf.aftype	= AFT_SPELL;\n";
$ls = `ls *.c`;
@ls = split(/\s+/,$ls);

foreach $itemfile (@ls) {
	chomp($itemfile);
	open(GDATA, "$itemfile");
	@tempdata = <GDATA>;
	close(GDATA);
	$data = join("", @tempdata);
	if ($data =~ m/\s/) {
#		$data =~ s/$looking/$replace/g;
#		$data =~ s/af.where(\s+)=(\s+)TO_AFFECTS;\n/af.where$1\=$2\TO_AFFECTS;\naf.aftype$1\=$2\AFT_SPELL;\n/g;
		$data =~ s/ +[\n\r]/\n/g;
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

