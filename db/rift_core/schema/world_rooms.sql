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
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `world_rooms`
--

DROP TABLE IF EXISTS `world_rooms`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `world_rooms` (
  `area_id` int(11) NOT NULL DEFAULT 0,
  `room_id` int(11) DEFAULT NULL,
  `vnum` int(11) NOT NULL DEFAULT 0,
  `name` varchar(200) NOT NULL DEFAULT '',
  `description` text NOT NULL,
  `sector_type` int(11) NOT NULL DEFAULT 0,
  `room_flags` bigint(20) DEFAULT 0,
  `0_to_room` int(11) NOT NULL DEFAULT -1,
  `0_exit_info` bigint(20) NOT NULL DEFAULT 0,
  `0_key` int(11) NOT NULL DEFAULT 0,
  `0_keyword` varchar(200) NOT NULL DEFAULT '',
  `1_to_room` int(11) NOT NULL DEFAULT -1,
  `1_exit_info` bigint(20) NOT NULL DEFAULT 0,
  `1_key` int(11) NOT NULL DEFAULT 0,
  `1_keyword` varchar(200) NOT NULL DEFAULT '',
  `2_to_room` int(11) NOT NULL DEFAULT -1,
  `2_exit_info` bigint(20) NOT NULL DEFAULT 0,
  `2_key` int(11) NOT NULL DEFAULT 0,
  `2_keyword` varchar(200) NOT NULL DEFAULT '',
  `3_to_room` int(11) NOT NULL DEFAULT -1,
  `3_exit_info` bigint(20) NOT NULL DEFAULT 0,
  `3_key` int(11) NOT NULL DEFAULT 0,
  `3_keyword` varchar(200) NOT NULL DEFAULT '',
  `4_to_room` int(11) NOT NULL DEFAULT -1,
  `4_exit_info` bigint(20) NOT NULL DEFAULT 0,
  `4_key` int(11) NOT NULL DEFAULT 0,
  `4_keyword` varchar(200) NOT NULL DEFAULT '',
  `5_to_room` int(11) NOT NULL DEFAULT -1,
  `5_exit_info` bigint(20) NOT NULL DEFAULT 0,
  `5_key` int(11) NOT NULL DEFAULT 0,
  `5_keyword` varchar(200) NOT NULL DEFAULT '',
  `cabal` int(11) NOT NULL DEFAULT 0,
  `owner` varchar(50) NOT NULL DEFAULT '0',
  KEY `area_id` (`area_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed
