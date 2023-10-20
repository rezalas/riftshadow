--
-- Table structure for table `climate_table`
--

DROP TABLE IF EXISTS `climate_table`;
CREATE TABLE `climate_table` (
  `name` varchar(50) NOT NULL default '',
  `number` int(11) NOT NULL default '0',
  `skyfreqs` varchar(200) NOT NULL default '',
  `tempfreqs` varchar(200) NOT NULL default ''
) ENGINE=MyISAM DEFAULT CHARSET=latin1;