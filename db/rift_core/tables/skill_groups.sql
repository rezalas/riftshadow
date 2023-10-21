--
-- Table structure for table `skill_groups`
--
DROP TABLE IF EXISTS `skill_groups`;

CREATE TABLE
	`skill_groups` (
		`name` varchar(50) NOT NULL default '',
		`skill` varchar(50) NOT NULL default ''
	) ENGINE = MyISAM DEFAULT CHARSET = latin1;