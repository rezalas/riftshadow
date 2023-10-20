--
-- Dumping data for table `d_lookup`
--
LOCK TABLES `d_lookup` WRITE;

/*!40000 ALTER TABLE `d_lookup` DISABLE KEYS */;

INSERT INTO
	`d_lookup`
VALUES
	('strength', 'str', 'STAT_STR', 'stats', 0),
	('intelligence', 'int', 'STAT_INT', 'stats', 1),
	('wisdom', 'wis', 'STAT_WIS', 'stats', 2),
	('dexterity', 'dex', 'STAT_DEX', 'stats', 3),
	('constitution', 'con', 'STAT_CON', 'stats', 4),
	(
		'casts',
		'casts',
		'CLASS_CASTER',
		'magical_class',
		1
	),
	(
		'communes',
		'communes',
		'CLASS_COMMUNER',
		'magical_class',
		2
	),
	(
		'none',
		'none',
		'CLASS_NEITHER',
		'magical_class',
		0
	),
	('open', 'open', 'RACE_OPEN', 'race_status', 1),
	(
		'closed',
		'closed',
		'RACE_CLOSED',
		'race_status',
		0
	),
	('any', 'any', 'ALIGN_ANY', 'align_allowed', 0),
	(
		'good neutral',
		'good neutral',
		'ALIGN_GN',
		'align_allowed',
		1
	),
	(
		'neutral evil',
		'neutral evil',
		'ALIGN_NE',
		'align_allowed',
		2
	),
	(
		'good evil',
		'good evil',
		'ALIGN_GE',
		'align_allowed',
		3
	),
	('good', 'good', 'ALIGN_G', 'align_allowed', 4),
	(
		'neutral',
		'neutral',
		'ALIGN_N',
		'align_allowed',
		5
	),
	('evil', 'evil', 'ALIGN_E', 'align_allowed', 6),
	('tiny', 'tiny', 'SIZE_TINY', 'size', 0),
	('small', 'small', 'SIZE_SMALL', 'size', 1),
	('medium', 'medium', 'SIZE_MEDIUM', 'size', 2),
	('large', 'large', 'SIZE_LARGE', 'size', 3),
	('huge', 'huge', 'SIZE_HUGE', 'size', 4),
	('giant', 'giant', 'SIZE_GIANT', 'size', 5),
	('immense', 'immense', 'SIZE_IMMENSE', 'size', 6),
	('open', 'open', 'CLASS_OPEN', 'class_status', 1),
	(
		'closed',
		'closed',
		'CLASS_CLOSED',
		'class_status',
		0
	),
	('normal', 'normal', 'ARE_NORMAL', 'area_type', 0),
	(
		'road_river',
		'road_river',
		'ARE_ROAD_RIVER',
		'area_type',
		1
	),
	('cabal', 'cabal', 'ARE_CABAL', 'area_type', 2),
	('quest', 'quest', 'ARE_QUEST', 'area_type', 3),
	('city', 'city', 'ARE_CITY', 'area_type', 4),
	(
		'unopened',
		'unopened',
		'ARE_UNOPENED',
		'area_type',
		5
	),
	('shrine', 'shrine', 'ARE_SHRINE', 'area_type', 6),
	('none', 'none', 'CLIMATE_NONE', 'climate_type', 0),
	(
		'temperate',
		'temperate',
		'CLIMATE_TEMPERATE',
		'climate_type',
		1
	),
	(
		'desert',
		'desert',
		'CLIMATE_DESERT',
		'climate_type',
		2
	),
	(
		'tropical',
		'tropical',
		'CLIMATE_TROPICAL',
		'climate_type',
		3
	),
	(
		'alpine',
		'alpine',
		'CLIMATE_ALPINE',
		'climate_type',
		4
	),
	(
		'tundra',
		'tundra',
		'CLIMATE_TUNDRA',
		'climate_type',
		5
	),
	(
		'subtropical',
		'subtropical',
		'CLIMATE_SUBTROPICAL',
		'climate_type',
		6
	),
	('arid', 'arid', 'CLIMATE_ARID', 'climate_type', 7),
	(
		'north',
		'n',
		'Directions::North',
		'directions',
		0
	),
	('east', 'e', 'Directions::East', 'directions', 1),
	(
		'south',
		's',
		'Directions::South',
		'directions',
		2
	),
	('west', 'w', 'Directions::West', 'directions', 3),
	('up', 'u', 'Directions::Up', 'directions', 4),
	('down', 'd', 'Directions::Down', 'directions', 5);

/*!40000 ALTER TABLE `d_lookup` ENABLE KEYS */;

UNLOCK TABLES;