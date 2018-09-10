cmake_minimum_required(VERSION 3.1)

### Hunter snapshot that will be used ###
### https://github.com/ruslo/hunter/releases ###
include("cmake/HunterGate.cmake")
HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.23.4.tar.gz"
        SHA1 "8b2255921208517a55c9533c500131387e3c3dd0"
)

project(ThreadWatcher)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

### Download dependencies ###
hunter_add_package(GTest)

### Find dependencies ###
find_package(GTest CONFIG REQUIRED)	# GTest::main

enable_testing()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_DEBUG_POSTFIX "D")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

add_subdirectory(src)
add_subdirectory(test)
