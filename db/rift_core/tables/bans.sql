--
-- Table structure for table `bans`
--
DROP TABLE IF EXISTS `bans`;

CREATE TABLE
	`bans` (
		`site` varchar(254) NOT NULL default '',
		`by` varchar(50) NOT NULL default '',
		`reason` varchar(100) NOT NULL default '',
		`date` datetime default NULL,
		`duration` bigint(20) NOT NULL default '0',
		`ban_type` int(11) NOT NULL default '0',
		`host_type` int(11) NOT NULL default '0'
	) ENGINE = MyISAM DEFAULT CHARSET = latin1;