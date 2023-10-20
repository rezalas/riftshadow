--
-- Table structure for table `logins`
--
DROP TABLE IF EXISTS `logins`;

CREATE TABLE
	`logins` (
		`name` varchar(50) NOT NULL default '',
		`site` varchar(75) NOT NULL default '',
		`time` varchar(50) NOT NULL default '',
		`ctime` bigint(20) default NULL,
		`played` int(11) default NULL,
		`obj` int(11) default NULL,
		`lobj` int(11) default NULL,
		`type` int(11) default NULL,
		KEY `name` (`name`),
		KEY `site` (`site`),
		KEY `time` (`time`)
	) ENGINE = MyISAM DEFAULT CHARSET = latin1;