--
-- Table structure for table `class_table`
--
DROP TABLE IF EXISTS `class_table`;

CREATE TABLE
	`class_table` (
		`id` int(11) NOT NULL auto_increment,
		`name` varchar(50) NOT NULL default '',
		`who_name` char(3) NOT NULL default '',
		`attr_prime` int(4) NOT NULL default '0',
		`align` int(4) NOT NULL default '0',
		`weapon` int(8) NOT NULL default '0',
		`gainconst` int(11) NOT NULL default '0',
		`base_group` varchar(50) NOT NULL default '',
		`default_group` varchar(50) NOT NULL default '',
		`ctype` int(4) NOT NULL default '0',
		`status` int(4) NOT NULL default '0',
		PRIMARY KEY (`id`)
	) ENGINE = MyISAM AUTO_INCREMENT = 16 DEFAULT CHARSET = latin1;