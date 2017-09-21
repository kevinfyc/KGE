
cmake_minimum_required(VERSION 2.8)

project(kge)

##################################################
#prepare
##################################################
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
include(common)

message(STATUS "cmake kge src dir: " ${PROJECT_SOURCE_DIR})
message(STATUS "cmake kge bin dir: " ${PROJECT_BINARY_DIR})

##################################################
#third party
##################################################
set(THIRD_PARTY_LIBS FreeType)

if(NOT IOS AND NOT ANDROID)
    list(APPEND THIRD_PARTY_LIBS glew)
endif()

foreach (TARGET ${THIRD_PARTY_LIBS})
    add_subdirectory (src/third_party/${TARGET})
endforeach ()

##################################################
#engine source
##################################################

add_subdirectory(src)
add_subdirectory(app)
