--
-- Table structure for table `notes`
--
DROP TABLE IF EXISTS `notes`;

CREATE TABLE
	`notes` (
		`type` tinyint(4) default NULL,
		`sender` tinytext,
		`date` tinytext,
		`to_list` tinytext,
		`subject` tinytext,
		`message` text,
		`timestamp` int(11) default NULL
	) ENGINE = MyISAM DEFAULT CHARSET = latin1;