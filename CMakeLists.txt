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

# Reports a pointer the optimizer can prove reaches a dereference while
# possibly null. It stays a warning for a reason specific to how it works. The
# diagnostic is driven by value range propagation, so it emits nothing at -O0
# and can only appear in an optimized build. A Debug tree cannot establish a
# baseline for it, which means the zero that the gated list below rests on is
# not something this group is able to supply. It was gated as an error in
# 1f05371 on the strength of a clean Debug build, where it is structurally
# silent, and the first optimized build to run afterward stopped on it.
#
# At -O3 across the whole tree it reports 71 distinct in repo sites in 25
# translation units, plus two more that resolve to inlined libstdc++ headers.
# Count these as warnings, not as errors. A translation unit that errors stops
# before the later passes run, so the same tree gated reports only 58 sites in
# 21 units. Fixing toward a zero measured under -Werror would have been chasing
# a number that grows as it is approached.
#
# The largest concentration is the OLC editor, 28 of the 71, where descriptor
# handles are arrowed off Deref with no null check, both directly as in
# `switch (Deref(ch->desc)->editor)` and through the EDIT_MOB, EDIT_OBJ and
# EDIT_AREA macros in olc.h. The remainder divide into two shapes. Some
# re-Deref a handle already proven non null a few lines up, where the repair is
# to use the local that already holds the proof. Others never had a guard at
# all, and predate the gating commit rather than arriving with it.
#
# That population is now zero and this is promoted to an error below, with the
# rest of the history above kept because it explains how the number has to be
# measured. Two things about it still hold and are the reason it sits apart from
# the groups in the gated list.
#
# It is silent at -O0, so a Debug build cannot catch a new violation and a
# developer working locally will not see one. The Release leg of CI is what
# holds this line, not the tree anyone edits in.
#
# It also has to be measured ungated. A translation unit that errors stops
# before the later passes run, so a gated tree under counts itself: the same
# tree that reported 71 sites reported 58 once it was an error. Take the number
# with the warning form, then promote.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options(-Werror=null-dereference)
endif()

# Same shape, same reasons. -Wall reports this one, but only where an optimized
# build's range analysis can see the index, so it is silent at -O0 and the
# Release leg of CI is what holds it. Both sites it found were live: a
# constructor clearing a third slot of two-element arrays, and a loop reading
# the lesser-demon favors at indexes that belonged to the greater ones. Each was
# right only because of what the struct happened to lay out next.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options(-Werror=array-bounds)
endif()

# This is mostly a severity change rather than new diagnostics. Nine of the
# eleven groups below are already reported by the -Wall -Wextra above, and all
# that happens here is the build stops when one of them fires. Only
# -Wduplicated-cond and -Wlogical-op are groups those two do not already
# enable.
#
# What makes stopping the build safe is that every group here is at zero across
# a clean build of the whole tree, so a violation cannot appear without
# reintroducing a defect class that has already been cleared out of this
# codebase. Each of them has caught a real one. -Wimplicit-fallthrough is a
# missing `break` that left a whole item class immune to shock. -Wempty-body is
# the stray `;` after an `if`. -Wtautological-compare is `x == x` and an array
# tested against 0. -Wsequence-point is `flag = flag++`, which never
# increments. -Wbool-compare is `10 > level < 15`, which parses but does not
# mean what it reads like. -Wsign-compare is a bounds check written as
# `idx > -1 || idx < table.size()`, where the `||` should have been `&&` and the
# only thing hiding it was `-1` converting to a huge unsigned value. That last
# one is worth reading twice. Casting the size to silence the warning would have
# turned a function that worked into an out of bounds read.
#
# The split is by which compiler actually emits the diagnostic, not by which
# one accepts the option, and the two are not the same question. Each group was
# checked by compiling a planted violation with both compilers at -std=c++17.
# clang rejects three of the gated options outright, and an unknown -Werror=
# option is a hard error there whether or not a translation unit opted into
# -Werror. It accepts the other two and then never fires them: it has no
# -Wint-in-bool-context, and its equivalent of -Wsequence-point is
# -Wunsequenced, which correctly stays quiet because C++17 made `i = i++` well
# defined. Rejected and inert are grouped together so that the gated list means
# one thing, which is that the diagnostic is GCC only in practice.
#
# -Wformat joined the list later, after the enum-class conversion. A scoped enum
# passed to printf's %d is the wrong type even when its underlying type is int,
# and five of them had landed, four in the player-file writers. The bytes were
# right only because of how the ABI passes a scoped enum. Both compilers emit
# this one, checked the same way as the rest.
#
# -Wswitch came from the same conversion. Once a family is an enum class, a
# switch over it that leaves values out is reported, and the conversion of the
# first 23 families left 21 such switches unread. None was a live defect, but
# reading them is the point: the next family converted will report its gaps
# here instead of adding to an uncounted pile. Each switch now lists the values
# it leaves alone, or says in a comment why everything else does nothing. Both
# compilers emit this one too.
add_compile_options(-Werror=implicit-fallthrough -Werror=empty-body
	-Werror=tautological-compare -Werror=missing-field-initializers
	-Werror=misleading-indentation -Werror=sign-compare -Werror=format
	-Werror=switch)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options(-Werror=sequence-point
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
