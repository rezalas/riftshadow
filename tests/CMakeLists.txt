project(riftshadow_tests)

add_library(catch INTERFACE)
target_include_directories(catch INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set(TEST_LIB_FLAGS RiftCore catch ${CRYPTOLIB} ${MARIADBLIB} m)

##
## ACT_OBJ TESTS
##
set(ACT_OBJ_TESTS act_obj_tests.c main.c)
set_source_files_properties(${ACT_OBJ_TESTS} PROPERTIES LANGUAGE CXX)
add_executable(act_obj_tests ${ACT_OBJ_TESTS})
target_link_libraries(act_obj_tests PRIVATE ${TEST_LIB_FLAGS})
add_test(NAME ACT_OBJ_TESTS COMMAND act_obj_tests)

##
## DB TESTS
##
set(DB_TESTS db_tests.c main.c)
set_source_files_properties(${DB_TESTS} PROPERTIES LANGUAGE CXX)
add_executable(db_tests ${DB_TESTS})
target_link_libraries(db_tests PRIVATE ${TEST_LIB_FLAGS})
add_test(NAME DB_TESTS COMMAND db_tests)

##
## DB TESTS
##
set(CABAL_TESTS cabal_tests.c main.c)
set_source_files_properties(${CABAL_TESTS} PROPERTIES LANGUAGE CXX)
add_executable(cabal_tests ${CABAL_TESTS})
target_link_libraries(cabal_tests PRIVATE ${TEST_LIB_FLAGS})
add_test(NAME CABAL_TESTS COMMAND cabal_tests)

##
## FIGHT TESTS
##
set(FIGHT_TESTS fight_tests.c main.c)
set_source_files_properties(${FIGHT_TESTS} PROPERTIES LANGUAGE CXX)
add_executable(fight_tests ${FIGHT_TESTS})
target_link_libraries(fight_tests PRIVATE ${TEST_LIB_FLAGS})
add_test(NAME FIGHT_TESTS COMMAND fight_tests)

##
## PROF TESTS
##
set(PROF_TESTS prof_tests.c main.c)
set_source_files_properties(${PROF_TESTS} PROPERTIES LANGUAGE CXX)
add_executable(prof_tests ${PROF_TESTS})
target_link_libraries(prof_tests PRIVATE ${TEST_LIB_FLAGS})
add_test(NAME PROF_TESTS COMMAND prof_tests)

##
## QUEUE TESTS
##
set(QUEUE_TESTS queue_tests.c main.c)
set_source_files_properties(${QUEUE_TESTS} PROPERTIES LANGUAGE CXX)
add_executable(queue_tests ${QUEUE_TESTS})
target_link_libraries(queue_tests PRIVATE ${TEST_LIB_FLAGS})
add_test(NAME QUEUE_TESTS COMMAND queue_tests)