#!/usr/bin/perl

$grep = `ps ux | grep startmud.pl`;

@data = split(/\s+/,$grep);

if($data[10] eq "perl") {
	system("kill $data[1]");
}
