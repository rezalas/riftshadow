CREATE DATABASE IF NOT EXISTS rift_core;
USE rift_core;

---
--- Scaffold tables
---
-- select 'Building bans table' AS ' ';
source bans.sql

-- select 'Building bit_lookup table' AS ' ';
source bit_lookup.sql

-- select 'Building class table' AS ' ';
source class.sql

-- select 'Building climate table' AS ' ';
source climate.sql

-- select 'Building d_lookup table' AS ' ';
source d_lookup.sql

-- select 'Building interp table' AS ' ';
source interp.sql

-- select 'Building players table' AS ' ';
source players.sql

-- select 'Building race table' AS ' ';
source race.sql

-- select 'Building skill_groups table' AS ' ';
source skill_groups.sql

-- select 'Building theft table' AS ' ';
source theft.sql

-- select 'Building world_areas table' AS ' ';
source world_areas.sql

-- select 'Building world_rooms_rid table' AS ' ';
source world_rooms_rid.sql

-- select 'Building world_rooms table' AS ' ';
source world_rooms.sql