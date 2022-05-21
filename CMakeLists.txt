cmake_minimum_required(VERSION 3.22.3)

set(CMAKE_TOOLCHAIN_FILE "${VCPKG_HOME}/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x86-windows-static")

set(VCPKG_INSTALL_CMD ${VCPKG_ROOT}/vcpkg.exe install --triplet ${VCPKG_TARGET_TRIPLET} --feature-flags manifests)
execute_process(COMMAND ${VCPKG_INSTALL_CMD} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

project(ThreadWatcher)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GTest CONFIG REQUIRED)

enable_testing()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_DEBUG_POSTFIX "D")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

add_subdirectory(src)
add_subdirectory(test)
