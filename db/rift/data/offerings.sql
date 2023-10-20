--
-- Dumping data for table `offerings`
--
LOCK TABLES `offerings` WRITE;

/*!40000 ALTER TABLE `offerings` DISABLE KEYS */;

INSERT INTO
	`offerings`
VALUES
	(
		'none',
		11803,
		'a serrated butcher\'s knife',
		'Fyndin',
		0,
		1087786772,
		309
	),
	(
		'none',
		2406,
		'a water skin',
		'Erifdel',
		0,
		1088109089,
		311
	),
	(
		'none',
		23637,
		'the hide of a savage panther',
		'Jathruk',
		0,
		1088114248,
		312
	),
	(
		'Erifdel',
		2406,
		'a water skin',
		'Erifdel',
		1,
		1088133225,
		313
	);

/*!40000 ALTER TABLE `offerings` ENABLE KEYS */;

UNLOCK TABLES;