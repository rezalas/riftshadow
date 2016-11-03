#!/usr/bin/perl

$grep = `ps ux | grep -E '([0-9]) perl startmud.pl'`;

@data = split(/\s+/,$grep);

if($data[10] ne "perl") {
	exec("cd ~/area && perl startmud.pl");
}
