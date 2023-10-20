--
-- Table structure for table `buglist`
--
DROP TABLE IF EXISTS `buglist`;

CREATE TABLE
	`buglist` (
		`ID` mediumint(8) unsigned NOT NULL auto_increment,
		`AddedBy` varchar(20) default NULL,
		`Date` tinytext,
		`Summary` text,
		`Description` text,
		`Status` tinyint(3) unsigned default NULL,
		PRIMARY KEY (`ID`)
	) ENGINE = MyISAM AUTO_INCREMENT = 13 DEFAULT CHARSET = latin1;