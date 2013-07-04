cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type.")
project(SSVSCollision)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -W -pedantic" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-fno-omit-frame-pointer -g3 -gdwarf-2 -O0" CACHE STRING "" FORCE)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/;${CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/extlibs/SSVUtils/cmake/modules/;${CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/extlibs/SSVStart/cmake/modules/;${CMAKE_MODULE_PATH}")
set(SSVSCOLLISION_BUILD_SHARED_LIB TRUE CACHE BOOL "Build shared libraries.")

if(SSVSCOLLISION_BUILD_SHARED_LIB)
	set(LIB_TYPE SHARED)
else()
	set(LIB_TYPE STATIC)
endif()

set(SFML_STATIC_LIBRARIES FALSE CACHE BOOL "Look for static SFML libraries.")
find_package(SFML 2 COMPONENTS system)
if(NOT SFML_FOUND)
	set(SFML_INCLUDE_DIR "" CACHE STRING "SFML2 include directory")
	set(SFML_SYSTEM_LIBRARY "" CACHE STRING "SFML2 System library file")
	message("\n-> SFML directory not found. Set include and libraries manually.")
endif()

find_package(Sparsehash REQUIRED)
find_package(SSVUtils REQUIRED)
find_package(SSVStart REQUIRED)

set(SRC_DIR "src")
set(INC_DIR "include")

file(GLOB SRC_LIST
	"${INC_DIR}/${PROJECT_NAME}/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/*.cpp"
	"${INC_DIR}/${PROJECT_NAME}/AABB/*.h"
	"${INC_DIR}/${PROJECT_NAME}/Body/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/Body/*.cpp"
	"${INC_DIR}/${PROJECT_NAME}/Global/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/Global/*.cpp"
	"${INC_DIR}/${PROJECT_NAME}/Resolver/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/Resolver/*.cpp"
	"${INC_DIR}/${PROJECT_NAME}/Resolver/Impulse/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/Resolver/Impulse/*.cpp"
	"${INC_DIR}/${PROJECT_NAME}/Resolver/Retro/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/Resolver/Retro/*.cpp"
	"${INC_DIR}/${PROJECT_NAME}/Spatial/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/Spatial/*.cpp"
	"${INC_DIR}/${PROJECT_NAME}/Spatial/Grid/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/Spatial/Grid/*.cpp"
	"${INC_DIR}/${PROJECT_NAME}/Utils/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/Utils/*.cpp"
	"${INC_DIR}/${PROJECT_NAME}/World/*.h"
	"${SRC_DIR}/${PROJECT_NAME}/World/*.cpp"
)

include_directories(${INC_DIR})
include_directories(${SPARSEHASH_INCLUDE_DIR})
include_directories(${SFML_INCLUDE_DIR})
include_directories(${SSVUTILS_INCLUDE_DIR})
include_directories(${SSVSTART_INCLUDE_DIR})
add_library(${PROJECT_NAME} ${LIB_TYPE} ${SRC_LIST})
target_link_libraries(SSVSCollision ${SFML_SYSTEM_LIBRARY})
target_link_libraries(SSVSCollision ${SSVUTILS_LIBRARY})
target_link_libraries(SSVSCollision ${SSVSTART_LIBRARY})

if(SSVSCOLLISION_BUILD_SHARED_LIB)
	set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX -d)
else()
	set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX -s-d)
	set_target_properties(${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX -s)
endif()

if(WIN32)
	install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
else()
	install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION lib LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
endif()

install(DIRECTORY ${INC_DIR} DESTINATION .)
