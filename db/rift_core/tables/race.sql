--
-- Table structure for table `race_table`
--
DROP TABLE IF EXISTS `race_table`;

CREATE TABLE
	`race_table` (
		`id` int(11) NOT NULL auto_increment,
		`name` varchar(50) NOT NULL default '',
		`short_name` varchar(50) NOT NULL default '',
		`imm_name` varchar(50) NOT NULL default '',
		`race_time` varchar(50) NOT NULL default '',
		`pcrace` int(1) NOT NULL default '0',
		`race_status` int(4) NOT NULL default '0',
		`act_flags` bigint(20) default '0',
		`aff_flags` bigint(20) default '0',
		`off_flags` bigint(20) default '0',
		`imm_flags` bigint(20) default '0',
		`res_flags` bigint(20) default '0',
		`vuln_flags` bigint(20) default '0',
		`form_flags` bigint(20) default '0',
		`body_parts` bigint(20) default '0',
		`arms` int(11) NOT NULL default '0',
		`legs` int(4) NOT NULL default '0',
		`size` int(4) NOT NULL default '0',
		`age_mod` float NOT NULL default '0',
		`deathtime` int(4) NOT NULL default '0',
		`align` int(4) NOT NULL default '0',
		`xp_add` int(4) NOT NULL default '0',
		`max_str` int(11) NOT NULL default '0',
		`max_int` int(11) NOT NULL default '0',
		`max_wis` int(11) NOT NULL default '0',
		`max_dex` int(11) NOT NULL default '0',
		`max_con` int(11) NOT NULL default '0',
		PRIMARY KEY (`id`),
		KEY `id` (`id`)
	) ENGINE = MyISAM AUTO_INCREMENT = 48 DEFAULT CHARSET = latin1;