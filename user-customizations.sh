# General customizations go in this file

# Create a rift user for all DBs and give it permissions for all DBs

# MySQL:

USER="rift";
PASS="rift123";

# Create user with a password and assign it to the databases
mysql -uhomestead -psecret -e "CREATE USER IF NOT EXISTS '$USER'@'localhost' IDENTIFIED BY '$PASS';"
mysql -uhomestead -psecret -e "GRANT ALTER,CREATE,DELETE,DROP,INDEX,INSERT,SELECT,UPDATE ON rift.* TO '$USER'@'localhost';"
mysql -uhomestead -psecret -e "GRANT ALTER,CREATE,DELETE,DROP,INDEX,INSERT,SELECT,UPDATE ON rift_core.* TO '$USER'@'localhost';"

echo "Database user '$USER' added for databases 'rift' and 'rift_core'."