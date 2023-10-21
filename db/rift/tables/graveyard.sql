--
-- Table structure for table `graveyard`
--
DROP TABLE IF EXISTS `graveyard`;

CREATE TABLE
	`graveyard` (
		`ID` int(11) NOT NULL auto_increment,
		`Pname` varchar(50) NOT NULL default '',
		`Pfrags` float NOT NULL default '0',
		`Pfgood` float NOT NULL default '0',
		`Pfneutral` float NOT NULL default '0',
		`Pfevil` float NOT NULL default '0',
		`Pfdeaths` float NOT NULL default '0',
		`Pmdeaths` int(11) NOT NULL default '0',
		`Phours` int(11) NOT NULL default '0',
		PRIMARY KEY (`ID`),
		KEY `Pname` (`Pname`)
	) ENGINE = MyISAM AUTO_INCREMENT = 165 DEFAULT CHARSET = latin1;