CREATE DATABASE IF NOT EXISTS rift_core;
USE rift_core;

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

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