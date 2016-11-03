#!/usr/bin/perl
use English;

$timekill = 180;
$date = `date`;
$logfile = "/home/rift/bootuplog.txt";

$file = "/home/rift/area/upfile.txt";
$time = time;
open(GDATA,$file);
@data = <GDATA>;
close(GDATA);
$uptime = join("",@data);
$seconds = $time - $uptime;

if ($seconds > $timekill) {
	die unless defined($pid=open(KID,"-|"));
	if ($pid) {
		@data=<KID>;
		close KID;
		if ($data[1]) {
			($null,$pid,$null2) = split(/ /,$data[1]);
			if ($pid eq "?") {
				$pid = $null;
			}
			system("kill -s 9 $pid");
			open(LOG,">>$logfile");
			print LOG "Kill: $date";
			close(LOG);
		}
	} else {
		$EUID=$UID;
		$EGID=$GID;
		$ENV{PATH}="/bin:/usr/bin";
		exec("/bin/ps -C rift");
		die "Can't execute ($!)!";
	}
}
