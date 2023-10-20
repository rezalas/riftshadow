---
--- Import data
---
SELECT 'Importing bans data' AS ' ';
source buglist.sql
SELECT 'Importing forum data' AS ' ';
source forum.sql
SELECT 'Importing graveyard data' AS ' ';
source graveyard.sql
SELECT 'Importing helpfiles data' AS ' ';
source helpfiles.sql
SELECT 'Importing helpfiles_bak data' AS ' ';
source helpfiles_bak.sql
SELECT 'Importing inductions data' AS ' ';
source inductions.sql
SELECT 'Importing logins data' AS ' ';
source logins.sql
SELECT 'Importing notes data' AS ' ';
source notes.sql
SELECT 'Importing offerings data' AS ' ';
source offerings.sql
SELECT 'Importing pklogs data' AS ' ';
source pklogs.sql
SELECT 'Importing race data' AS ' ';
source race.sql
SELECT 'Importing sitecomments data' AS ' ';
source sitecomments.sql
SELECT 'Importing sitetracker data' AS ' ';
source sitetracker.sql
SELECT 'Importing theft data' AS ' ';
source theft.sql
SELECT 'Importing votes data' AS ' ';
source votes.sql

SELECT 'rift database complete!' AS ' ';