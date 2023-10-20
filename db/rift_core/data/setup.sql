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

SELECT 'rift_core database complete!' AS ' ';