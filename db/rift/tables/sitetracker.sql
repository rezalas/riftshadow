--
-- Table structure for table `sitetracker`
--
DROP TABLE IF EXISTS `sitetracker`;

CREATE TABLE
	`sitetracker` (
		`site_id` int(11) NOT NULL auto_increment,
		`site_name` tinytext,
		`denials` int(11) default '0',
		PRIMARY KEY (`site_id`)
	) ENGINE = MyISAM AUTO_INCREMENT = 4 DEFAULT CHARSET = latin1;