--
-- Table structure for table `race`
--
DROP TABLE IF EXISTS `race`;

CREATE TABLE
	`race` (
		`name` varchar(50) NOT NULL default '',
		`type` varchar(50) NOT NULL default '',
		`value0` varchar(50) NOT NULL default '0',
		`value1` varchar(50) NOT NULL default '0'
	) ENGINE = MyISAM DEFAULT CHARSET = latin1;