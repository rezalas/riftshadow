--
-- Table structure for table `bit_lookup`
--

DROP TABLE IF EXISTS `bit_lookup`;
CREATE TABLE `bit_lookup` (
	`name` varchar(50) NOT NULL default '',
	`bit` bigint(50) default '0',
	`define` varchar(50) NOT NULL default '',
	`type` varchar(50) NOT NULL default '',
	`num_bits` int(11) NOT NULL default '0'
) ENGINE = MyISAM DEFAULT CHARSET = latin1;