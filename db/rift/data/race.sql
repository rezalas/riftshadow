--
-- Dumping data for table `race`
--
LOCK TABLES `race` WRITE;

/*!40000 ALTER TABLE `race` DISABLE KEYS */;

INSERT INTO
	`race`
VALUES
	('dwarf', 'affected_by', '0', '33554432'),
	('dwarf', 'vuln_flags', '0', '2048'),
	('dwarf', 'who_name', 'Dwa', ''),
	('dwarf', 'name', 'Dwarf', '');

/*!40000 ALTER TABLE `race` ENABLE KEYS */;

UNLOCK TABLES;