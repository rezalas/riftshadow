#!/usr/bin/perl

# Original location of script: /area/


#$looking = 'affect_to_area\s*\(\s*([a-zA-Z->_0-9\(\)]*)\s*,\s*&([a-zA-Z->_0-9\(\)]*)\s*\);';
#$looking = '->class\s*=\s*(.*);';
#$looking = '->perm_stat\[([a-zA-Z->_0-9\[\]]*)\]';
#$looking = 'skill_table\[([a-zA-Z->_0-9\[\]]*)\]\.';
#$looking = '[^n]*\s*send_to_char\s*\((.*),\s*([a-zA-Z->_0-9\(\)]*)\s*\);';
#$looking = 'add_act_to_queue\((.*),\(char\*\)\"(.*)\",(.*),(.*),(.*),(.*)\);';
$looking = '\nTEACHES swimming~';
$ls = `ls *.are`;
@ls = split(/\s+/,$ls);

foreach $itemfile (@ls) {
        chomp($itemfile);
        open(GDATA, "$itemfile");
        @tempdata = <GDATA>;
        close(GDATA);
        $data = join("", @tempdata);
		if ($data =~ m/$looking/) {
                $data =~ s/$looking/\n/g;
                open(PDATA, ">$itemfile");
                print PDATA $data;
                close(PDATA);
                print "$itemfile fixed.\n";
				#print "I would like to replace $looking with $1.\n";
                $counterfixed++;
        }
        $counter++;
}
print "\n\n$counter files searched.";
print "\n$counterfixed files fixed.\n";


