CC      = g++
PROF    = -O1 -g
ifeq ($(OS),Windows_NT)
	RM		= del /f
	FNAME	= rift_tests.exe
	INCLUDES = -w $(PROF) $(NOCRYPT) -I C:/Program\ Files/MariaDB\ 10.4/include/mysql -I C:/MinGW/msys/1.0/include
	L_FLAGS = -lm   -llibcrypto -Lc:/MinGW/lib -Lc:/MinGW/msys/1.0/lib -L"C:\Program Files (x86)\MySQL\MySQL Connector C++ 8.0\lib" -Lc:/MinGW/msys/1.0/lib/openssl/engines-1.0.0 -l mysqlcppconn8-2-vs14
	# The above library includes requires figuring out, as I'm not sure what
	# libraries we should use to get this pig to fly. I've broken it down to 
	# the point that all the object files build, but if you try to link with maria she
	# spits a mile of undefined references in your face and marries your nearest relative instead.

else 
	RM		= rm -f
	FNAME	= rift_tests
	INCLUDES= -w $(PROF) $(NOCRYPT) -I/usr/include/mysql
	L_FLAGS =  -lm -lcrypt -L/usr/lib -lmysqlclient
endif
#INCLUDES=  -Wall -Wno-unused-variable -Wno-deprecated $(PROF) $(NOCRYPT) -I/usr/include/mysql -lmysqlclient 


O_FILES = ../code/config.o		\
			../code/strings.o	\
			../code/file.o		\
			../code/sql.o		\
			../code/bitvector.o	\
			../code/act_comm.o	\
			../code/act_ente.o	\
			../code/act_info.o	\
			../code/act_move.o	\
			../code/act_obj.o	\
			../code/act_wiz.o	\
			../code/alias.o		\
			../code/ban.o		\
			../code/comm.o		\
			../code/const.o		\
			../code/db.o		\
			../code/db2.o		\
			../code/effects.o	\
			../code/fight.o		\
			../code/flags.o		\
			../code/handler.o	\
			../code/healer.o	\
			../code/interp.o	\
			../code/lookup.o	\
			../code/note.o		\
			../code/magic.o		\
			../code/recycle.o	\
			../code/save.o		\
			../code/scan.o		\
			../code/skills.o	\
			../code/tables.o	\
			../code/update.o	\
			../code/queue.o		\
			../code/tattoo.o	\
			../code/dioextra.o	\
			../code/cabal.o		\
			../code/material.o	\
			../code/moremagic.o	\
			../code/devextra.o	\
			../code/iprog.o		\
			../code/mprog.o		\
			../code/rprog.o		\
			../code/aprog.o		\
			../code/titles.o	\
			../code/chardef.o	\
			../code/necro.o		\
			../code/misc.o		\
			../code/sorcerer.o	\
			../code/help.o		\
			../code/quest.o		\
			../code/warrior.o	\
			../code/chrono.o	\
			../code/ap.o		\
			../code/thief.o		\
			../code/race.o		\
			../code/zealot.o	\
			../code/paladin.o	\
			../code/druid.o		\
			../code/heal.o		\
			../code/olc.o		\
			../code/olc_act.o	\
			../code/olc_save.o	\
			../code/bit.o		\
			../code/mem.o		\
			../code/string.o	\
			../code/mspec.o		\
			../code/ispec.o		\
			../code/newmem.o	\
			../code/mud.o		\
			../code/bootup.o	\
			../code/class.o		\
			../code/prof.o		\
			db_tests.o			\
			fight_tests.o		\
			main.o
		
			#$(find ../code/*.o -printf "%p \")
tartarus: $(O_FILES)
	$(RM) $(FNAME)
	$(CC) $(PROF) -o $(FNAME) $(O_FILES) $(L_FLAGS)
ifneq ($(OS),Windows_NT)
	chmod 770 *.o
endif
clean:
	$(RM) *.o

.c.o: merc.h
	$(CC) -c $(INCLUDES) $<
