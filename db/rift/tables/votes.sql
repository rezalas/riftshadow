--
-- Table structure for table `votes`
--
DROP TABLE IF EXISTS `votes`;

CREATE TABLE
	`votes` (
		`voter` varchar(50) NOT NULL default '',
		`vote_for` varchar(50) NOT NULL default '',
		`cabal` tinyint(4) NOT NULL default '0',
		`time` bigint(20) NOT NULL default '0',
		`host` varchar(50) NOT NULL default '',
		KEY `voter` (`voter`, `vote_for`)
	) ENGINE = MyISAM DEFAULT CHARSET = latin1;