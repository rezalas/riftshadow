/*M!999999\- enable the sandbox mode */ 
-- MariaDB dump 10.19  Distrib 10.11.14-MariaDB, for debian-linux-gnu (x86_64)
--
-- Database: rift_core
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
-- Dumping data for table `class_table`
--

LOCK TABLES `class_table` WRITE;
/*!40000 ALTER TABLE `class_table` DISABLE KEYS */;
INSERT INTO `class_table` VALUES
(1,'warrior','War',0,0,24589,5,'class basics','class default',0,1),
(2,'thief','Thi',3,0,24588,4,'class basics','class default',0,1),
(3,'zealot','Zea',2,0,24589,3,'class basics','class default',2,1),
(4,'paladin','Pal',0,4,24589,3,'class basics','class default',2,1),
(5,'anti-paladin','A-P',0,6,24589,4,'class basics','class default',1,1),
(6,'ranger','Ran',0,0,24589,5,'class basics','class default',0,0),
(7,'assassin','Asn',3,0,24589,4,'class basics','class default',1,1),
(8,'shapeshifter','Shp',1,0,24589,1,'class basics','class default',1,0),
(9,'healer','Hea',2,0,24586,3,'class basics','class default',2,1),
(10,'necromancer','Nec',1,6,24589,1,'class basics','class default',1,1),
(11,'sorcerer','Sor',1,0,24588,1,'class basics','class default',1,1),
(0,'none','non',0,0,0,0,'basic skills','basic skills',0,0);
/*!40000 ALTER TABLE `class_table` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed
