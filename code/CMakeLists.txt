project(riftshadow_src)

set(RIFT_SOURCE config.cpp
	strings.c
	file.c
	cdirectory.c
	clogger.c
	sql.c
	bitvector.c
	act_comm.c
	act_ente.c
	act_info.c
	act_move.c
	act_obj.c
	act_wiz.c
	alias.c
	ban.c
	comm.c
	const.c
	db.c
	db2.c
	effects.c
	fight.c
	flags.c
	handler.c
	characterClasses/healer.c
	interp.c
	lookup.c
	note.c
	magic.c
	recycle.c
	save.c
	scan.c
	skills.c
	tables.c
	update.c
	queue.c
	tattoo.c
	dioextra.c
	cabal.c
	material.c
	moremagic.c
	devextra.c
	iprog.c
	mprog.c
	rprog.c
	aprog.c
	titles.c
	chardef.c
	characterClasses/necro.c
	misc.c
	characterClasses/sorcerer.c
	help.c
	quest.c
	characterClasses/warrior.c
	characterClasses/chrono.c
	characterClasses/ap.c
	characterClasses/thief.c
	race.c
	characterClasses/zealot.c
	characterClasses/paladin.c
	characterClasses/druid.c
	heal.c
	olc.c
	olc_act.c
	olc_save.c
	bit.c
	mem.c
	string.c
	mspec.c
	ispec.c
	newmem.c
	mud.c
	bootup.c
	characterClasses/class.c
	prof.c
	vote.c
	utility.c
)

set_source_files_properties(${RIFT_SOURCE} main.c PROPERTIES LANGUAGE CXX)
add_library(RiftCore SHARED ${RIFT_SOURCE})
add_executable(rift main.c)

target_link_libraries(rift PRIVATE RiftCore ${CRYPTOLIB} ${MARIADBLIB} m)

