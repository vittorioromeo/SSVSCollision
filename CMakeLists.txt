cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type.")
project(SSVSCollision)

set(CMAKE_CXX_FLAGS "-std=c++1y -Wall -Wextra -W -pedantic -pthread" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-fno-omit-frame-pointer -g3 -gdwarf-2 -O0" CACHE STRING "" FORCE)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/;${CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/extlibs/SSVUtils/cmake/modules/;${CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/extlibs/SSVStart/cmake/modules/;${CMAKE_MODULE_PATH}")

set(SFML_STATIC_LIBRARIES FALSE CACHE BOOL "Look for static SFML libraries.")
find_package(SFML 2.1 COMPONENTS system)
if(NOT SFML_FOUND)
	set(SFML_INCLUDE_DIR "" CACHE STRING "SFML2 include directory")
	set(SFML_SYSTEM_LIBRARY "" CACHE STRING "SFML2 System library file")
	message("\n-> SFML directory not found. Set include and libraries manually.")
endif()

find_package(SSVUtils REQUIRED)
find_package(SSVStart REQUIRED)

set(SRC_DIR "src")
set(INC_DIR "include")
file(GLOB_RECURSE SRC_LIST "${INC_DIR}/*" "${SRC_DIR}/*")

include_directories(${INC_DIR})
include_directories(${SFML_INCLUDE_DIR})
include_directories(${SSVUTILS_INCLUDE_DIR})
include_directories(${SSVSTART_INCLUDE_DIR})
add_library(HEADER_ONLY_TARGET STATIC ${SRC_LIST})
set_target_properties(HEADER_ONLY_TARGET PROPERTIES LINKER_LANGUAGE CXX)

install(DIRECTORY ${INC_DIR} DESTINATION .)
