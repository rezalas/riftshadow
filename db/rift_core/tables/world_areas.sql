--
-- Table structure for table `world_areas`
--
DROP TABLE IF EXISTS `world_areas`;

CREATE TABLE
	`world_areas` (
		`id` int(11) NOT NULL default '0',
		`name` varchar(50) NOT NULL default '',
		`credits` varchar(50) NOT NULL default '',
		`low_range` int(11) NOT NULL default '0',
		`high_range` int(11) NOT NULL default '0',
		`min_vnum` int(11) NOT NULL default '0',
		`max_vnum` int(11) NOT NULL default '0',
		`type` int(11) NOT NULL default '0',
		`climate` int(11) NOT NULL default '0',
		`flags` bigint(20) default NULL,
		KEY `id` (`id`)
	) ENGINE = MyISAM DEFAULT CHARSET = latin1;