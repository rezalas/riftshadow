# > cmake -B .
# > make
cmake_minimum_required(VERSION 3.15)

# need to define the compiler before project because reasons
if(WIN32)
	set(CCOMPILER "c:\\MinGW\\bin\\gcc.exe")
	set(CXXCOMPILER "c:\\MinGW\\bin\\gcc.exe")
else()
	set(CCOMPILER "/usr/bin/gcc")
	set(CXXCOMPILER "/usr/bin/g++")
endif()
set(CMAKE_C_COMPILER "${CCOMPILER}")
set(CMAKE_C_FLAGS "-O1 -g")

set(CMAKE_CXX_COMPILER "${CXXCOMPILER}")
set(CMAKE_CXX_FLAGS "-O1 -g -Wno-write-strings")


project(riftshadow)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
	include_directories("C:\\Program Files\\MariaDB 10.4\\include\\mysql" "C:\\MinGW\\msys\\1.0\\include")
	link_directories("c:\\MinGW\\lib" "c:\\MinGW\\msys\\1.0\\lib" "C:\\Program Files (x86)\\MySQL\\MySQL Connector C++ 8.0\\lib" "c:\\MinGW\\msys\\1.0\\lib\\openssl\\engines-1.0.0")
	set(CRYPTOLIB "libcrypto")
	set(MARIADBLIB "mysqlcppconn8-2-vs14")
else()
	include_directories("/usr/include" "/usr/include/mysql")
	link_directories("/usr/lib")
	set(CRYPTOLIB "crypt")
	set(MARIADBLIB "mysqlclient")
endif()

##
## SOURCE
##

set(RIFT_SOURCE code/config.cpp
	code/config.h
	code/strings.c
	code/file.c
	code/sql.c
	code/bitvector.c
	code/act_comm.c
	code/act_comm.h
	code/act_ente.c
	code/act_ente.h
	code/act_info.c
	code/act_info.h
	code/act_move.c
	code/act_move.h
	code/act_obj.c
	code/act_obj.h
	code/act_wiz.c
	code/act_wiz.h
	code/alias.c
	code/alias.h
	code/ban.c
	code/ban.h
	code/comm.c
	code/comm.h
	code/const.c
	code/db.c
	code/db.h
	code/db2.c
	code/db2.h
	code/effects.c
	code/effects.h
	code/fight.c
	code/fight.h
	code/flags.c
	code/flags.h
	code/handler.c
	code/handler.h
	code/healer.c
	code/healer.h
	code/interp.c
	code/interp.h
	code/lookup.c
	code/lookup.h
	code/note.c
	code/note.h
	code/magic.c
	code/magic.h
	code/recycle.c
	code/recycle.h
	code/save.c
	code/save.h
	code/scan.c
	code/scan.h
	code/skills.c
	code/skills.h
	code/tables.c
	code/tables.h
	code/update.c
	code/update.h
	code/queue.c
	code/queue.h
	code/tattoo.c
	code/tattoo.h
	code/dioextra.c
	code/dioextra.h
	code/cabal.c
	code/cabal.h
	code/material.c
	code/moremagic.c
	code/moremagic.h
	code/devextra.c
	code/devextra.h
	code/iprog.c
	code/iprog.h
	code/mprog.c
	code/mprog.h
	code/rprog.c
	code/rprog.h
	code/aprog.c
	code/aprog.h
	code/titles.c
	code/chardef.c
	code/necro.c
	code/necro.h
	code/misc.c
	code/misc.h
	code/sorcerer.c
	code/sorcerer.h
	code/help.c
	code/help.h
	code/quest.c
	code/quest.h
	code/warrior.c
	code/warrior.h
	code/chrono.c
	code/chrono.h
	code/ap.c
	code/ap.h
	code/thief.c
	code/thief.h
	code/race.c
	code/race.h
	code/zealot.c
	code/zealot.h
	code/paladin.c
	code/paladin.h
	code/druid.c
	code/druid.h
	code/heal.c
	code/heal.h
	code/olc.c
	code/olc.h
	code/olc_act.c
	code/olc_act.h
	code/olc_save.c
	code/olc_save.h
	code/bit.c
	code/bit.h
	code/mem.c
	code/mem.h
	code/string.c
	code/string.h
	code/mspec.c
	code/mspec.h
	code/ispec.c
	code/newmem.c
	code/newmem.h
	code/mud.c
	code/mud.h
	code/bootup.c
	code/bootup.h
	code/class.c
	code/class.h
	code/prof.c
	code/prof.h
)

set_source_files_properties(${RIFT_SOURCE} code/main.c PROPERTIES LANGUAGE CXX)
add_library(RiftCore ${RIFT_SOURCE})
add_executable(rift code/main.c)

target_link_libraries(rift RiftCore ${CRYPTOLIB} ${MARIADBLIB} m)

##
## TESTS
##

set(RIFT_SOURCE_TEST 
	tests/db_tests.c
	tests/fight_tests.c
	tests/main.c
)

set_source_files_properties(${RIFT_SOURCE_TEST} tests/main.c PROPERTIES LANGUAGE CXX)
add_executable(rift_tests ${RIFT_SOURCE_TEST})

target_link_libraries(rift_tests RiftCore ${CRYPTOLIB} ${MARIADBLIB} m)

enable_testing()
add_test(NAME RiftTest COMMAND rift_tests)

