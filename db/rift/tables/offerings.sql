--
-- Table structure for table `offerings`
--
DROP TABLE IF EXISTS `offerings`;

CREATE TABLE
	`offerings` (
		`deity` text,
		`offeringvnum` int(11) default NULL,
		`offeringshort` text,
		`player` text,
		`status` tinyint(4) default NULL,
		`time` int(11) default NULL,
		`id` int(11) NOT NULL auto_increment,
		PRIMARY KEY (`id`)
	) ENGINE = MyISAM AUTO_INCREMENT = 314 DEFAULT CHARSET = latin1;