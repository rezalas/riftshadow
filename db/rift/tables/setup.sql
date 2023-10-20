CREATE DATABASE IF NOT EXISTS rift;
USE rift;

SELECT 'Scaffolding rift database' AS ' ';
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

