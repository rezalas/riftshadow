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

# Four extra groups that catch expression-level logic errors -Wall -Wextra
# miss: duplicated conditions and branches, bitwise operators used where
# logical ones were meant, and dereferences the compiler can prove are null.
# Warnings only for now. They are promoted to errors once the existing
# diagnostics they report have been worked through.
#
# Only -Wnull-dereference is portable. The other three are GCC only, and the
# sanitizer build compiles with clang, where an unrecognized warning option is
# itself a warning. That turns into a hard error on the translation units built
# with -Werror, so the three are gated on the compiler rather than added
# everywhere.
add_compile_options(-Wnull-dereference)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options(-Wduplicated-cond -Wduplicated-branches -Wlogical-op)
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
