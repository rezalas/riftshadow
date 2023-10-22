---
--- Set the database name
---
use rift_core;

---
--- Import data
---

SELECT 'Importing bans data' AS ' ';
source bans.sql

SELECT 'Importing bit_lookup data' AS ' ';
source bit_lookup.sql

SELECT 'Importing class data' AS ' ';
source class.sql

SELECT 'Importing climate data' AS ' ';
source climate.sql

SELECT 'Importing d_lookup data' AS ' ';
source d_lookup.sql

SELECT 'Importing interp data' AS ' ';
source interp.sql

SELECT 'Importing players data' AS ' ';
source players.sql

SELECT 'Importing race data' AS ' ';
source race.sql

SELECT 'Importing skill_groups data' AS ' ';
source skill_groups.sql

SELECT 'Importing theft data' AS ' ';
source theft.sql

SELECT 'Importing world_areas data' AS ' ';
source world_areas.sql

SELECT 'Importing world_rooms_rid data' AS ' ';
source world_rooms_rid.sql

SELECT 'Importing world_rooms data' AS ' ';
source world_rooms.sql

/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

SELECT 'rift_core database complete!' AS ' ';