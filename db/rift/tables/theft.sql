--
-- Table structure for table `theft`
--
DROP TABLE IF EXISTS `theft`;

CREATE TABLE
	`theft` (
		`ID` int(11) NOT NULL auto_increment,
		`ch` varchar(50) NOT NULL default '',
		`victim` varchar(50) NOT NULL default '',
		`obj` varchar(50) NOT NULL default '',
		`success` tinyint(4) NOT NULL default '0',
		PRIMARY KEY (`ID`),
		KEY `ID` (`ID`),
		KEY `ch` (`ch`),
		KEY `victim` (`victim`),
		KEY `obj` (`obj`)
	) ENGINE = MyISAM AUTO_INCREMENT = 866 DEFAULT CHARSET = latin1;