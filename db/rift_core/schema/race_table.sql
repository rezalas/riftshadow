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
-- Table structure for table `race_table`
--

DROP TABLE IF EXISTS `race_table`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `race_table` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL DEFAULT '',
  `short_name` varchar(50) NOT NULL DEFAULT '',
  `imm_name` varchar(50) NOT NULL DEFAULT '',
  `race_time` varchar(50) NOT NULL DEFAULT '',
  `pcrace` int(1) NOT NULL DEFAULT 0,
  `race_status` int(4) NOT NULL DEFAULT 0,
  `act_flags` bigint(20) DEFAULT 0,
  `aff_flags` bigint(20) DEFAULT 0,
  `off_flags` bigint(20) DEFAULT 0,
  `imm_flags` bigint(20) DEFAULT 0,
  `res_flags` bigint(20) DEFAULT 0,
  `vuln_flags` bigint(20) DEFAULT 0,
  `form_flags` bigint(20) DEFAULT 0,
  `body_parts` bigint(20) DEFAULT 0,
  `arms` int(11) NOT NULL DEFAULT 0,
  `legs` int(4) NOT NULL DEFAULT 0,
  `size` int(4) NOT NULL DEFAULT 0,
  `age_mod` float NOT NULL DEFAULT 0,
  `deathtime` int(4) NOT NULL DEFAULT 0,
  `align` int(4) NOT NULL DEFAULT 0,
  `xp_add` int(4) NOT NULL DEFAULT 0,
  `max_str` int(11) NOT NULL DEFAULT 0,
  `max_int` int(11) NOT NULL DEFAULT 0,
  `max_wis` int(11) NOT NULL DEFAULT 0,
  `max_dex` int(11) NOT NULL DEFAULT 0,
  `max_con` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  KEY `id` (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=48 DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
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
