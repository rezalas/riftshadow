# > cmake -B .
# > make
cmake_minimum_required(VERSION 3.15)

# need to define the compiler before project because reasons
if(WIN32)
	set(CCOMPILER "c:\\MinGW\\bin\\gcc.exe")
	set(CXXCOMPILER "c:\\MinGW\\bin\\gcc.exe")
else()
	set(CCOMPILER "/usr/bin/gcc")
	set(CXXCOMPILER "/usr/bin/g++")
endif()
set(CMAKE_C_COMPILER "${CCOMPILER}")
set(CMAKE_C_FLAGS "-O1 -g")

set(CMAKE_CXX_COMPILER "${CXXCOMPILER}")
set(CMAKE_CXX_FLAGS "-O1 -g -Wno-write-strings")


project(riftshadow)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
	include_directories("C:\\Program Files\\MariaDB 10.4\\include\\mysql" "C:\\MinGW\\msys\\1.0\\include")
	link_directories("c:\\MinGW\\lib" "c:\\MinGW\\msys\\1.0\\lib" "C:\\Program Files (x86)\\MySQL\\MySQL Connector C++ 8.0\\lib" "c:\\MinGW\\msys\\1.0\\lib\\openssl\\engines-1.0.0")
	set(CRYPTOLIB "libcrypto")
	set(MARIADBLIB "mysqlcppconn8-2-vs14")
else()
	include_directories("/usr/include" "/usr/include/mysql")
	link_directories("/usr/lib")
	set(CRYPTOLIB "crypt")
	set(MARIADBLIB "mysqlclient")
endif()

add_subdirectory(code)

enable_testing()
add_subdirectory(tests)
