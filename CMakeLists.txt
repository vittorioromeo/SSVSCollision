cmake_minimum_required(VERSION 2.8)
project(SSVSCollision)

set(SSVSCOLLISION_BUILD_SHARED_LIB FALSE CACHE BOOL "Build shared libraries.")
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type." FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" CACHE STRING "" FORCE)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/;${CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/extlibs/SSVUtils/cmake/Modules/;${CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/extlibs/SSVStart/cmake/Modules/;${CMAKE_MODULE_PATH}")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -pg -Og" CACHE STRING "" FORCE)

set(SPARSEHASH_INCLUDE_DIR "" CACHE STRING "google::sparsehash include directory")

set(SFML_STATIC_LIBRARIES FALSE CACHE BOOL "Look for static SFML libraries.")

find_package(SFML 2 COMPONENTS system)
if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
	message("\nSFML found and included\n")
else()
	set(SFML_INCLUDE_DIR "" CACHE STRING "SFML2 include directory")
	set(SFML_SYSTEM_LIBRARY "" CACHE STRING "SFML2 System library file")
	message("\n-> SFML directory not found. Set include and libraries manually.")
endif()

find_package(SSVUtils REQUIRED)
if(SSVUTILS_FOUND)
	include_directories(${SSVUTILS_INCLUDE_DIR})
	message("\nSSVUtils found and included\n")
else()
	set(SSVUTILS_ROOT "" CACHE PATH "SSVUtils top-level directory")
	message("\n-> SSVUtils directory not found. Set SSVUTILS_ROOT to SSVUtils' top-level path (containing both \"include\" and \"lib\" directories).")
endif()

find_package(SSVStart REQUIRED)
if(SSVSTART_FOUND)
	include_directories(${SSVSTART_INCLUDE_DIR})
	message("\nSSVStart found and included\n")
else()
	set(SSVSTART_ROOT "" CACHE PATH "SSVStart top-level directory")
	message("\n-> SSVStart directory not found. Set SSVSTART_ROOT to SSVStart's top-level path (containing both \"include\" and \"lib\" directories).")
endif()

if(SSVSCOLLISION_BUILD_SHARED_LIB)
	set(LIB_TYPE SHARED)
else()
	set(LIB_TYPE STATIC)
endif()

set(SRC_DIR "src")
set(INC_DIR "include")

set(
	SOURCES
	${INC_DIR}/${PROJECT_NAME}/AABB/AABB.h
	${INC_DIR}/${PROJECT_NAME}/Body/Body.h
	${INC_DIR}/${PROJECT_NAME}/Body/CallbackInfo.h
	${INC_DIR}/${PROJECT_NAME}/Resolver/ResolverBase.h
	${INC_DIR}/${PROJECT_NAME}/Resolver/Retro/Retro.h
	${INC_DIR}/${PROJECT_NAME}/Spatial/SpatialBase.h
	${INC_DIR}/${PROJECT_NAME}/Spatial/SpatialInfoBase.h
	${INC_DIR}/${PROJECT_NAME}/Spatial/Grid/Cell.h
	${INC_DIR}/${PROJECT_NAME}/Spatial/Grid/Grid.h
	${INC_DIR}/${PROJECT_NAME}/Spatial/Grid/GridInfo.h
	${INC_DIR}/${PROJECT_NAME}/Spatial/Grid/GridQuery.h
	${INC_DIR}/${PROJECT_NAME}/Spatial/Grid/GridQueryTraits.h
	${INC_DIR}/${PROJECT_NAME}/Utils/Utils.h
	${INC_DIR}/${PROJECT_NAME}/World/World.h
	${SRC_DIR}/${PROJECT_NAME}/AABB/AABB.cpp
	${SRC_DIR}/${PROJECT_NAME}/Body/Body.cpp
	${SRC_DIR}/${PROJECT_NAME}/Body/BodyProperties.cpp
	${SRC_DIR}/${PROJECT_NAME}/Resolver/Retro/Retro.cpp
	${SRC_DIR}/${PROJECT_NAME}/Spatial/Grid/Cell.cpp
	${SRC_DIR}/${PROJECT_NAME}/Spatial/Grid/Grid.cpp
	${SRC_DIR}/${PROJECT_NAME}/Spatial/Grid/GridInfo.cpp
	${SRC_DIR}/${PROJECT_NAME}/Spatial/Grid/GridQuery.cpp
	${SRC_DIR}/${PROJECT_NAME}/Spatial/Grid/GridQueryTraits.cpp
	${SRC_DIR}/${PROJECT_NAME}/Utils/Utils.cpp
	${SRC_DIR}/${PROJECT_NAME}/World/World.cpp
)

include_directories(${INC_DIR})
include_directories(${SPARSEHASH_INCLUDE_DIR})

add_library(${PROJECT_NAME} ${LIB_TYPE} ${SOURCES})
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
	install(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)
else()
	install(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION lib
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)
endif()

install(DIRECTORY ${INC_DIR}
	DESTINATION .)
