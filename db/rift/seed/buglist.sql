/*M!999999\- enable the sandbox mode */ 
-- MariaDB dump 10.19  Distrib 10.11.14-MariaDB, for debian-linux-gnu (x86_64)
--
-- Database: rift
-- ------------------------------------------------------
-- Server version	10.11.14-MariaDB-0ubuntu0.24.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Dumping data for table `buglist`
--

LOCK TABLES `buglist` WRITE;
/*!40000 ALTER TABLE `buglist` DISABLE KEYS */;
INSERT INTO `buglist` VALUES
(1,'Calenduil','08/18/2001 10:15pm','Cimar Troopers are still bugged','They are still going after the wrong people sometimes, when a guy cast\n\rdeafen on someone else, the victim got ganked.\n\r',1),
(2,'Calenduil','08/19/2001  2:43am','Finger still has corrupt bitvectors from old system','Someone fix it.  Please? :P\n\r',1),
(3,'Morglum','08/19/2001  2:53pm','Mud crashed trying to load a pfile with a saved golem','The pfile looked fine, but once i took out the golem it loaded up fine.\n\r',1),
(4,'Carantoc','08/20/2001  8:51pm','Morglum:  Read your note on the OLC.','ADD TEH AMRS AND TEH LEGS!!!11  (and the exp mods)\n\r',1),
(5,'Morglum','08/21/2001  7:58pm','You can practice style skills before you have a breakthrough.','NEEDS FIX0r3D\n\r',1),
(6,'Morglum','08/22/2001  2:04pm','Skills increasin when you level..','Powers need to go up as well.\n\r',1),
(7,'Karsus','03/29/2002 12:41am','racial group alteration (dwarf+duergar) (elf+half+drow).','alignment can be used to prevent wear restrict flags, but racial wise it\n\rshouldnt be specific.\n\r',1),
(8,'Karsus','03/29/2002 12:43am','hobble, mv reduction needs to be set to a 25% value','hobble keeps putting players into neg moves, Way to powerfull.\n\rNeed to set it so that mv reduction is 25% of current moves. Maybee 35%\n\r',1),
(9,'Karsus','03/29/2002 12:45am','fatigue commune, saves way to often.','hobble keeps putting players into neg moves, Way to powerfull.\n\rNeed to set it so that mv reduction is 25% of current moves. Maybee 35%\n\rIts supposed to be a functional commune, up how often it drains mvs.\n\rThen reduce its damage done, your % save should be almost be direct to your\n\rsaves/100\n\r',1),
(10,'Karsus','03/29/2002 12:48am','Burden needs to be increased','hobble keeps putting players into neg moves, Way to powerfull.\n\rNeed to set it so that mv reduction is 25% of current moves. Maybee 35%\n\rIts supposed to be a functional commune, up how often it drains mvs.\n\rThen reduce its damage done, your % save should be almost be direct to your\n\rsaves/100\n\rBurden should be around +200 pounds, 75 to worn eq, 125 to inventory\n\ras to hamper skills and spells and tanking alike.\n\r',1),
(11,'Morglum','04/02/2002  9:54pm','testing','(null)',1),
(12,'Erifdel','04/14/2004  6:03pm','Mass Healing is reported to heal all in room instead of just group.','This incident was reported by Naezia.\n\r',1);
/*!40000 ALTER TABLE `buglist` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed
