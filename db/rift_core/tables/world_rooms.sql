--
-- Table structure for table `world_rooms`
--
DROP TABLE IF EXISTS `world_rooms`;

CREATE TABLE
	`world_rooms` (
		`area_id` int(11) NOT NULL default '0',
		`room_id` int(11) default NULL,
		`vnum` int(11) NOT NULL default '0',
		`name` varchar(200) NOT NULL default '',
		`description` text NOT NULL,
		`sector_type` int(11) NOT NULL default '0',
		`room_flags` bigint(20) default '0',
		`0_to_room` int(11) NOT NULL default '-1',
		`0_exit_info` bigint(20) NOT NULL default '0',
		`0_key` int(11) NOT NULL default '0',
		`0_keyword` varchar(200) NOT NULL default '',
		`1_to_room` int(11) NOT NULL default '-1',
		`1_exit_info` bigint(20) NOT NULL default '0',
		`1_key` int(11) NOT NULL default '0',
		`1_keyword` varchar(200) NOT NULL default '',
		`2_to_room` int(11) NOT NULL default '-1',
		`2_exit_info` bigint(20) NOT NULL default '0',
		`2_key` int(11) NOT NULL default '0',
		`2_keyword` varchar(200) NOT NULL default '',
		`3_to_room` int(11) NOT NULL default '-1',
		`3_exit_info` bigint(20) NOT NULL default '0',
		`3_key` int(11) NOT NULL default '0',
		`3_keyword` varchar(200) NOT NULL default '',
		`4_to_room` int(11) NOT NULL default '-1',
		`4_exit_info` bigint(20) NOT NULL default '0',
		`4_key` int(11) NOT NULL default '0',
		`4_keyword` varchar(200) NOT NULL default '',
		`5_to_room` int(11) NOT NULL default '-1',
		`5_exit_info` bigint(20) NOT NULL default '0',
		`5_key` int(11) NOT NULL default '0',
		`5_keyword` varchar(200) NOT NULL default '',
		`cabal` int(11) NOT NULL default '0',
		`owner` varchar(50) NOT NULL default '0',
		KEY `area_id` (`area_id`)
	) ENGINE = MyISAM DEFAULT CHARSET = latin1;