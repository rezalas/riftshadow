--
-- Table structure for table `inductions`
--
DROP TABLE IF EXISTS `inductions`;

CREATE TABLE
	`inductions` (
		`ID` int(11) NOT NULL default '0',
		`ch` varchar(50) NOT NULL default '',
		`victim` varchar(50) NOT NULL default '',
		`cabal` int(11) NOT NULL default '0',
		`ctime` bigint(20) NOT NULL default '0',
		`chsite` varchar(50) NOT NULL default '',
		`victimsite` varchar(50) NOT NULL default '',
		PRIMARY KEY (`ID`),
		KEY `ID` (`ID`, `cabal`, `ctime`)
	) ENGINE = MyISAM DEFAULT CHARSET = latin1;