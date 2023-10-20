--
-- Dumping data for table `class_table`
--
LOCK TABLES `class_table` WRITE;

/*!40000 ALTER TABLE `class_table` DISABLE KEYS */;

INSERT INTO
	`class_table`
VALUES
	(
		1,
		'warrior',
		'War',
		0,
		0,
		24589,
		5,
		'class basics',
		'class default',
		0,
		1
	),
	(
		2,
		'thief',
		'Thi',
		3,
		0,
		24588,
		4,
		'class basics',
		'class default',
		0,
		1
	),
	(
		3,
		'zealot',
		'Zea',
		2,
		0,
		24589,
		3,
		'class basics',
		'class default',
		2,
		1
	),
	(
		4,
		'paladin',
		'Pal',
		0,
		4,
		24589,
		3,
		'class basics',
		'class default',
		2,
		1
	),
	(
		5,
		'anti-paladin',
		'A-P',
		0,
		6,
		24589,
		4,
		'class basics',
		'class default',
		1,
		1
	),
	(
		6,
		'ranger',
		'Ran',
		0,
		0,
		24589,
		5,
		'class basics',
		'class default',
		0,
		0
	),
	(
		7,
		'assassin',
		'Asn',
		3,
		0,
		24589,
		4,
		'class basics',
		'class default',
		1,
		1
	),
	(
		8,
		'shapeshifter',
		'Shp',
		1,
		0,
		24589,
		1,
		'class basics',
		'class default',
		1,
		0
	),
	(
		9,
		'healer',
		'Hea',
		2,
		0,
		24586,
		3,
		'class basics',
		'class default',
		2,
		1
	),
	(
		10,
		'necromancer',
		'Nec',
		1,
		6,
		24589,
		1,
		'class basics',
		'class default',
		1,
		1
	),
	(
		11,
		'sorcerer',
		'Sor',
		1,
		0,
		24588,
		1,
		'class basics',
		'class default',
		1,
		1
	),
	(
		0,
		'none',
		'non',
		0,
		0,
		0,
		0,
		'basic skills',
		'basic skills',
		0,
		0
	);

/*!40000 ALTER TABLE `class_table` ENABLE KEYS */;

UNLOCK TABLES;