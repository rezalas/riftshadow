--
-- Table structure for table `sitecomments`
--
DROP TABLE IF EXISTS `sitecomments`;

CREATE TABLE
	`sitecomments` (
		`site_id` int(11) default NULL,
		`comment_id` int(11) NOT NULL auto_increment,
		`comment_name` varchar(20) default NULL,
		`comment_date` varchar(30) default NULL,
		`comment` text,
		PRIMARY KEY (`comment_id`)
	) ENGINE = MyISAM AUTO_INCREMENT = 4 DEFAULT CHARSET = latin1;