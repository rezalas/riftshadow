cmake_minimum_required(VERSION 3.16)

project(riftshadow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_compile_definitions(FMT_HEADER_ONLY)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wno-write-strings>)

# Let's see those warnings. Since warnings can hide bugs
# or weird behavior, we want to see them all.
# -Wall -Wextra are on everywhere as *warnings only*
add_compile_options(-Wall -Wextra)

# Catches expression-level logic errors that -Wall -Wextra miss: branches
# duplicated within a single if/else chain. It stays a warning rather than an
# error because its one remaining site is a pair of byte-identical branches in
# the circle damage ladder, and this diagnostic is the only automated thing
# still pointing at it.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options(-Wduplicated-branches)
endif()

# This is mostly a severity change rather than new diagnostics. Eight of the
# eleven groups below are already reported by the -Wall -Wextra above, and all
# that happens here is the build stops when one of them fires. Only
# -Wnull-dereference, -Wduplicated-cond and -Wlogical-op are groups those two
# do not already enable.
#
# What makes stopping the build safe is that every group here is at zero across
# a clean build of the whole tree, so a violation cannot appear without
# reintroducing a defect class that has already been cleared out of this
# codebase. Each of them has caught a real one. -Wimplicit-fallthrough is a
# missing `break` that left a whole item class immune to shock. -Wempty-body is
# the stray `;` after an `if`. -Wtautological-compare is `x == x` and an array
# tested against 0. -Wsequence-point is `flag = flag++`, which never
# increments. -Wbool-compare is `10 > level < 15`, which parses but does not
# mean what it reads like.
#
# The split is by which compiler actually emits the diagnostic, not by which
# one accepts the option, and the two are not the same question. Each group was
# checked by compiling a planted violation with both compilers at -std=c++17.
# clang rejects three of the gated options outright, and an unknown -Werror=
# option is a hard error there whether or not a translation unit opted into
# -Werror. It accepts the other three and then never fires them: it has no
# -Wnull-dereference and no -Wint-in-bool-context, and its equivalent of
# -Wsequence-point is -Wunsequenced, which correctly stays quiet because C++17
# made `i = i++` well defined. Rejected and inert are grouped together so that
# the gated list means one thing, which is that the diagnostic is GCC only in
# practice.
add_compile_options(-Werror=implicit-fallthrough -Werror=empty-body
	-Werror=tautological-compare -Werror=missing-field-initializers
	-Werror=misleading-indentation)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options(-Werror=null-dereference -Werror=sequence-point
		-Werror=int-in-bool-context -Werror=bool-compare
		-Werror=duplicated-cond -Werror=logical-op)
endif()

# Build option used to turn on a dedicated ASAN+UBSAN build. 
# Basically a built in Valgrind alternative. This is useful
# for debugging memory issues, but it is not suitable for
# production builds. THIS IS OFF BY DEFAULT.
# To turn it on, use -DRIFT_SANITIZE=ON
option(RIFT_SANITIZE "Build with AddressSanitizer + UndefinedBehaviorSanitizer" OFF)
if(RIFT_SANITIZE)
	add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
	add_link_options(-fsanitize=address,undefined)
endif()

if(WIN32)
	set(MYSQL_INCLUDE_DIRS "C:/Program Files/MariaDB 10.4/include/mysql" "C:/MinGW/msys/1.0/include")
	link_directories("c:/MinGW/lib" "c:/MinGW/msys/1.0/lib" "C:/Program Files (x86)/MySQL/MySQL Connector C++ 8.0/lib" "c:/MinGW/msys/1.0/lib/openssl/engines-1.0.0")
	set(CRYPTOLIB "libcrypto")
	set(MARIADBLIB "mysqlcppconn8-2-vs14")
else()
	set(MYSQL_INCLUDE_DIRS "/usr/include/mysql")
	find_library(CRYPTOLIB NAMES crypt REQUIRED)
	find_library(MARIADBLIB NAMES mysqlclient mariadb REQUIRED)
endif()

add_subdirectory(code)

enable_testing()
add_subdirectory(tests)
