project(riftshadow_tests)

add_library(catch INTERFACE)
target_include_directories(catch INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set(RIFT_SOURCE_TEST 
	db_tests.c
	fight_tests.c
	main.c
)

set_source_files_properties(${RIFT_SOURCE_TEST} main.c PROPERTIES LANGUAGE CXX)
add_executable(rift_tests ${RIFT_SOURCE_TEST})

target_link_libraries(rift_tests PRIVATE RiftCore catch ${CRYPTOLIB} ${MARIADBLIB} m)

add_test(NAME RiftTest COMMAND rift_tests)
