#!/bin/sh
NOCOLOR='\033[0m'
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'

echo "Setting up the riftshadow development environment"

# If you would like to do some extra provisioning you may
# add any commands you wish to this file and they will
# be run after the Homestead machine is provisioned.
#
# If you have user-specific configurations you would like
# to apply, you may also create user-customizations.sh,
# which will be run after this script.

# If you're not quite ready for Node 12.x
# Uncomment these lines to roll back to
# v11.x or v10.x

# Remove Node.js v12.x:
#sudo apt-get -y purge nodejs
#sudo rm -rf /usr/lib/node_modules/npm/lib
#sudo rm -rf //etc/apt/sources.list.d/nodesource.list

# Install Node.js v11.x
#curl -sL https://deb.nodesource.com/setup_11.x | sudo -E bash -
#sudo apt-get install -y nodejs

# Install Node.js v10.x
#curl -sL https://deb.nodesource.com/setup_10.x | sudo -E bash -
#sudo apt-get install -y nodejs

# Get a copy of the signing key for cmake's developer
echo "Installing the CMake signing key"
{
	sudo wget -O /etc/apt/trusted.gpg.d/kitware.asc https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null
	echo "${GREEN}CMake key installed${NOCOLOR}\n"
} || {
	echo "${RED}Failed to install CMake signing key. Aborting.${NOCOLOR}"
	exit
}

# Add the cmake repo to sources list
echo "Adding CMake Repo"
{
	sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu bionic main'
	echo "${GREEN}CMake Repo Added${NOCOLOR}\n"
} || {
	echo "${RED}Failed to add the CMake repo. Aborting.${NOCOLOR}"
	exit
}

# Add another repo to sources list for gcc-9, g++-9, and update
#sudo apt-add-repository ppa:ubuntu-toolchain-r/test
echo "${NOCOLOR}Updating APT${NOCOLOR}"
sudo apt-get update
echo "${GREEN}Update Complete${NOCOLOR}\n"

# Install deps
echo "Installing dependencies"
sudo apt-get install -y ninja-build make cmake gcc g++ 
echo "${GREEN}Finished installing${NOCOLOR}\n"

echo "Checking environment for a MySQL distribution"
mySqlPresent=$(type mysql >/dev/null 2>&1 && echo true || echo false)

if $mySqlPresent; then
	echo "${GREEN}A MySQL distribution found!${NOCOLOR}"
	echo "${GREEN}You are currently using: ${NOCOLOR}$(mysqld --version)\n"

	{
		sudo service mariadb start
		sudo service mariadb status | cat
	} || {
		sudo service mysql start
		sudo service mysql status | cat
	}

else
	echo "${YELLOW}No MySQL distribution found, installing MariaDB now.\n${NOCOLOR}"
	sudo apt-get install -y libmariadb-dev libmariadb-dev-compat mariadb-client mariadb-server

	echo "${GREEN}Finished installing MariaDB server and client.${NOCOLOR}"
	echo "Starting MariaDB"

	{
		sudo service mariadb start
		sudo service mariadb status | cat
	} || {
		echo "${RED}Failed to start MariaDB, please check your service settings.${NOCOLOR}"
		exit
	}
fi

# Update alternatives
#sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 60 --slave /usr/bin/g++ g++ /usr/bin/g++-9

# Setup Database
# The databases are stood up from the codified per-table SQL under db/, driven
# by db/manifest.txt. See db/README.md. Output is deliberately NOT suppressed:
# hiding it leaves out valuable debugging information.
echo "${NOCOLOR}Running database scaffolding\n"
{
	if [ -f ./db/install.sh ]; then
		sh ./db/install.sh
	elif [ -f ~/code/db/install.sh ]; then
		sh ~/code/db/install.sh
		~/code/admin/artisan migrate
	else
		echo "${RED}db/install.sh not found. Cannot scaffold the database.${NOCOLOR}"
		exit 1
	fi
} ||
{ # log that there was an error and to check the MySQL settings
	echo "${RED}Database scaffolding failed. Inspect the error above.${NOCOLOR}"
	exit 1
}

echo "${GREEN}Database scaffolding successful! Huzzah!${NOCOLOR}"
