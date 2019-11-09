CC      = g++
PROF    = -O1 -g
ifeq ($(OS),Windows_NT)
	RM		= del /f
	FNAME	= rift.exe
	INCLUDES = -w $(PROF) $(NOCRYPT) -I C:/Program\ Files/MariaDB\ 10.4/include/mysql -I C:/MinGW/msys/1.0/include
	L_FLAGS = -lm   -llibcrypto -Lc:/MinGW/lib -Lc:/MinGW/msys/1.0/lib -L"C:\Program Files (x86)\MySQL\MySQL Connector C++ 8.0\lib" -Lc:/MinGW/msys/1.0/lib/openssl/engines-1.0.0 -l mysqlcppconn8-2-vs14
	# The above library includes requires figuring out, as I'm not sure what
	# libraries we should use to get this pig to fly. I've broken it down to 
	# the point that all the object files build, but if you try to link with maria she
	# spits a mile of undefined references in your face and marries your nearest relative instead.

else 
	RM		= rm -f
	FNAME	= rift
	INCLUDES= -w $(PROF) $(NOCRYPT) -I/usr/include/mysql
	L_FLAGS =  -lm -lcrypt -L/usr/lib -lmysqlclient
endif
#INCLUDES=  -Wall -Wno-unused-variable -Wno-deprecated $(PROF) $(NOCRYPT) -I/usr/include/mysql -lmysqlclient 



O_FILES = config.o \
		  strings.o \
		  file.o \
		  sql.o \
		  bitvector.o \
		  act_comm.o \
		  act_ente.o \
		  act_info.o \
		  act_move.o \
		  act_obj.o \
		  act_wiz.o \
          alias.o \
		  ban.o \
		  comm.o \
		  const.o \
		  db.o \
		  db2.o \
		  effects.o \
		  fight.o \
		  flags.o \
	  	  handler.o \
		  healer.o \
		  interp.o \
		  lookup.o \
		  note.o \
		  magic.o \
		  recycle.o \
		  save.o \
		  scan.o \
		  skills.o \
		  tables.o \
		  update.o \
		  queue.o \
          tattoo.o \
		  dioextra.o \
		  cabal.o \
		  material.o \
		  moremagic.o \
		  devextra.o \
		  iprog.o \
		  mprog.o \
		  rprog.o \
		  aprog.o \
		  titles.o \
		  chardef.o \
		  necro.o \
		  misc.o \
		  sorcerer.o \
		  help.o \
		  quest.o \
		  warrior.o \
		  chrono.o \
		  ap.o \
		  thief.o \
		  race.o \
		  zealot.o \
		  paladin.o \
		  druid.o \
		  heal.o \
		  olc.o \
		  olc_act.o \
		  olc_save.o \
		  bit.o \
		  mem.o \
		  string.o \
		  mspec.o \
		  ispec.o \
		  newmem.o \
		  mud.o \
		  bootup.o \
		  class.o \
		  prof.o
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
