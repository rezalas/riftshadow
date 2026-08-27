project(riftshadow_tests)

add_library(catch INTERFACE)
target_include_directories(catch INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set(TEST_LIB_FLAGS RiftCore catch ${CRYPTOLIB} ${MARIADBLIB} m)

set_source_files_properties(main.c PROPERTIES LANGUAGE CXX)
add_library(TestMain SHARED main.c)
set_target_properties(TestMain PROPERTIES LINKER_LANGUAGE CXX)

set(TEST_SRC_FILES act_obj_tests.c
	ban_tests.c
	entity_storage_tests.c
	bitflag_tests.c
	bug_tests.c
	cabal_tests.c
	class_tests.c
	db_tests.c
	fight_tests.c
	handler_tests.c
	help_tests.c
	induction_tests.c
	login_tests.c
	magic_tests.c
	mprog_tests.c
	note_tests.c
	pathfind_tests.c
	offering_tests.c
	pklog_tests.c
	sitecomment_tests.c
	sitetracker_tests.c
	graveyard_tests.c
	handle_tests.c
	player_tests.c
	prof_tests.c
	queue_tests.c
	rune_tests.c
	spell_target_tests.c
	theft_tests.c
	vote_tests.c
	utility_tests.c
	standalone_tests.c
	update_tests.c
)
# world_fixture.h is a header, so it is not listed above. It is included by the
# tests that need a loaded world, and it holds what test_helpers.c used to.

##
## CREATE TEST TARGETS
##
foreach(testsource ${TEST_SRC_FILES})
	get_filename_component(filename ${testsource} NAME_WE)
	set_source_files_properties(${filename}.c PROPERTIES LANGUAGE CXX)
	add_executable(${filename} ${filename}.c)
	target_link_libraries(${filename} PRIVATE ${TEST_LIB_FLAGS} TestMain)
	# No WORKING_DIRECTORY on purpose. The game resolves its own paths from the
	# root rather than from the directory it happens to be started in, and a
	# test that only passes when launched from one place is not testing that.
	add_test(NAME ${filename} COMMAND ${filename})
endforeach()
