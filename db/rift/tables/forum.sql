--
-- Table structure for table `forum`
--
DROP TABLE IF EXISTS `forum`;

CREATE TABLE
	`forum` (
		`number` int(11) NOT NULL auto_increment,
		`name` tinytext,
		`email` tinytext,
		`date` tinytext,
		`subject` tinytext,
		`body` longtext,
		`host` int(11) NOT NULL default '0',
		`state` int(11) NOT NULL default '0',
		PRIMARY KEY (`number`)
	) ENGINE = MyISAM DEFAULT CHARSET = latin1;