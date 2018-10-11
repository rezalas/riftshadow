# Riftshadow MUD
[![Build Status](https://travis-ci.com/rezalas/riftshadow.svg?branch=master)](https://travis-ci.com/rezalas/riftshadow)
---
RS is a fun swords and sorcery style PVP-enabled MUD from the early 2000s. My goal for this community is to restore RS to a fully-functional state, capable of operating on modern hardware. Along the way the community should have the goal of adapting the code to be easier to read and maintain without changing the play of the game itself.

### Goals 
* Add appropriate commenting and remove inappropriate comments that exist 
* adapt code to be readable and maintainable over time by multiple developers
* Preserve gameplay style
* Improve security (ex. data storage of passwords)
* Decouple storage type from the code to support multiple backends

### Build Reqs
The following packages are required to build on linux x86 and test the application at this time.
* make
* g++
* mysql-server
* libmysqlclient-dev

You will also need to inject the databases into the mysql server, add a user named 'rift', and set up your connection strings properly in order to start up. After building the project you will need to copy the generted 'rift' file to either the area folder, or to whatever execution folder you intend to use (if you intend to use a different one also make the rs.conf file and copy the areas over as well). 

###### rift mysql setup
required DBs and assoc. files

| Database  | SQL struct file |
|-----------|-----------------|
|  rift_core | rift_core.sql|
|  rift | rift.sql |
|  riftforum | unknown / needs creation |

default user: rift
default pwd: rift123

###### rs.conf
``` 
game_port = 6000
base_directory = /home/rift/code/newcode/
player_dir = /home/rift/player/
```
