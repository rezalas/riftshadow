CC      = g++
PROF    = -O1 -g
INCLUDES=  -Wall -Wno-unused-variable -Wno-deprecated $(PROF) $(NOCRYPT) -I/usr/include/mysql
L_FLAGS =  $(PROF) -lcrypt -L/usr/lib -lmysqlclient

O_FILES = strings.o \
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
	rm -f rift
	$(CC) $(L_FLAGS) -o rift $(O_FILES)
	chmod 770 *.o

clean:
	rm -f *.o

.c.o: merc.h
	$(CC) -c $(INCLUDES) $<
