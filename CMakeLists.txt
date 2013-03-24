cmake_minimum_required(VERSION 2.8)
project(SSVSCollision)

set(SSVSCOLLISION_BUILD_SHARED_LIB FALSE CACHE BOOL "Build shared libraries.")
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type." FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" CACHE STRING "" FORCE)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/;${CMAKE_SOURCE_DIR}/extlibs/SSVUtils/cmake/Modules/;${CMAKE_SOURCE_DIR}/extlibs/SSVStart/cmake/Modules/;${CMAKE_MODULE_PATH}")

find_package(SFML 2 COMPONENTS system)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
else()
    set(SFML_ROOT "" CACHE PATH "SFML top-level directory")
    message("\n-> SFML directory not found. Set SFML_ROOT to SFML's top-level path (containing both \"include\" and \"lib\" directories).")
endif()

find_package(SSVUtils)
if(SSVUTILS_FOUND)
    include_directories(${SSVUTILS_INCLUDE_DIR})
else()
    set(SSVUTILS_ROOT "" CACHE PATH "SSVUtils top-level directory")
    message("\n-> SSVUtils directory not found. Set SSVUTILS_ROOT to SSVUtils' top-level path (containing both \"include\" and \"lib\" directories).")
endif()

find_package(SSVStart)
if(SSVSTART_FOUND)
    include_directories(${SSVSTART_INCLUDE_DIR})
else()
    set(SSVSTART_ROOT "" CACHE PATH "SSVStart top-level directory")
    message("\n-> SSVStart directory not found. Set SSVSTART_ROOT to SSVStart's top-level path (containing both \"include\" and \"lib\" directories).")
endif()

set(SFML_INCLUDE_DIR "" CACHE STRING "SFML2 include directory")
set(SFML_SYSTEM_LIBRARY "" CACHE STRING "SFML2 System library file")

set(SSVUTILS_STATIC_LIBS "" CACHE BOOL "Check to link SSVUtils statically.")
set(SSVUTILS_INCLUDE_DIR "" CACHE STRING "SSVUtils include directory")
set(SSVUTILS_LIBRARY "" CACHE STRING "SSVUtils library file")

set(SSVSTART_STATIC_LIBS "" CACHE BOOL "Check to link SSVStart statically.")
set(SSVSTART_INCLUDE_DIR "" CACHE STRING "SSVStart include directory")
set(SSVSTART_LIBRARY "" CACHE STRING "SSVStart library file")

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
	${INC_DIR}/${PROJECT_NAME}/Utils/Utils.h
	${INC_DIR}/${PROJECT_NAME}/World/World.h
	${SRC_DIR}/${PROJECT_NAME}/AABB/AABB.cpp
	${SRC_DIR}/${PROJECT_NAME}/Body/Body.cpp
	${SRC_DIR}/${PROJECT_NAME}/Body/BodyProperties.cpp
	${SRC_DIR}/${PROJECT_NAME}/Resolver/Retro/Retro.cpp
	${SRC_DIR}/${PROJECT_NAME}/Spatial/Grid/Cell.cpp
	${SRC_DIR}/${PROJECT_NAME}/Spatial/Grid/Grid.cpp
	${SRC_DIR}/${PROJECT_NAME}/Spatial/Grid/GridInfo.cpp
	${SRC_DIR}/${PROJECT_NAME}/Utils/Utils.cpp
	${SRC_DIR}/${PROJECT_NAME}/World/World.cpp
)

include_directories(${INC_DIR})

add_library(${PROJECT_NAME} ${LIB_TYPE} ${SOURCES})
target_link_libraries(SSVSCollision ${SFML_LIBRARIES})
target_link_libraries(SSVSCollision ${SSVUTILS_LIBRARIES})
target_link_libraries(SSVSCollision ${SSVSTART_LIBRARIES})

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
