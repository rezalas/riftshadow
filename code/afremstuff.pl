#!/usr/bin/perl

$ls = `ls *.c`;
@ls = split(/\s+/,$ls);

foreach $itemfile (@ls) {
	chomp($itemfile);
	open(GDATA, "$itemfile");
	@tempdata = <GDATA>;
	close(GDATA);

	$linenumber = 0;

	foreach $line (@tempdata)
	{
		if (($line =~ m/TO_/) && ($line =~ m/(.*)\.where/))
		{
			$word = $1;
			if ($word =~ m/(\s+)(.*)/)
			{
				$space = $1;
				$word = $2;
			}
			if ($line =~ m/TO_ROOM_AFFECTS/)
			{
				print "RAF:$itemfile:$linenumber:$line";
				$tempdata[$linenumber] = "$space\init_affect_room(\&$word);\n$line";
			}
			else
			{
				print "AF:$itemfile:$linenumber:$line";
				$tempdata[$linenumber] = "$space\init_affect(\&$word);\n$line";
			}
			$counterfixed++;
		}
		$linenumber++;
		$space = "";
	}

	$data = join("",@tempdata);
	open(PDATA, ">$itemfile");
	print PDATA $data;
	close(PDATA);

	$counter++;
}
print "\n\n$counter files searched.";
print "\n$counterfixed instances fixed.\n";

