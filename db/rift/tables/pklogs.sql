--
-- Table structure for table `pklogs`
--
DROP TABLE IF EXISTS `pklogs`;

CREATE TABLE
	`pklogs` (
		`killer` tinytext,
		`killercabal` tinyint(4) NOT NULL default '0',
		`victim` tinytext,
		`victimcabal` tinyint(4) NOT NULL default '0',
		`date` tinytext,
		`room` tinytext,
		`ctime` bigint(20) default '1'
	) ENGINE = MyISAM DEFAULT CHARSET = latin1;