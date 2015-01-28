cmake_minimum_required(VERSION 2.8)
project(SSVSCollision)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../SSVCMake/cmake/modules/;${CMAKE_SOURCE_DIR}/extlibs/SSVCMake/cmake/modules/;${CMAKE_MODULE_PATH}")
include(SSVCMake)

SSVCMake_setDefaults()
SSVCMake_findSFML()
SSVCMake_findExtlib(SSVUtils)
SSVCMake_findExtlib(SSVStart)
SSVCMake_setAndInstallHeaderOnly()
