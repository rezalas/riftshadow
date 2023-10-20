--
-- Table structure for table `players`
--
DROP TABLE IF EXISTS `players`;

CREATE TABLE
	`players` (
		`name` varchar(20) default NULL,
		`lastlogin` int(11) default NULL,
		`level` int(11) default NULL,
		`class` int(11) default NULL,
		`race` int(11) default NULL,
		`cabal` int(11) default NULL,
		`sex` int(11) default NULL,
		`hours` int(11) default NULL,
		`align` int(11) default NULL,
		`ethos` int(11) default NULL,
		`logins` int(11) default NULL,
		`noc_logins` int(11) default NULL,
		`c_logins` int(11) default NULL,
		`gold` int(11) default '0',
		`pks` float NOT NULL default '0',
		`induct` tinyint(4) NOT NULL default '0'
	) ENGINE = MyISAM DEFAULT CHARSET = latin1;