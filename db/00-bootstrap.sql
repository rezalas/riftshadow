--
-- Bootstrap: databases, the dev user, and grants.
-- (FOR DEVELOPMENT ONLY -- the password below is a well-known dev credential.)
--
-- Replaces the old db/setup.sql, now archived at
-- docs/historical/db-dumps/setup.sql.
--
-- Only `rift_core` and `rift` are created. config.json also carries a
-- `riftforum` connection block, but no code path ever requests it
-- (Config::GetDbConnection is only ever called with "rift_core" and "rift"),
-- so there is deliberately no riftforum database here.
--

CREATE DATABASE IF NOT EXISTS rift_core;
CREATE DATABASE IF NOT EXISTS rift;

--
-- Recreate the rift user so repeated installs converge on the same state.
--
USE mysql;
DROP USER IF EXISTS 'rift'@'localhost';
CREATE USER 'rift'@'localhost' IDENTIFIED BY 'rift123';

GRANT USAGE ON *.* TO 'rift'@'localhost';
GRANT ALL PRIVILEGES ON rift.* TO 'rift'@'localhost';
GRANT ALL PRIVILEGES ON rift_core.* TO 'rift'@'localhost';
FLUSH PRIVILEGES;
