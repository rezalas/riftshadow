--
-- Table structure for table `world_rooms_rid`
--
DROP TABLE IF EXISTS `world_rooms_rid`;

CREATE TABLE
	`world_rooms_rid` (
		`room_id` int(11) NOT NULL default '0',
		`vnum` int(11) NOT NULL default '0'
	) ENGINE = MyISAM DEFAULT CHARSET = latin1;