--
-- Table structure for table `helpfiles_bak`
--
DROP TABLE IF EXISTS `helpfiles_bak`;

CREATE TABLE
	`helpfiles_bak` (
		`id` int(11) NOT NULL auto_increment,
		`title` tinytext,
		`skill` tinytext,
		`minlevel` tinyint(3) unsigned default '0',
		`helpdata` text,
		`author` varchar(50) default NULL,
		PRIMARY KEY (`id`)
	) ENGINE = MyISAM AUTO_INCREMENT = 673 DEFAULT CHARSET = latin1;