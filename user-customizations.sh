# General customizations go in this file

# Create a rift user for all DBs and give it permissions for all DBs

# MySQL:
# Create rift user with a password of rift123 and assign it to the databases
mysql -uhomestead -psecret -Dmysql -e "CREATE USER IF NOT EXISTS 'rift'@'localhost' IDENTIFIED BY 'rift123';"
mysql -uhomestead -psecret -Dmysql -e "GRANT ALTER,CREATE,DELETE,DROP,INDEX,INSERT,SELECT,UPDATE ON rift.* TO 'rift'@'localhost';"
mysql -uhomestead -psecret -Dmysql -e "GRANT ALTER,CREATE,DELETE,DROP,INDEX,INSERT,SELECT,UPDATE ON rift_core.* TO 'rift'@'localhost';"

# PostgreSQL:
# Create rift user with a password of rift123 and assign it to the databases
psql "dbname=homestead host=localhost user=homestead password=secret" -c "CREATE USER rift PASSWORD 'rift123'"
psql "dbname=homestead host=localhost user=homestead password=secret" -c "GRANT ALL PRIVILEGES ON DATABASE rift TO rift"
psql "dbname=homestead host=localhost user=homestead password=secret" -c "GRANT ALL PRIVILEGES ON DATABASE rift_core TO rift"

echo "Database user 'rift' added for databases 'rift' and 'rift_core'."