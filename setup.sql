--
-- mariadb setup script for riftshadow
-- (FOR DEVELOPMENT ONLY)
--

-- create the required databases
CREATE DATABASE rift_core;
CREATE DATABASE rift;

-- import rift_core script into rift_core db
use rift_core;
source ./rift_core.sql;

-- import rift script into rift db
use rift;
source ./rift.sql;

-- create the rift user
use mysql;
CREATE USER 'rift'@'localhost' IDENTIFIED BY 'rift123';

-- grant rift user rights to access mariadb and all rights to the two dbs
GRANT USAGE ON *.* TO 'rift'@'localhost' IDENTIFIED BY 'rift123';
GRANT ALL PRIVILEGES ON rift.* TO 'rift'@'localhost';
GRANT ALL PRIVILEGES ON rift_core.* TO 'rift'@'localhost';
FLUSH PRIVILEGES;

-- debug purposes. ensure the rift user has correct rights
SHOW GRANTS FOR 'rift'@'localhost';


