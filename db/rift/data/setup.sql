---
--- Set the database name
---
use rift;

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

/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

SELECT 'rift database complete!' AS ' ';