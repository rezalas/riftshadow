# > cmake -B .
# > make
cmake_minimum_required(VERSION 3.12)

# need to define the compiler before project because reasons
if(WIN32)
	set(CCOMPILER "c:\\MinGW\\bin\\gcc.exe")
	set(CXXCOMPILER "c:\\MinGW\\bin\\gcc.exe")
else()
	set(CCOMPILER "/usr/bin/gcc")
	set(CXXCOMPILER "/usr/bin/g++")
endif()
set(CMAKE_C_COMPILER "${CCOMPILER}")
set(CMAKE_C_FLAGS "-O1 -g -DFMT_HEADER_ONLY")

set(CMAKE_CXX_COMPILER "${CXXCOMPILER}")
set(CMAKE_CXX_FLAGS "-O1 -g -Wno-write-strings -DFMT_HEADER_ONLY")


project(riftshadow)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
	include_directories("C:\\Program Files\\MariaDB 10.4\\include\\mysql" "C:\\MinGW\\msys\\1.0\\include" "${CMAKE_SOURCE_DIR}/code/include")
	link_directories("c:\\MinGW\\lib" "c:\\MinGW\\msys\\1.0\\lib" "C:\\Program Files (x86)\\MySQL\\MySQL Connector C++ 8.0\\lib" "c:\\MinGW\\msys\\1.0\\lib\\openssl\\engines-1.0.0" "${CMAKE_BINARY_DIR}\\code" "${CMAKE_BINARY_DIR}\\tests")
	set(CRYPTOLIB "libcrypto")
	set(MARIADBLIB "mysqlcppconn8-2-vs14")
else()
	include_directories("/usr/include" "/usr/include/mysql" "${CMAKE_SOURCE_DIR}/code/include")
	link_directories("/usr/lib" "${CMAKE_BINARY_DIR}\\code" "${CMAKE_BINARY_DIR}\\tests")
	set(CRYPTOLIB "crypt")
	set(MARIADBLIB "mysqlclient")
endif()

add_subdirectory(code)

enable_testing()
add_subdirectory(tests)
