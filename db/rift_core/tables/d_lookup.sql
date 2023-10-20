--
-- Table structure for table `d_lookup`
--
DROP TABLE IF EXISTS `d_lookup`;

CREATE TABLE
	`d_lookup` (
		`name` varchar(50) NOT NULL default '',
		`abbrev` varchar(50) NOT NULL default '',
		`define` varchar(50) NOT NULL default '',
		`category` varchar(50) NOT NULL default '',
		`value` int(9) NOT NULL default '0'
	) ENGINE = MyISAM DEFAULT CHARSET = latin1;