project(riftshadow_tests)

add_library(catch INTERFACE)
target_include_directories(catch INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set(TEST_LIB_FLAGS RiftCore catch ${CRYPTOLIB} ${MARIADBLIB} m)

set_source_files_properties(main.c PROPERTIES LANGUAGE CXX)
add_library(TestMain OBJECT main.c)


set(TEST_SRC_FILES act_obj_tests.c
	db_tests.c
	fight_tests.c
	queue_tests.c
)

##
## CREATE TEST TARGETS
##
foreach(testsource ${TEST_SRC_FILES})
	get_filename_component(filename ${testsource} NAME_WE)
	set_source_files_properties(${filename} PROPERTIES LANGUAGE CXX)
	add_executable(${filename} ${filename}.c $<TARGET_OBJECTS:TestMain>)
	target_link_libraries(${filename} PRIVATE ${TEST_LIB_FLAGS})
	add_test(NAME ${filename} COMMAND ${filename})
endforeach()
