CREATE DATABASE IF NOT EXISTS rift;
USE rift;

SELECT 'Scaffolding rift database' AS ' ';


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
select 'Building buglist table' AS ' ';
source buglist.sql
select 'Building forum table' AS ' ';
source forum.sql
select 'Building graveyard table' AS ' ';
source graveyard.sql
select 'Building helpfiles table' AS ' ';
source helpfiles.sql
select 'Building helpfiles_bak table' AS ' ';
source helpfiles_bak.sql
select 'Building inductions table' AS ' ';
source inductions.sql
select 'Building logins table' AS ' ';
source logins.sql
select 'Building notes table' AS ' ';
source notes.sql
select 'Building offerings table' AS ' ';
source offerings.sql
select 'Building pklogs table' AS ' ';
source pklogs.sql
select 'Building race table' AS ' ';
source race.sql
select 'Building sitecomments table' AS ' ';
source sitecomments.sql
select 'Building sitetracker table' AS ' ';
source sitetracker.sql
select 'Building theft table' AS ' ';
source theft.sql
select 'Building votes table' AS ' ';
source votes.sql

