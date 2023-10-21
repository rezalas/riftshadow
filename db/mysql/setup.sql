-- create the rift user
use mysql;
CREATE USER IF NOT EXISTS 'rift'@'localhost' IDENTIFIED BY 'rift123';

-- grant rift user rights to access mariadb and all rights to the two dbs
GRANT USAGE ON *.* TO 'rift'@'localhost' IDENTIFIED BY 'rift123';
GRANT ALL PRIVILEGES ON rift.* TO 'rift'@'localhost';
GRANT ALL PRIVILEGES ON rift_core.* TO 'rift'@'localhost';
FLUSH PRIVILEGES;

-- debug purposes. ensure the rift user has correct rights
SHOW GRANTS FOR 'rift'@'localhost';