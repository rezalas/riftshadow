#!/usr/bin/perl

print "File name: ";
chomp($file = <STDIN>);

print "Prefix you want to change from (e.g. 26 if 2600-2699): ";
chomp($prefixfrom = <STDIN>);

print "Prefix you want to change to (e.g. 36 if 3600-3699): ";
chomp($prefixto = <STDIN>);

open(GDATA, "$file");
@tempdata = <GDATA>;
close(GDATA);

$linenumber = 0;

foreach $line (@tempdata)
{
	if ($line =~ m/^$prefixfrom(\d+) $prefixfrom(\d+)\n/m)
	{
		$tempdata[$linenumber] = "$prefixto$1 $prefixto$2\n";
	}
	elsif ($line =~ m/^\#$prefixfrom(\d+)\n/m)
	{
		$tempdata[$linenumber] = "\#$prefixto$1\n";
	}
	elsif ($line =~ m/^(.*) $prefixfrom(\d+) (\d+) ([\w-]+)\~\n/m)
	{
		$tempdata[$linenumber] = "$1 $prefixto$2 $3 $4\~\n";
	}
	$linenumber++;
}

$data = join("",@tempdata);

if ($data =~ m/(.*?)#RESETS(.*?)\#\$/s)
{
	$top = $1;
	$bottom = $2;
	$top =~ s/^D (\w+)\s+$prefixfrom(\d+)([\s\n])(.*) $prefixfrom(\d+)\n/D $1 $prefixto$2$3$4 $prefixto$5\n/gm;
	$top =~ s/^D (\w+)\s+$prefixfrom(\d+)([\s\n])(.*) ([\w-]+)\n/D $1 $prefixto$2$3$4 $5\n/gm;
	$bottom =~ s/ $prefixfrom(\d+)([\s\n])/ $prefixto$1$2/g;
	$data = "$top\#RESETS$bottom\#\$\n";
}
open(PDATA, ">$file.new");
print PDATA $data;
close(PDATA);

print "\nGo UberMapper.\n";
