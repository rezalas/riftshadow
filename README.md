# Riftshadow MUD

[![Build & Test](https://github.com/rezalas/riftshadow/actions/workflows/cmake.yml/badge.svg)](https://github.com/rezalas/riftshadow/actions/workflows/cmake.yml)

---

| Table of Contents |
| ------------------ |
|**[Summary](#summary)**|
|**[Original Contributors](#original-contributors)**|
|**[Goals](#goals)**|
|**[Contributing](#contributing)**|
|**[Build Requirements](#build-requirements)**|
|**[SQL Setup](#sql-setup)**|
|**[config json](#config-json)**|

## Summary

Riftshadow (RS) is a swords and sorcery style PVP-enabled multi-user dungeon (MUD) from the early 2000s. As the original server is no longer live, the original authors provided the source code to the community to use and modify so that RS can live on. 
The primary goals for this community are to restore RS to a fully-functional state capable of operating on modern hardware with all features finished and bugs eliminated or codified as features.

Preservation of the game for future generations requires  adapting the code to be easier to read and maintain, but without changing the feel of the core gameplay.

## Original Contributors
Original DikuMUD (c) Hans Staerfeldt, Katya Nyboe, Tom Madsen, Michael Seifert, and Sebastian Hammer - Based on ROM 2.4 

## Goals

* With preservation in mind, act to maintain and improve the codebase with modern standards.
* Add appropriate commenting, and remove inappropriate comments that exist.
* adapt code to be readable and maintainable over time by future developers.
* Preserve core gameplay.
* Improve security (ex. data storage of passwords) and thus safety for players and maintainers.
* Decouple storage types from the code to support multiple backends with limited code changes.

## Contributing

If you're interested in helping us with this project, you can find more information on our [Contributing page](CONTRIBUTING.md#basic)

## Build Reqs

The following packages are required to build on linux x86 and test the application at this time.

* make
* cmake
* git
* g++
* sql-server (mariadb suggested)
* libmariadb-dev
* libmariadb-dev-compat

### Building

To build, run `cmake .` at the root of the project directory which will build system-appropriate Makefile configs. Then, run `make` to begin the build process. Afterward if successful the binary is located in the `./code/` directory and ready to execute.

### Running the Server
1. Get your environment ready with the steps in "[Setting up your Environment](./CONTRIBUTING.md)"
2. From the root directory, run `make rift` to build the executable. 
3. Navigate to `./code/` and run `./rift`

### Connecting with Telnet
After the server is running open another terminal instance and run
`telnet localhost 9999`

(The domain name and port might be different if your overridden the development defaults)

### Database injection

You will also need to inject the databases into the mysql server, add a user named 'rift', and set up your connection strings properly in order to start up. 

*Note:* If you have issues getting the project to run properly you're likely missing the area files or the log 'area/login.txt'. (this is going to be refactored out eventually).

## SQL Setup

required DBs and assoc. files

| Database  | SQL struct file |
|-----------|-----------------|
|  rift_core | rift_core.sql|
|  rift | rift.sql |
|  riftforum | unknown / needs creation |

default user: rift
default pwd: rift123

The username and password can be changed using the config json and no longer need to be hosted on the same system. See the config json section for information on updating credentials.

## config json

The following is an example config.json file, which needs to be located in the root directory for the project. The defaults listed for the db connections are customizable, so if you want to name your databases differently use the same key name here but change the db value to match your setup.

```json
{
    "Port" : 9999,
     "DbConnections": {
          "rift" : {
               "Host" : "localhost",
               "User" : "rift",
               "Pwd" : "rift123",
               "Db" : "rift"
          },
          "rift_core" : {
              ...
          },
          "riftforum" : {
              ...
          }
     }
}
```
