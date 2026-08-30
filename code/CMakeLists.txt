project(riftshadow_src)

set(RIFT_SOURCE config.cpp
	spell_target.c
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
	gameloop.c
	riftpath.c
	const.c
	persisted_enum.c
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
	editor.c
	mspec.c
	ispec.c
	pstring.c
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

# -Wno-write-strings is switched on for the whole tree in the top level
# CMakeLists, because most of this code still stores string literals in char*
# fields. These files do not, and this keeps it that way: -Werror=write-strings
# both re-enables the warning and makes it fatal, and it comes after the
# blanket -Wno on the command line, so it wins.
#
# A file belongs here once it compiles with no write-strings warning at all,
# including from the headers it pulls in. Add files as they are cleaned. The
# files still carrying sites are listed in the modernization plan, which owns
# the tree-wide sweep.
set(RIFT_WRITE_STRINGS_CLEAN
	alias.c
	bit.c
	characterClasses/class.c
	editor.c
	gameloop.c
	main.c
	pstring.c
	queue.c
	riftpath.c
	spell_target.c
	titles.c
)
set_source_files_properties(${RIFT_WRITE_STRINGS_CLEAN} PROPERTIES
	COMPILE_OPTIONS "-x;c++;-Werror=write-strings")

# The set above is already covered by -Werror, so it only needs the warning
# switched back on.
set_property(SOURCE ${RIFT_WERROR_SOURCES} APPEND PROPERTY
	COMPILE_OPTIONS "-Wwrite-strings")

add_library(RiftCore SHARED ${RIFT_SOURCE})
add_executable(rift main.c)

target_include_directories(RiftCore PUBLIC
	"${CMAKE_SOURCE_DIR}/code/include"
	${MYSQL_INCLUDE_DIRS}
)

# Where the game looks for areas, player files and logs when RIFT_ROOT is not
# set in the environment. Only riftpath.c reads it.
target_compile_definitions(RiftCore PRIVATE
	RIFT_DEFAULT_ROOT="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(rift PRIVATE RiftCore ${CRYPTOLIB} ${MARIADBLIB} m)

