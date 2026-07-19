project(riftshadow_src)

set(RIFT_SOURCE config.cpp
	strings.c
	file.c
	cdirectory.c
	cfilesystem.c
	clogger.c
	dbsession.c
	repositories/banrepository.c
	repositories/theftrepository.c
	repositories/inductionrepository.c
	repositories/voterepository.c
	repositories/noterepository.c
	repositories/helprepository.c
	repositories/bugrepository.c
	repositories/offeringrepository.c
	repositories/loginrepository.c
	repositories/pklogrepository.c
	repositories/sitecommentrepository.c
	repositories/sitetrackerrepository.c
	repositories/graveyardrepository.c
	repositories/playerrepository.c
	repositories/classrepository.c
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
	characterClasses/class.c
	prof.c
	vote.c
	utility.c
)

set_source_files_properties(${RIFT_SOURCE} main.c PROPERTIES LANGUAGE CXX)

# These files compile clean under -Wall -Wextra currently and must stay that way.
# So we can add stricter rules. Once we clean up the other files, we can add
# those here as well.
set(RIFT_WERROR_SOURCES
	config.cpp
	bitvector.c
	cdirectory.c
	cfilesystem.c
	clogger.c
	dbsession.c
	file.c
	repositories/banrepository.c
	repositories/theftrepository.c
	repositories/inductionrepository.c
	repositories/voterepository.c
	repositories/noterepository.c
	repositories/helprepository.c
	repositories/bugrepository.c
	repositories/offeringrepository.c
	repositories/loginrepository.c
	repositories/pklogrepository.c
	repositories/sitecommentrepository.c
	repositories/sitetrackerrepository.c
	repositories/graveyardrepository.c
	repositories/playerrepository.c
	repositories/classrepository.c
)
# Flags applied to the ratchet set, in order:
#  -x c++      Make the already-forced C++ compilation explicit for the
#              .c-extension files. Without it clang++ emits dialect warnings,
#              which -Werror would turn fatal for every modern .c.
#  -Werror     Turns all warnings into errors.
#  -Wno-error=mismatched-tags   Downgraded to a warning. It fires only in the
#              legacy headers. Main use is for the Microsoft C++ ABI.
set_source_files_properties(${RIFT_WERROR_SOURCES} PROPERTIES
	COMPILE_OPTIONS "-x;c++;-Werror;-Wno-error=mismatched-tags")

add_library(RiftCore SHARED ${RIFT_SOURCE})
add_executable(rift main.c)

target_include_directories(RiftCore PUBLIC
	"${CMAKE_SOURCE_DIR}/code/include"
	${MYSQL_INCLUDE_DIRS}
)

target_link_libraries(rift PRIVATE RiftCore ${CRYPTOLIB} ${MARIADBLIB} m)

