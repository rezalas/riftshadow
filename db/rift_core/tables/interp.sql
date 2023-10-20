--
-- Table structure for table `interp_table`
--
DROP TABLE IF EXISTS `interp_table`;

CREATE TABLE
	`interp_table` (
		`id` int(11) NOT NULL auto_increment,
		`command_name` varchar(50) NOT NULL default '',
		`immediate_eval` int(11) NOT NULL default '0',
		`do_fun_name` varchar(50) NOT NULL default '',
		`minimum_level` int(11) NOT NULL default '0',
		`log_level` int(11) NOT NULL default '0',
		`visibility` bigint(20) default '0',
		PRIMARY KEY (`command_name`),
		KEY `id` (`id`)
	) ENGINE = MyISAM AUTO_INCREMENT = 429 DEFAULT CHARSET = latin1;