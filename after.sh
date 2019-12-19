#!/bin/sh

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
wget -O /etc/apt/trusted.gpg.d/kitware.asc https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null

# Add the cmake repo to sources list
sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu bionic main'

# Add another repo to sources list for gcc-9, g++-9, and update
sudo apt-add-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update

# Install deps
sudo apt-get install -y ninja-build cmake gcc-9 g++-9 libmariadb-dev libmariadb-dev-compat

# Update alternatives
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 60 --slave /usr/bin/g++ g++ /usr/bin/g++-9
