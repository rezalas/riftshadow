--
-- mariadb setup script for riftshadow
-- (FOR DEVELOPMENT ONLY)
--
-- create the rift user
use mysql;
DROP USER IF EXISTS 'rift'@'localhost';
CREATE USER 'rift'@'localhost' IDENTIFIED BY 'rift123';

-- grant rift user rights to access mariadb and all rights to the two dbs
GRANT USAGE ON *.* TO 'rift'@'localhost';
GRANT ALL PRIVILEGES ON rift.* TO 'rift'@'localhost';
GRANT ALL PRIVILEGES ON rift_core.* TO 'rift'@'localhost';
FLUSH PRIVILEGES;

-- debug purposes. ensure the rift user has correct rights
SHOW GRANTS FOR 'rift'@'localhost';

